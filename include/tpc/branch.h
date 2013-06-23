/******************************************************************************

  Copyright (c) 1997,2002,2009,2012 by Turku PET Center

  branch.h

  Contains the branching ratios of isotopes.
  To correct for activities, multiply them with 1.0/branching_ratio.

  Reference: Table of Isotopes, Sixth edition, edited by C.M. Lederer,
  J.M. Hollander, I. Perlman. WILEY, 1967.

  Updated:
  2002-08-08 Vesa Oikonen
    Added Ga-68, and the reference.
  2009-03-13 VO
    Added N-13 and Rb-82.
  2012-01-31 VO
    Added Cu-64 with value 0.174 as used in Inveon images.
  2012-02-23 VO
    Added function branchingFraction().


******************************************************************************/
#ifndef _BRANCH_H_
#define _BRANCH_H_
/*****************************************************************************/
#define BRANCHING_O 0.999
#define BRANCHING_C 0.998
#define BRANCHING_Cu64 0.174
#define BRANCHING_N 0.998
#define BRANCHING_F 0.967
#define BRANCHING_Ge 0.891
#define BRANCHING_Ga 0.891
#define BRANCHING_Rb 0.950
/*****************************************************************************/
extern float branchingFraction(int isotope);
/*****************************************************************************/
#endif

