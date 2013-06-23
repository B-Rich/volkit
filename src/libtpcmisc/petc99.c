/*****************************************************************************

  petc99.c  (c) 2003,2004 Turku PET Centre

  This file contains the ISO C99 functions that are not yet available in
  all C compilers but that are required by the PET library.
  The functions are named as temp_functionname() to prevent problems with
  compilers that already have these functions.
  
  Version:
  2003-11-30 Vesa Oikonen
  2004-11-07 VO
    Test the compiler whether to use these functions
  2004-11-26 Calle Laakkonen
    Fixed temp_roundf (rounded negative numbers incorrectly)


*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/****************************************************************************/
#include "petc99.h"
/****************************************************************************/

/*****************************************************************************/
/*!
 * int roundf(float e)  - Rounds up float e to nearest int
 *
 * @param e float value
 * @return rounded integer
 */
int temp_roundf(float e)
{
#if defined(__STDC_VERSION__) && __STD_VERSION__>=199901L
  return(roundf(e));
#else
  if(e<0.0) {
      return (int)(e-0.5);
  } else {
      return (int)(e+0.5);
  }
#endif
}
/****************************************************************************/

/****************************************************************************/

