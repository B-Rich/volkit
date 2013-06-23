/******************************************************************************
  Copyright (c) 2006,2008,2009,2012 Turku PET Centre

  File:        decpoint.c
  Description: Contains functions for reading real numbers from strings which
               may contain either decimal dots or commas.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details:
  http://www.gnu.org/copyleft/lesser.html

  You should have received a copy of the GNU Lesser General Public License
  along with this library/program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi

  Modification history:
  2006-06-03 Vesa Oikonen
    First created.
  2008-10-10 VO
    Added function to calculate nr of decimals, dec_nr()).
  2009-06-09 VO
    Added function atof_with_check() although this does not directly relate to
    decimal points.
  2012-03-04 VO
    Added functions dec_separator(), dec_separator_change(), and
    strPtrToNextValue().
    Fixed a bug in dec_comma_is().
    Function atof_dpi() sets only the first decimal comma to dot. String is not
    modified.
  2012-03-05 VO
    Further changes in strPtrToNextValue().


******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
/*****************************************************************************/
#include "decpoint.h"
/*****************************************************************************/

/*****************************************************************************/
/** Checks whether argument string contains a decimal comma instead of dot.
\return Returns 1 if decimal comma is found and 0 if not found.
 */
int dec_comma_is(
  /** Pointer to string (not modified). */
  char *str
) {
  if(strchr(str, '.')!=NULL) return(0);
  if(strchr(str, ',')!=NULL) return(1);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Checks whether argument string contains a decimal comma or dot, or neither.
\return Returns 0, if neither is found, 1 if dot, and 2 if comma is found.
 */
int dec_separator(
  /** Pointer to string (not modified). */
  char *str
) {
  if(strchr(str, '.')!=NULL) return(1);
  if(strchr(str, ',')!=NULL) return(2);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Convert the first decimal separator to comma or dot, as required. */
void dec_separator_change(
  /** Pointer to string (modified when necessary). */
  char *str,
  /** Requested decimal separator: 0=dot, 1=comma. */
  int decsep
) {
  char *cptr;
  cptr=strchr(str, '.'); if(cptr!=NULL && decsep==1) {*cptr=','; return;}
  cptr=strchr(str, ','); if(cptr!=NULL && decsep==0) {*cptr='.'; return;}
  return;
}
/*****************************************************************************/

/*****************************************************************************/
/** Replacement of atof(), which works whether string contains decimal dots
 *  or decimal commas. Possible commas are replaced by dots in the
 *  argument string.
\return Returns the double float.
 */
double atof_dpi(
  /** Pointer to string (not modified). */
  char *str
) {
  char *cptr;
  double f;

  /* If string contains a dot, then use atof directly */
  if(strchr(str, '.')!=NULL) return(atof(str));
  /* Otherwise, convert all commas to dots */
  //while((cptr=strchr(str, ','))!=NULL) *cptr='.';
  cptr=strchr(str, ','); if(cptr!=NULL) *cptr='.';
  f=atof(str); if(cptr!=NULL) *cptr=',';
  return(f);
}
/*****************************************************************************/

/*****************************************************************************/
/** Returns the number of decimal places in the argument string, representing
 *  a floating point value. String can contain either decimal dots or commas.
 */
int dec_nr(char *str)
{
  char *cptr;
  int n;

  /* Find the first dot or comma, but stop with E */
  cptr=str;
  while(*cptr!='.' && *cptr!=',') {
    if(*cptr==(char)0 || *cptr=='E' || *cptr=='e') return 0;
    cptr++;
  }
  if(*cptr==(char)0 || (*cptr!='.' && *cptr!=',')) return 0;
  /* Calculate the number of digits that follows */
  cptr++; n=0; while(cptr!=NULL && isdigit(*cptr)) {n++; cptr++;}
  return n;
}
/*****************************************************************************/

/*****************************************************************************/
/** Converts a string to float using strtod(), but if its return
    value is zero this function checks that argument string actually contains
    a number.
\return Returns 0 if successful, and 1 in case of an error.
*/
int atof_with_check(
  /** String which is converted to a double; not modified */
  char *double_as_string,
  /** Pointer to the double float */
  double *result_value
) {
  char* cptr;

  *result_value=atof_dpi(double_as_string);
  if(*result_value!=0.0) return(0);
  cptr=double_as_string;
  while(*cptr!=0 && (*cptr=='+' || *cptr=='-' || *cptr==' ')) cptr++;
  if(*cptr=='0') return(0); else return(1);
}
/*****************************************************************************/

/*****************************************************************************/
/** This function searches the given string for a string representation of
 *  numerical value, possibly with decimal and exponent part.
 *  Returns also pointer to the string right after where the
 *  numerical value ended, and from where the next number can be searched.
Return Returns pointer to start of the next string, or NULL if not found.
 */
char *strPtrToNextValue(
  /** Pointer to string where numerical values are searched; not changed. */
  char *str,
  /** Obligatory pointer to string pointer, which will be set to point to the
   *  first character after value string, or to NULL if string ends. */ 
  char **nxtp
) {
  char *cptr, *strt;
  int i, j, n=0;

  // search the start
  if(strlen(str)<1) {*nxtp=NULL; return NULL;}
  // find the index where number might start
  cptr=str;
  do {
    i=strcspn(cptr, "+-0123456789"); if(i==strlen(cptr)) {*nxtp=NULL; return NULL;}
    strt=cptr+i;
    // any previous character must be a separator
    if(i>0) {cptr=strt-1; i=strcspn(cptr, " \t,;"); if(i>0) cptr+=i;}
  } while(i>0);
  // get past + and - ; if more than one, that is an error
  cptr=strt; if(*cptr=='-' || *cptr=='+') {n++; cptr++;}
  // get past first set of numbers
  i=strspn(cptr, "0123456789"); n+=i;
  if(i<1) {*nxtp=cptr; return NULL;}
  cptr+=i; n+=i;
  // get past decimal separator
  i=strspn(cptr, ",.");
  if(i==0) { // there was none, thus we're done
    *nxtp=cptr; return strt; 
  } else if(i>1) { // can't be more than one decimal separator
    *nxtp=cptr; return strt;
  }
  cptr+=1; n+=1;
  // after decimal separator we must have numbers again
  i=strspn(cptr, "0123456789");
  if(i<1) { // what was thought to be decimal separator was not that
    *nxtp=cptr-1; return strt;
  }
  cptr+=i; n+=i;
  // check if we have exponent part
  if(*cptr=='E' || *cptr=='e') i=1; else i=0;
  // if not, then we're done
  if(i==0) {*nxtp=cptr; return strt;}
  cptr+=1;
  // we may have signed exponent
  if(*cptr=='-' || *cptr=='+') {cptr+=1; i++;}
  // after exponent we must have numbers again
  j=strspn(cptr, "0123456789");
  if(j<1) { // what was thought to be exponent was not that
    *nxtp=cptr-i; return strt;
  }
  cptr+=j; n+=j;
  // now this must be the end
  *nxtp=cptr;
  return strt;
}
#if(0) // Tests
int test_strPtrToNextValue()
{
  char *cptr, *cptr2=NULL, tmp[256];
  int i=0, j=0, n;

  static char *tests[] = {
  "",
  " ",
  " abc d ef",
  " 1  ",
  "M2 s123456 7s",
  "65s, 44.3min m44m b7,7 c1,2,3 43.2",
  "6 s",
  "1 23      4,5 6, 7",
  "-1.2345E-009 0.23 434.1 +65 +2.3E2",
  "-1.2345E-009,0.23,434.1,+65,+2.3E2",
  "-1.2345E-009, 0.23, 434.1, +65, +2.3E2",
  "-1,2345E-009;0,23;434,1;+65;+2,3E2",
  "-1.2345E-009	0.23	434.1	+65	+2.3E2", // tabs
  0};

  while(tests[i]!=0) {
    j=0; printf("Test %d:\n", 1+i); printf("  test_string := '%s'\n", tests[i]);
    cptr=strPtrToNextValue(tests[i], &cptr2);
    while(cptr!=NULL) {
      n=strlen(cptr); if(cptr2!=NULL) n-=strlen(cptr2);
      strncpy(tmp, cptr, n); tmp[n]=(char)0;
      printf("  valstring[%d] := '%s'\n", 1+j, tmp);
      if(cptr2==NULL) break;
      cptr=cptr2; cptr=strPtrToNextValue(cptr, &cptr2);
      j++;
    }
    i++;
  }
  return 0;
}
#endif
/*****************************************************************************/

/*****************************************************************************/
