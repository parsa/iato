// ---------------------------------------------------------------------------
// - Processor.cpp                                                           -
// - iato:iaio client - processor class implementation                       -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
// -                                              Andre  Seznec    2002:2004 -
// ---------------------------------------------------------------------------
// - This program  is  free software;  you can redistribute it and/or modify -
// - it  under  the  terms  of the GNU  General Public License version 2, as -
// - published by the Free Software Foundation.                              -
// -                                                                         -
// - This  program  is  distributed  in  the  hope  that it  will be useful, -
// - but   without  any  warranty; without  even  the  implied  warranty  of -
// - merchantability or fitness for a particular purpose.                    -
// -                                                                         -
// - See the GNU General Public License version 2 for more details           -
// ---------------------------------------------------------------------------

#include "Srn.hpp"
#include "Abi.hpp"
#include "Irt.hpp"
#include "Eib.hpp"
#include "Mrt.hpp"
#include "Mta.hpp"
#include "Exception.hpp"
#include "IpgStg.hpp"
#include "RotStg.hpp"
#include "ExpStg.hpp"
#include "RenStg.hpp"
#include "RegStg.hpp"
#include "ExeStg.hpp"
#include "DetStg.hpp"
#include "Fp1Stg.hpp"
#include "Fp2Stg.hpp"
#include "Fp3Stg.hpp"
#include "Fp4Stg.hpp"
#include "BobStg.hpp"
#include "BedStg.hpp"
#include "WrbStg.hpp"
#include "FwbStg.hpp"
#include "Gshare.hpp"
#include "Detect.hpp"
#include "Bimodal.hpp"
#include "Resteer.hpp"
#include "RseLogic.hpp"
#include "Pipelane.hpp"
#include "Processor.hpp"

namespace iato {
  using namespace std;

  namespace {
    void write_fixture_registers (Register* rbk, const TestFixture& fixture,
				  const FixtureContext& ctx) {
      if ((!rbk) || fixture.regs ().empty ()) return;
      for (const auto& entry : fixture.regs ()) {
	t_octa value = entry.value->resolve (ctx);
	switch (entry.type) {
	case PREG:
	  rbk->write (PREG, entry.index, (value & 0x1) != 0);
	  break;
	case FREG:
	  throw Exception ("TEST-FIXTURE-FILE: floating-point register seeds "
			   "are not supported");
	default:
	  rbk->write (entry.type, entry.index, value);
	  break;
	}
      }
    }

    void write_fixture_memory (Mta* mta, const TestFixture& fixture,
			       const FixtureContext& ctx) {
      if (fixture.mems ().empty ()) return;
      if (!mta) {
	throw Exception (
	    "TEST-FIXTURE-FILE contains memory data but no memory adapter "
	    "is available for this client");
      }
      for (const auto& entry : fixture.mems ()) {
	t_octa addr  = entry.address->resolve (ctx);
	t_octa value = entry.value->resolve (ctx);
	Mrt mrt;
	switch (entry.size) {
	case 1:
	  mrt = Mrt (Mrt::REQ_ST1, addr);
	  mrt.setbval (static_cast<t_byte> (value & 0xFFULL));
	  break;
	case 2:
	  mrt = Mrt (Mrt::REQ_ST2, addr);
	  mrt.setwval (static_cast<t_word> (value & 0xFFFFULL));
	  break;
	case 4:
	  mrt = Mrt (Mrt::REQ_ST4, addr);
	  mrt.setqval (static_cast<t_quad> (value & 0xFFFFFFFFULL));
	  break;
	case 8:
	  mrt = Mrt (Mrt::REQ_ST8, addr);
	  mrt.setoval (value);
	  break;
	default:
	  throw Exception ("TEST-FIXTURE-FILE: unsupported store width");
	}
	mta->process (mrt);
      }
    }

    void apply_fixture (Register* rbk, Mta* mta, const TestFixture& fixture,
			const FixtureContext& ctx) {
      if (fixture.empty ()) return;
      write_fixture_registers (rbk, fixture, ctx);
      write_fixture_memory (mta, fixture, ctx);
    }
  }
  
  // this procedure creates the processor environment from a context
  static Env* build_env (Stx* stx) {
    // create the environment
    Env* env = new Env;
    // create the global resources
    env->add (new Irt        (stx));
    env->add (new Eib        (stx));
    env->add (new Iib        (stx));
    env->add (new Bdb        (stx));
    env->add (new Detect     (stx));
    env->add (new Hazard     (stx));
    env->add (new Resteer    (stx));
    env->add (new Disperse   (stx));
    env->add (new Register   (stx));
    env->add (new RseLogic   (stx));
    env->add (new Scoreboard (stx));
    // specialized bypass networks
    env->add (new Bpn (stx, RESOURCE_EBN));
    env->add (new Bpn (stx, RESOURCE_LBN));
    // specialized memory bypass networks
    env->add (new Mbn (stx, RESOURCE_EMB));
    env->add (new Mbn (stx, RESOURCE_LMB));
    // issue and output port buffers
    env->add (new Spb (stx, RESOURCE_IPB));
    env->add (new Spb (stx, RESOURCE_OPB));
    // add complex resources
    env->add (Branch::mkbr (stx));
    return env;
  }

  // this procedure create the M micro pipeline
  static Pipeline* build_m_pipe (Stx* stx, const long index) {
    // build the pipeline
    ostringstream os;
    os << "PM" << index;
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new RenStg (stx, MUNIT, index));
    pipe->add (new RegStg (stx, MUNIT, index));
    pipe->add (new ExeStg (stx, MUNIT, index));
    pipe->add (new DetStg (stx, MUNIT, index));
    pipe->add (new WrbStg (stx, MUNIT, index));
    return pipe;
  }

  // this procedure create the I micro pipeline
  static Pipeline* build_i_pipe (Stx* stx, const long index) {
    // build the pipeline
    ostringstream os;
    os << "PI" << index;
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new RenStg (stx, IUNIT, index));
    pipe->add (new RegStg (stx, IUNIT, index));
    pipe->add (new ExeStg (stx, IUNIT, index));
    pipe->add (new DetStg (stx, IUNIT, index));
    pipe->add (new WrbStg (stx, IUNIT, index));
    return pipe;
  }

  // this procedure create the F micro pipeline
  static Pipeline* build_f_pipe (Stx* stx, const long index) {
    // build the pipeline
    ostringstream os;
    os << "PF" << index;
    long flat = stx->getlong ("LATENCY-F-UNIT");
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new RenStg (stx, FUNIT, index));
    pipe->add (new RegStg (stx, FUNIT, index));
    if (flat == 1) {
      pipe->add (new ExeStg (stx, FUNIT, index));
      pipe->add (new DetStg (stx, FUNIT, index));
    } else {
      pipe->add (new Fp1Stg (stx, FUNIT, index));
      pipe->add (new Fp2Stg (stx, FUNIT, index));
      pipe->add (new Fp3Stg (stx, FUNIT, index));
      pipe->add (new Fp4Stg (stx, FUNIT, index));
    }
    pipe->add (new FwbStg (stx, FUNIT, index));
    return pipe;
  }

  // this procedure create the B micro pipeline
  static Pipeline* build_b_pipe (Stx* stx, const long index) {
    // build the pipeline
    ostringstream os;
    os << "PB" << index;
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new RenStg (stx, BUNIT, index));
    pipe->add (new RegStg (stx, BUNIT, index));
    pipe->add (new BobStg (stx, BUNIT, index));
    pipe->add (new BedStg (stx, BUNIT, index));
    pipe->add (new WrbStg (stx, BUNIT, index));
    return pipe;
  }

  // this procedure create the processor pipelane
  static Pipelane* build_lane (Stx* stx) {
    // create the pipelane
    Pipelane* lane = new Pipelane (stx, true);
    // add the M pipelines
    long mnum = stx->getlong ("NUMBER-M-UNITS"); assert (mnum > 0);
    for (long i = 0; i < mnum; i++) lane->add (build_m_pipe (stx, i));
    // add the I pipelines
    long inum = stx->getlong ("NUMBER-I-UNITS"); assert (inum > 0);
    for (long i = 0; i < inum; i++) lane->add (build_i_pipe (stx, i));
    // add the F pipelines
    long fnum = stx->getlong ("NUMBER-F-UNITS"); assert (fnum > 0);
    for (long i = 0; i < fnum; i++) lane->add (build_f_pipe (stx, i));
    // add the B pipelines
    long bnum = stx->getlong ("NUMBER-B-UNITS"); assert (bnum > 0);
    for (long i = 0; i < bnum; i++) lane->add (build_b_pipe (stx, i));
    return lane;
  }

  // this procedure create the processor main pipeline
  static Pipeline* build_pipe (Stx* stx, Env* env) {
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx);
    // add the pipeline stages
    pipe->add (new IpgStg (stx));
    pipe->add (new RotStg (stx));
    pipe->add (new ExpStg (stx));
    // add the pipelane
    pipe->add (build_lane (stx));
    // add the pipeline in the environment
    env->add (pipe);
    return pipe;
  }

  // create a new processor

  Processor::Processor (Stx* stx) {
    // create the environment
    p_env  = build_env  (stx);
    // create the pipeline
    p_pipe = build_pipe (stx, p_env);
    // create the watchdog
    p_wdog = new Watchdog (stx);
    d_entry = OCTA_0;
    d_stkva = OCTA_0;
    d_bspva = OCTA_0;
    d_argva = OCTA_0;
    d_tlsva = OCTA_0;
    d_gpva  = OCTA_0;
    d_fixture_path   = stx->getstr ("TEST-FIXTURE-FILE");
    d_fixture_loaded = false;
    p_mta = nullptr;
  }

  // destroy this processor

  Processor::~Processor (void) {
    delete p_env;
  }

  // reset this processor

  void Processor::reset (void) {
    // reset all resources
    p_env->reset ();
    // get the bank and initialize
    Register* rbk = dynamic_cast <Register*> (p_env->get (RESOURCE_RBK));
    if (rbk) {
      rbk->write (AREG, AR_FPSR, DEF_FPSR);
      rbk->write (IPRG, 0,       d_entry);
      rbk->write (GREG, ABI_SP,  d_stkva);
      if (d_argva != OCTA_0) rbk->write (GREG, ABI_ARG, d_argva);
      if (d_tlsva != OCTA_0) rbk->write (GREG, ABI_TP,  d_tlsva);
      if (d_gpva  != OCTA_0) {
        // Linux/IA64 crt1.o expects r1 to hold an IP-relative GP delta, and
        // then computes the actual GP with `mov r9=ip; sub r1=r9,r1`.
        // Seed r1 with (entry_ip + 0x20) - gp so the prologue reconstructs gp.
        const t_octa ip_for_gp = d_entry + 0x20;
        const t_octa gp_seed   = ip_for_gp - d_gpva;
	rbk->write (GREG, ABI_GP, gp_seed);
	rbk->write (GREG, ABI_GP_ALIAS, d_gpva);
      }
      rbk->write (AREG, AR_BSP , d_bspva);
      rbk->write (AREG, AR_BSPS, d_bspva);
    }
    if (!d_fixture_path.empty ()) {
      if (!d_fixture_loaded) {
        d_fixture = TestFixture::load (d_fixture_path);
        d_fixture_loaded = true;
      }
      FixtureContext ctx;
      ctx.entry = d_entry;
      ctx.stack = d_stkva;
      ctx.bsp   = d_bspva;
      ctx.arg   = d_argva;
      ctx.tls   = d_tlsva;
      ctx.gp    = d_gpva;
      apply_fixture (rbk, p_mta, d_fixture, ctx);
    }
  }

  // flush this processor

  void Processor::flush (void) {
    p_pipe->flush ();
    p_wdog->reset ();
  }

  // run this processor

  void Processor::run (void) {
    // notify the watchdog
    p_wdog->notify ();
    // run the pipeline
    p_pipe->run ();
  }

  // set an external interrupt

  void Processor::extint (const Interrupt& vi) {
    // grab the interrupt buffer
    Eib* eib = dynamic_cast <Eib*> (p_env->get (RESOURCE_EIB));
    if (eib) eib->push (vi);
  }

  // report this processor

  void Processor::report (void) const {
    p_env->report ();
  }
  
  // return true if the processor is halted

  bool Processor::ishalted (void) const {
    return p_pipe->ishalted ();
  }

  // set the processor with a stat collector
  
  void Processor::setstc (Stat* stc) {
    if (!stc) return;
    if (p_env) p_env->setstc (stc);
  }
 
  // set the processor with a tracer
  
  void Processor::settrc (Tracer* tracer) {
    if (!tracer) return;
    if (p_env) p_env->settrc (tracer);
  }

  // bind this processor

  void Processor::bind (System* sys) {
    // chech for resource
    if (!sys) return;
    // bind memory initial values
    d_entry = sys->getentry ();
    d_stkva = sys->getstkva ();
    d_bspva = sys->getbspva ();
    d_argva = sys->getargva ();
    d_tlsva = sys->gettlsva ();
    d_gpva  = sys->getgpva ();
    // bind the watchdog
    p_env->add (p_wdog);
    // bind the memory architecture
    Hma* hma = sys->gethma ();
    p_mta = nullptr;
    if (hma) {
      Mta* mta = hma->getmta ();
      if (mta) {
        p_env->add (mta);
        p_mta = mta;
      }
    }
    // grab resource used for binding
    Irt*        irt = dynamic_cast <Irt*>        (p_env->get (RESOURCE_IRT));
    Iib*        iib = dynamic_cast <Iib*>        (p_env->get (RESOURCE_IIB));
    Rse*        rse = dynamic_cast <Rse*>        (p_env->get (RESOURCE_RSE));
    Spb*        ipb = dynamic_cast <Spb*>        (p_env->get (RESOURCE_IPB));
    Spb*        opb = dynamic_cast <Spb*>        (p_env->get (RESOURCE_OPB));
    Hazard*     hdl = dynamic_cast <Hazard*>     (p_env->get (RESOURCE_HDL));
    Restart*    pfr = dynamic_cast <Restart*>    (p_env->get (RESOURCE_PFR));
    Disperse*   dis = dynamic_cast <Disperse*>   (p_env->get (RESOURCE_BDS));
    Register*   rbk = dynamic_cast <Register*>   (p_env->get (RESOURCE_RBK));
    Scoreboard* psb = dynamic_cast <Scoreboard*> (p_env->get (RESOURCE_PSB));
    // bind backing store spill/fill support for the rse (requires memory + rbk)
    if (hma && rse && rbk) {
      Memory* mem = hma->getmem ();
      if (mem) rse->bind (mem, rbk);
    }
    // bind the pipeline
    p_pipe->bind (p_env, 0, 0);
    // bind the restart resource
    pfr->bind (p_pipe, p_env);
    // bind the syscall plugin
    Syscall* sci = sys->getsci ();
    if (sci && irt) {
      sci->setrse (rse);
      sci->setrbk (rbk);
      irt->bind (FAULT_IT_INST_BREAK, sci);
    }
    // bind the disperse logic
    if (dis) dis->bind (ipb, iib, psb);
    // bind the hazard logic
    if (hdl) hdl->bind (opb, psb);
  }

  // return the processor register bank

  Register* Processor::getrbk (void) const {
    Register* rbk = dynamic_cast <Register*> (p_env->get (RESOURCE_RBK));
    return rbk;
  }
}
