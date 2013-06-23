/******************************************************************************

  Copyright (c) 2006,2013 Turku PET Centre

  File: quots.c
  Description: Functions for processing strings with quotation marks.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details:
  http://www.gnu.org/copyleft/lesser.html

  You should have received a copy of the GNU Lesser General Public License
  along with this library/program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

  Turku PET Centre hereby disclaims all copyright interest in the program.
  Juhani Knuuti
  Director, Professor
  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi/

  Modification history:
  2006-10-23 Vesa Oikonen
    First created.
  2013-06-17 VO
    Added function strnCopyClean() from dft2svg.
    

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*****************************************************************************/
#include "quots.h"
/*****************************************************************************/

/*****************************************************************************/
/** The strstr_noquotation() function returns a pointer to the first occurrence
 *  in the string pointed to by str1, excluding parts that are inside quotation
 *  marks "" or '', of the string pointed to by str2.
 *
\return Returns NULL pointer if no match is found, or if found, then pointer
    to the first occurrence.
 */ 
char *strstr_noquotation(
  /** Pointer to string to be searched */
  const char *str1,
  /** Pointer to string with quotation marks */
  const char *str2
) {
  int i, test_len;
  int single_quotation=0;
  int double_quotation=0;
  char *cptr;
  
  if(str1==NULL) return((char*)NULL);
  if(str2==NULL) return((char*)str1);
  test_len=strlen(str2); if(test_len<1) return((char*)str1);
  for(i=0, cptr=(char*)str1; i<strlen(str1); i++, cptr++) {
    if(*cptr=='\'') {
      if(single_quotation==0) single_quotation=1; else single_quotation=0;
      continue;
    }
    if(*cptr=='\"') {
      if(double_quotation==0) double_quotation=1; else double_quotation=0;
      continue;
    }
    if(single_quotation==1 || double_quotation==1) continue;
    if(strncmp(cptr, str2, test_len)==0) return(cptr);
  }
  return((char*)NULL);
}
/*****************************************************************************/

/*****************************************************************************/
/** Copy str1 to str1, removing any quotation marks around the string,
 *  and making sure that string fits to str2.
\return Returns the length of the new string str2.
 */
int strnCopyClean(
  /** Pointer to pre-allocated result string with length of at least maxlen
   *  characters, including NULL character */
  char *str1,
  /** Pointer to the original string; not changed in this function */
  const char *str2,
  /** Max length of str1, inclusing the end NULL */
  int maxlen
) {
  char *cptr;
  int i;

  if(str1==NULL || maxlen<1) return(0);
  str1[0]=(char)0; if(str2==NULL || strlen(str2)<1) return(0);
  cptr=(char*)str2; cptr+=strspn(str2, "\"\'\t\n\r ");
  strncpy(str1, cptr, maxlen); str1[maxlen]=(char)0;
  i=strlen(str1); if(i<1) return(0);
  cptr=str1+(i-1);
  while(i>0) {
    if(*cptr!='\"' && *cptr!='\'' && *cptr!='\t' && *cptr!='\n' &&
       *cptr!='\r' && *cptr!=' ')
           break;
    i--; str1[i]=(char)0; cptr--;
  }
  return(i);
}
/*****************************************************************************/

/*****************************************************************************/

