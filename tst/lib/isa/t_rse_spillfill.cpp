// ---------------------------------------------------------------------------
// - t_rse_spillfill.cpp                                                     -
// - iato:isa testers - rse spill/fill stress test                            -
// ---------------------------------------------------------------------------
// - This test forces the stacked-register file (GR_LRSZ == 128) to wrap and  -
// - requires backing-store spill/fill to preserve values and NaT bits across -
// - deep call/return transitions.                                            -
// ---------------------------------------------------------------------------

#include <cassert>
#include <vector>

#include "Isa.hpp"
#include "Cfm.hpp"
#include "Rse.hpp"
#include "Register.hpp"
#include "Segment.hpp"

namespace {
  using namespace iato;

  static t_octa mkval (const long frame, const long reg) {
    return (static_cast<t_octa>(frame) << 32) ^ static_cast<t_octa>(reg);
  }

  static bool mknat (const long frame, const long reg) {
    return ((frame ^ reg) & 0x1) != 0;
  }

  static void write_locals (const Rse::State& st, Register& rbk,
			    const long frame, const long sol) {
    for (long i = 0; i < sol; i++) {
      const long lnum = GR_STBS + i;
      const long preg = st.mapgr (lnum, false);
      rbk.write (GREG, preg, mkval (frame, i));
      rbk.write (NREG, preg, mknat (frame, i));
    }
  }

  static void check_locals (const Rse::State& st, const Register& rbk,
			    const long frame, const long sol) {
    for (long i = 0; i < sol; i++) {
      const long lnum = GR_STBS + i;
      const long preg = st.mapgr (lnum, false);
      assert (rbk.getoval (GREG, preg) == mkval (frame, i));
      assert (rbk.getbval (NREG, preg) == mknat (frame, i));
    }
  }
}

int main (int, char**) {
  using namespace iato;

  // Parameters chosen to force wraparound of the 96 stacked physical GRs
  // (r32..r127) and thus require spill/fill to preserve older frame locals.
  const long DEPTH = 12;
  const t_byte SOF = 32;
  const t_byte SOL = 16;

  // Backing store memory.
  Segment bmem (1 << 20);

  // Register bank and initial BSP/BSPSTORE/RNAT setup.
  Register rbk;
  rbk.reset ();
  rbk.write (AREG, AR_BSP,  OCTA_0);
  rbk.write (AREG, AR_BSPS, OCTA_0);
  rbk.write (AREG, AR_RNAT, OCTA_0);

  // RSE state with backing store binding.
  Rse::State st;
  st.bind (&bmem, &rbk);

  // Build an initial frame.
  Cfm cfm0 = OCTA_0;
  cfm0.alloc (SOF, SOL, 0);
  st.alloc (cfm0);
  st.spill ();

  // Keep each frame's cfm so we can "return" to it later.
  std::vector<Cfm> frames;
  frames.push_back (cfm0);

  // Root frame locals.
  write_locals (st, rbk, 0, SOL);
  check_locals (st, rbk, 0, SOL);

  // Drive a deep call chain. Each frame allocates locals and writes them.
  for (long f = 1; f <= DEPTH; f++) {
    // call -> callee prologue state
    Cfm ocfm = frames.back ();
    Cfm ncfm = OCTA_0;
    ncfm.call (ocfm);
    st.call (ncfm);
    st.spill ();

    // callee alloc
    Cfm cfm = OCTA_0;
    cfm.alloc (SOF, SOL, 0);
    st.alloc (cfm);
    st.spill ();

    frames.push_back (cfm);

    // write locals for this frame
    write_locals (st, rbk, f, SOL);
    check_locals (st, rbk, f, SOL);
  }

  // Sanity: ensure the physical registers that initially held frame-0 locals
  // have been overwritten by deep frames (otherwise this test is not actually
  // stressing spill/fill).
  bool overwritten = false;
  for (long i = 0; i < SOL; i++) {
    const long preg = GR_STBS + i;
    if (rbk.getoval (GREG, preg) != mkval (0, i)) {
      overwritten = true;
      break;
    }
  }
  assert (overwritten == true);

  // Unwind and validate locals at each return boundary.
  for (long f = DEPTH; f >= 1; f--) {
    st.retn (frames[f - 1]);
    st.fill ();
    check_locals (st, rbk, f - 1, SOL);
  }

  // At the end, we are back in frame 0 and its locals must match.
  check_locals (st, rbk, 0, SOL);

  return 0;
}


