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
  
  // this procedure create a branch predictor from a context
  static Branch* build_bprd (Stx* stx) {
    // get the predictor name
    string name = stx->getstr ("BRANCH-PREDICTOR-TYPE");
    // check for default
    if ((name.length () == 0) || (name == "none")) return new Branch (stx);
    // check for bimodal
    if (name == "bimodal") return new Bimodal (stx);
    // check for gshare
    if (name == "gshare") return new Gshare (stx);    
    // not found
    string msg = "invalid branch predictor name ";
    throw Exception ("context-error", msg + name);
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
    env->add (build_bprd   (stx));
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
    // get the pipeline latency
    long flat = stx->getlong ("LATENCY-F-UNIT");
    assert ((flat == 1) || (flat == 4));
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
    // create the environment and pipeline
    p_env  = build_env  (stx);
    p_pipe = build_pipe (stx, p_env);
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
      rbk->write (AREG, AR_BSP , d_bspva);
      rbk->write (AREG, AR_BSPS, d_bspva);
    }
  }

  // flush this processor

  void Processor::flush (void) {
    p_pipe->flush ();
  }

  // run this processor

  void Processor::run (void) {
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
    // bind the memory architecture
    Hma* hma = sys->gethma ();
    if (hma) p_env->add (hma->getmta ());
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

  // return trhe processor register bank

  Register* Processor::getrbk (void) const {
    Register* rbk = dynamic_cast <Register*> (p_env->get (RESOURCE_RBK));
    return rbk;
  }
}
