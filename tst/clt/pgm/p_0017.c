/*
 * ---------------------------------------------------------------------------
 * - p_0017.c                                                                -
 * - iato:pgm tester : compex tester that make iaka life miserable           -
 * ---------------------------------------------------------------------------
 * - (c) inria 2002-2004                                                     -
 * ---------------------------------------------------------------------------
 * - authors                                      Amaury Darsch    2002:2004 -
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

#include <stdio.h>
#include <stdlib.h>

/*
 * This example is taken from crafty - specifically, it is the piece of code
 * that permits to clear a hash table
 */
#define And(a,b)    ((a) & (b))
#define Or(a,b)     ((a) | (b))
#define Xor(a,b)    ((a) ^ (b))
#define Compl(a)    (~(a))
#define Shiftl(a,b) ((a) << (b))
#define Shiftr(a,b) ((a) >> (b))
/*
 * The bitboard type
 */

typedef unsigned long long int BITBOARD;

/*
 * The hash entry
 */

typedef struct {
  BITBOARD word1;
  BITBOARD word2;
} HASH_ENTRY;

/*
 * base size constants
 */
long hash_table_size  = 0;
long mask_clear_entry = 0;

/*
 * base constants
 */
HASH_ENTRY* trans_ref_ba = NULL;
HASH_ENTRY* trans_ref_wa = NULL;
HASH_ENTRY* trans_ref_bb = NULL;
HASH_ENTRY* trans_ref_wb = NULL;

/*
 * hash clear
 */

static void clear_hash_table (void) {
  long i = 0;
  printf ("clearing hash table\n");
  if (trans_ref_ba && trans_ref_wa) {
    for (i=0;i<hash_table_size;i++) {
      (trans_ref_ba+i)->word1=Or(And((trans_ref_ba+i)->word1,
			mask_clear_entry),Shiftl((BITBOARD) 65536,21));
      (trans_ref_wa+i)->word1=Or(And((trans_ref_wa+i)->word1,
		        mask_clear_entry),Shiftl((BITBOARD) 65536,21));
    }
    for (i=0;i<2*hash_table_size;i++) {
      (trans_ref_bb+i)->word1=Or(And((trans_ref_bb+i)->word1,
                        mask_clear_entry),Shiftl((BITBOARD) 65536,21));
      (trans_ref_wb+i)->word1=Or(And((trans_ref_wb+i)->word1,
                        mask_clear_entry),Shiftl((BITBOARD) 65536,21));
    }
  }
}

/*
 * main bazard
 */

int main () {
  /* initialization */
  hash_table_size  = 1024;
  mask_clear_entry = Compl(Or(Shiftl(109,21),Shiftr(3,3)));
  /* hash allocation */
  printf ("allocating hash table\n");
  HASH_ENTRY* trans_ref_ba = malloc (16*hash_table_size);
  HASH_ENTRY* trans_ref_wa = malloc (16*hash_table_size);
  HASH_ENTRY* trans_ref_bb = malloc (16*2*hash_table_size);
  HASH_ENTRY* trans_ref_wb = malloc (16*2*hash_table_size);

  /* hash clear */
  clear_hash_table ();
 
  /* free data */
  printf ("freeing hash table\n");
  free (trans_ref_ba);
  free (trans_ref_wa);
  free (trans_ref_bb);
  free (trans_ref_wb);
  return 0;
}
