// ---------------------------------------------------------------------------
// - Processor.cpp                                                           -
// - iato:iaoo client - processor class implementation                       -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
// -                                              Pierre Villalon  2002:2003 -
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
#include "Soo.hpp"
#include "Abi.hpp"
#include "Rob.hpp"
#include "Mrt.hpp"
#include "Mta.hpp"
#include "Irb.hpp"
#include "Irt.hpp"
#include "Eib.hpp"
#include "IpgStg.hpp"
#include "DecStg.hpp"
#include "LrnStg.hpp"
#include "PrnStg.hpp"
#include "SlcStg.hpp"
#include "RegStg.hpp"
#include "ExeStg.hpp"
#include "WbkStg.hpp"
#include "CmtStg.hpp"
#include "Resteer.hpp"
#include "RseLogic.hpp"
#include "Exception.hpp"
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
    env->add (new Urf      (stx));
    env->add (new Rob      (stx));
    env->add (new Irb      (stx));
    env->add (new Irt      (stx));
    env->add (new Gcs      (stx));
    env->add (new Bpn      (stx));
    env->add (new Mob      (stx));
    env->add (new Iib      (stx));
    env->add (new Eib      (stx));
    env->add (new Detect   (stx));
    env->add (new Resteer  (stx));
    env->add (new Register (stx));
    env->add (new RseLogic (stx));
    // add complex resources
    env->add (Branch::mkbr    (stx));
    env->add (Predicate::mkpr (stx));
    return env;
  }

  // this procedure create the M micro pipeline
  static Pipeline* build_m_pipe (Stx* stx, const long index) {
    // build pipeline name
    ostringstream os;
    os << "MP" << index;
    // get the pipeline latency
    long plat = stx->getlong ("LATENCY-M-UNIT") - 1;
    assert (plat >= 0);
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new SlcStg (stx, MUNIT));
    pipe->add (new RegStg (stx, MUNIT));
    for (long i = 0; i < plat; i++) pipe->add (new DlyStg (stx, MUNIT));
    pipe->add (new ExeStg (stx, MUNIT));
    pipe->add (new WbkStg (stx, MUNIT));
    return pipe;
  }

  // this procedure create the I micro pipeline
  static Pipeline* build_i_pipe (Stx* stx, const long index) {
    // build pipeline name
    ostringstream os;
    os << "IP" << index;
    // get the pipeline latency
    long plat = stx->getlong ("LATENCY-I-UNIT") - 1;
    assert (plat >= 0);
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new SlcStg (stx, IUNIT));
    pipe->add (new RegStg (stx, IUNIT));
    for (long i = 0; i < plat; i++) pipe->add (new DlyStg (stx, IUNIT));
    pipe->add (new ExeStg (stx, IUNIT));
    pipe->add (new WbkStg (stx, IUNIT));
    return pipe;
  }

  // this procedure create the F micro pipeline
  static Pipeline* build_f_pipe (Stx* stx, const long index) {
    // build pipeline name
    ostringstream os;
    os << "FP" << index;
    // get the pipeline latency
    long plat = stx->getlong ("LATENCY-F-UNIT") - 1;
    assert (plat >= 0);
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new SlcStg (stx, FUNIT));
    pipe->add (new RegStg (stx, FUNIT));
    for (long i = 0; i < plat; i++) pipe->add (new DlyStg (stx, FUNIT));
    pipe->add (new ExeStg (stx, FUNIT));
    pipe->add (new WbkStg (stx, FUNIT));
    return pipe;
  }

  // this procedure create the B micro pipeline
  static Pipeline* build_b_pipe (Stx* stx, const long index) {
    // build pipeline name
    ostringstream os;
    os << "BP" << index;
    // get the pipeline latency
    long plat = stx->getlong ("LATENCY-B-UNIT") - 1;
    assert (plat >= 0);
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx, os.str ());
    // add the stages
    pipe->add (new SlcStg (stx, BUNIT));
    pipe->add (new RegStg (stx, BUNIT));
    for (long i = 0; i < plat; i++) pipe->add (new DlyStg (stx, BUNIT));
    pipe->add (new ExeStg (stx, BUNIT));
    pipe->add (new WbkStg (stx, BUNIT));
    return pipe;
  }

  // this procedure create the processor pipelane
  static Pipelane* build_lane (Stx* stx) {
    // create the pipelane
    Pipelane* lane = new Pipelane (stx);
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
  static Pipeline* build_pipe (Stx* stx) {
    // create the pipeline
    Pipeline* pipe = new Pipeline (stx);
    // add the pipeline stages
    pipe->add (new IpgStg (stx));
    pipe->add (new DecStg (stx));
    pipe->add (new LrnStg (stx));
    pipe->add (new PrnStg (stx));
    // add the pipelane
    pipe->add (build_lane (stx));
    // finish the pipeline
    pipe->add (new CmtStg (stx));
    return pipe;
  }

  // create a new processor

  Processor::Processor (Stx* stx) {
    // create the environment
    p_env  = build_env  (stx);
    // create the pipeline
    p_pipe = build_pipe (stx);
    // create the watchdog
    p_wdog = new Watchdog (stx);
    // add the pipeline to the environment
    p_env->add (p_pipe);  
    // add the watchdog to the environment
    p_env->add (p_wdog);  
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
    p_env->reset  ();
    // get the bank and initialize
    Register* rbk = dynamic_cast <Register*> (p_env->get (RESOURCE_RBK));
    if (rbk) {
      rbk->write (AREG, AR_FPSR, DEF_FPSR);
      rbk->write (IPRG, 0,       d_entry);
      rbk->write (GREG, ABI_SP,  d_stkva);
      if (d_argva != OCTA_0) {
        rbk->write (GREG, ABI_ARG, d_argva);
      }
      if (d_tlsva != OCTA_0) rbk->write (GREG, ABI_TP,  d_tlsva);
      if (d_gpva  != OCTA_0) {
	rbk->write (GREG, ABI_GP, d_gpva);
	rbk->write (GREG, ABI_GP_ALIAS, d_gpva);
      }
      if (d_gpva != OCTA_0) {
        rbk->write (GREG, 35, d_gpva + 400);
        rbk->write (GREG, 36, d_gpva + 800);
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
    Irt*      irt = dynamic_cast <Irt*>      (p_env->get (RESOURCE_IRT));
    Rse*      rse = dynamic_cast <Rse*>      (p_env->get (RESOURCE_RSE));
    Detect*   dtl = dynamic_cast <Detect*>   (p_env->get (RESOURCE_DTL));
    Restart*  pfr = dynamic_cast <Restart*>  (p_env->get (RESOURCE_PFR));
    Register* rbk = dynamic_cast <Register*> (p_env->get (RESOURCE_RBK));
    // bind the pipeline
    p_pipe->bind (p_env, 0, 0);
    // bind the detection logic
    dtl->bind (rbk);
    // bind the restart resource
    pfr->bind (p_pipe, p_env);
    // bind the syscall plugin
    if (irt) {
      Syscall* sci = sys->getsci ();
      if (sci) {
	sci->setrse (rse);
	sci->setrbk (rbk);
	irt->bind (FAULT_IT_INST_BREAK, sci);
      }
    }
  }

  // return trhe processor register bank

  Register* Processor::getrbk (void) const {
    Register* rbk = dynamic_cast <Register*> (p_env->get (RESOURCE_RBK));
    return rbk;
  }
}
