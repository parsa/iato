/* ---------------------------------------------------------------------------
 * - iato.lds                                                                -
 * - iato config : ld script that incorporates xit.o                         -
 * ---------------------------------------------------------------------------
 * - (c) inria 2002-2004                                                     -
 * ---------------------------------------------------------------------------
 * - authors                                      Amaury Darsch    2002:2004 -
 * -                                              Pierre Villalon  2002:2003 -
 * -                                              Andre  Seznec    2002:2004 -
 * ---------------------------------------------------------------------------
 * - This program is  free software;  you can redistribute  it and/or modify -
 * - it  under  the  terms of the  GNU  General Public License version 2, as -
 * - published by the Free Software Foundation.                              -
 * -                                                                         -
 * - This  program  is  distributed  in  the  hope that it  will be  useful, -
 * - but   without  any  warranty;  without  even  the implied  warranty  of -
 * - merchantability or fitness for a particular purpose.                    -
 * -                                                                         -
 * - See the GNU General Public License version 2 for more details           -
 * ---------------------------------------------------------------------------
 */

OUTPUT_ARCH (ia64)
INPUT       (xit.o)
ENTRY       (_start)

SECTIONS {
  /* start after all headers */
  . = 0x4000000000000000 + SIZEOF_HEADERS;

  /* incorporate test suite and fini code */
  .text : {
    *     (.text)
  }
  .fini : {
    xit.o (.fini)
  }

  /* incorporate iato data section for load/store test */
  .iato.data ALIGN (0x10) : {
    * (.iato.data)
  }

  /* incorporate data test suite */
  .iato.checker ALIGN (0x10) : {
    * (.iato.checker)
  }

  /* discard other section */
  /DISCARD/ : {
    * (.data)
    * (.bss)
    * (.IA_64.unwind_info)
    * (.IA_64.unwind)
  }
}
