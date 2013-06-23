/******************************************************************************

  Copyright (c) 2004,2006,2010,2012 Turku PET Centre

  File: studynr.c

  Description: Functions for processing PET study number.

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
  2004-08-23 Vesa Oikonen
    First introduced.
  2004-09-05 VO
    Included functions fnmatch(), fncasematch(), rnameSplit() and rnameMatch().
  2004-09-20 VO
    Doxygen style comments corrected.
  2006-03-16 VO
    Corrected comment.
  2006-07-26 VO
    Included functions studynr_match(), studynr_validity_check(),
    studynr_rm_zeroes(), and studynr_to_lowercase().
    Function studynr_from_fname() rewritten because of new definitions for
    study numbers.
  2006-10-02 VO
    Bug correction in studynr_validity_check().
  2006-10-12 VO
    Bug correction in studynr_validity_check().
  2010-04-11 VO
    Additional comment.
  2010-04-26 VO
    Added functions studynr_from_fname2(), studynr_validity_check2(),
    and studynr_in_fname().
    Function studynr_from_fname() uses studynr_from_fname2().
    Function studynr_from_fname2() first tries studynr_in_fname() and only if it
    fails, then tries to extract studynr by its own.
  2010-05-18 VO
    Change in code comments.
  2012-03-17 VO
    Bug fix in studynr_in_fname(): did not identify study numer if it was
    followed by other text like 'ap'.
  2012-06-08 VO
    Added function rnameRmDots().
  2012-07-10 VO
    Tiny code formatting changes.
  2012-09-28 VO
    Part of contents moved into new rname.c.


******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*****************************************************************************/
#include "studynr.h"
/*****************************************************************************/
#include "substitutions.h"
/*****************************************************************************/

/*****************************************************************************/
/** Find study number (max MAX_STUDYNR_LEN chars) inside filename;
 *  Study number must contain 1-5 letters followed by 1-5 digits, if
 *  such string is not found then error code is returned.
 *  Initial zeroes are removed if necessary and uppercase letters are 
 *  changed to lowercase.
\return Returns 0 if successful, otherwise <>0.
*/
int studynr_in_fname(
  /** Filename may include path. Filename is not modified. */
  char *fname,
  /** Pointer to string (>=MAX_STUDYNR_LEN+1 chars) where the resulting
      study number is written. */ 
  char *studynr
) {
  int i, ret;
  char *cptr, temp[FILENAME_MAX], temp2[FILENAME_MAX];

  //printf("studynr_in_filename(%s, string)\n", fname);
  if(fname==NULL || studynr==NULL) return(1);
  for(i=0; i<=MAX_STUDYNR_LEN; i++) studynr[i]=(char)0;
  /* Remove path */
  cptr=strrchr(fname, '/'); if(cptr==NULL) cptr=strrchr(fname, '\\');
  if(cptr==NULL) cptr=fname; else cptr++;
  i=strlen(cptr); if(i>FILENAME_MAX) i=FILENAME_MAX;
  strncpy(temp, cptr, i);
  /* Verify tokens in filename whether they are valid as study number */
  cptr=strtok(temp, "_-+{}!~.()");
  while(cptr!=NULL && !studynr[0]) {
    strcpy(temp2, cptr);
    /* Remove everything after the letter+digit parts */
    for(i=1; i<strlen(temp2); i++) if(!isalpha((int)temp2[i])) break;
    for(; i<strlen(temp2); i++) if(!isdigit((int)temp2[i])) break;
    temp2[i]=(char)0;
    /* Only then check it */
    ret=studynr_validity_check2(temp2, 1);
    if(ret==1) {
      strcpy(studynr, temp2); 
      if(studynr_rm_zeroes(studynr)!=0) strcpy(studynr, "");
      if(studynr_to_lowercase(studynr)!=0) strcpy(studynr, "");
    }
    cptr=strtok(NULL, "_-+{}!~.()");
  }
  if(!studynr[0]) return(2);
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Extract study number (max MAX_STUDYNR_LEN chars) from filename.
 *  This function removes initial zeroes from the number part, and
 *  converts uppercase letters to lowercase, if necessary.  
 *
\return Returns 0 if successful.
 */
int studynr_from_fname2(
  /** Filename, which may include the path. Filename is not modified. */
  char *fname,
  /** Pointer for the resulting study number. Memory (>=MAX_STUDYNR_LEN+1 chars)
   *  must be allocated before calling this. */
  char *studynr,
  /** Validity of studynr is verified (0) or not verified (1) */
  int force
) {
  int i;
  char *cptr;

  if(fname==NULL || studynr==NULL) return(1);
  for(i=0; i<=MAX_STUDYNR_LEN; i++) studynr[i]=(char)0;

  /* At first try if studynr_in_fname() works */
  if(studynr_in_fname(fname, studynr)==0) return(0);

  /* Remove path */
  cptr=strrchr(fname, '/'); if(cptr==NULL) cptr=strrchr(fname, '\\');
  if(cptr==NULL) cptr=fname; else cptr++;
  i=strlen(cptr); if(i>MAX_STUDYNR_LEN) i=MAX_STUDYNR_LEN;
  strncpy(studynr, cptr, i);
  if(force!=0) return(0);

  /* Check that first character is a letter */
  if(!isalpha((int)studynr[0])) {strcpy(studynr, ""); return(4);}
  /* Remove everything after the letter+digit parts */
  for(i=1; i<strlen(studynr); i++) if(!isalpha((int)studynr[i])) break;
  for(; i<strlen(studynr); i++) if(!isdigit((int)studynr[i])) break;
  studynr[i]=(char)0;
  /* Check the length of the study number */
  if(strlen(studynr)<2) {strcpy(studynr, ""); return(5);}
  /* Remove initial zeroes from the number part */
  if(studynr_rm_zeroes(studynr)!=0) {strcpy(studynr, ""); return(6);}
  /* Convert characters to lower case */
  if(studynr_to_lowercase(studynr)!=0) {strcpy(studynr, ""); return(7);}
  /* Check the validity of the study number */
  if(!studynr_validity_check(studynr)) {strcpy(studynr, ""); return(8);}
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Extract study number (max MAX_STUDYNR_LEN chars) from filename.
 *   This function removes initial zeroes from the number part, and
 *   converts uppercase letters to lowercase, if necessary.
 *
 * @param fname Filename may include path. Filename is not modified.
 * @param studynr Pointer for the resulting study number. Memory (>=MAX_STUDYNR_LEN+1 chars)
 * for it must be allocated before calling this.
 * @return 0 if successful.
 */
int studynr_from_fname(char *fname, char *studynr) {
  return studynr_from_fname2(fname, studynr, 0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Check if string fname matches string key, which may contain wildcards
 * ? and *.
 *
 * @param key key string
 * @param fname filename that is evaluated
 * @return 1 if strings do match and 0 if not.
 */
int fnmatch(char *key, char *fname) {
  char *key_ptr=NULL, *fname_ptr=NULL;

  while((*key)&&(*fname)) {
    if((*key=='?')||(*key==*fname)) {
      key++; fname++;
    } else if(*key=='*') {
      if(*(key+1)==*fname) {key_ptr=key++; fname_ptr=fname+1;}
      else {fname++; if(*(key+1)=='?') {key_ptr=key++; fname_ptr=fname;}}
    } else if((key_ptr!=NULL) && (*fname_ptr)) {
      return(fnmatch(key_ptr, fname_ptr));
    } else {
      return(0);
    }
  }
  if((*fname)&&(key_ptr!=NULL)) {return(fnmatch(key_ptr, fname_ptr));}
  else {if(*key=='*') key++; return(*key==*fname);}
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Case-independent check whether string fname matches string key,
 *   which may contain wildcards ? and *.
 *
 * @param key key string
 * @param fname filename that is evaluated
 * @return 1 if strings do match and 0 if not.
 */
int fncasematch(char *key, char *fname) {
  char *key_ptr=NULL, *fname_ptr=NULL;

  while((*key)&&(*fname)) {
    if((*key=='?')||(toupper((int)*key)==toupper((int)*fname))) {
      key++; fname++;
    } else if(*key=='*') {
      if(toupper((int)*(key+1))==toupper((int)*fname)) {key_ptr=key++; fname_ptr=fname+1;}
      else {fname++; if(*(key+1)=='?') {key_ptr=key++; fname_ptr=fname;}}
    } else if((key_ptr!=NULL) && (*fname_ptr)) {
      return(fnmatch(key_ptr, fname_ptr));
    } else {
      return(0);
    }
  }
  if((*fname)&&(key_ptr!=NULL)) {return(fnmatch(key_ptr, fname_ptr));}
  else {if(*key=='*') key++; return(toupper((int)*key)==toupper((int)*fname));}
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Check whether two valid study numbers are the same.
 *   If either of study numbers is shorter than the other, the end parts are
 *   compared; thus study numbers that are changed by SPM can be matched.
 *   Argument strings are not modified.
 *
 * @param studynr1 compared number
 * @param studynr2 compared number
 * @return 1 if study numbers match exactly, 2 if the match is probable,
 * and zero, if no match is found.
 */
int studynr_match(char *studynr1, char *studynr2) {
  int len1, len2;
  char *cptr1, *cptr2;

  len1=strlen(studynr1); if(len1<2 || len1>MAX_STUDYNR_LEN) return(0);
  len2=strlen(studynr2); if(len2<2 || len2>MAX_STUDYNR_LEN) return(0);
  if(len1==len2 && strcmp(studynr1, studynr2)==0) return(1);
  if(len2>len1) cptr2=studynr2+len2-len1; else cptr2=studynr2;
  if(len1>len2) cptr1=studynr1+len1-len2; else cptr1=studynr1;
  if(strcasecmp(cptr1, cptr2)==0) return(2);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Check that the argument string is a valid TPC study number containing
 *  1-5 letters (upper- or lowercase) followed by 1-5 digits.
 *  Argument string is not modified.
 *
 * @return 1 if study number is valid, zero if not valid or in case of
 * failure.
 */
int studynr_validity_check2(
  /** String to be evaluated as study number. Not modified here. */
  char *studynr,
  /** Number part of study number may start with zero (1) or may not (0) */
  int zero_ok
) {
  int i, j, len;

  len=strlen(studynr); if(len<2 || len>MAX_STUDYNR_LEN) return(0);
  for(i=0; i<len; i++) if(!isalnum((int)studynr[i])) return(0);
  for(i=0; i<len; i++) if(!isalpha((int)studynr[i])) break;
  if(i<1 || i>5) return(0);
  if(zero_ok==0 && studynr[i]=='0') return(0); /* first digit must be >0 */
  for(j=0; (i+j)<len; j++) if(!isdigit((int)studynr[i+j])) return(0);
  if(j<1 || j>5) return(0);
  return(1);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Check that the argument string is a valid TPC study number containing
 *   1-5 letters (upper- or lowercase) followed by 1-5 digits.
 *   Argument string is not modified.
 *
 * @param studynr evaluated study number
 * @return 1 if study number is valid, zero if not valid or in case of
 * failure.
 */
int studynr_validity_check(char *studynr) {
  return studynr_validity_check2(studynr, 0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Remove zeroes from the number part of the PET study number.
 *
 * @param studynr modified study number
 * @return nonzero in case of failure.
 */
int studynr_rm_zeroes(char *studynr) {
  int i, j, len;

  len=strlen(studynr); if(len<2) return(1);
  if(isdigit((int)studynr[0])) return(2);
  if(!isdigit((int)studynr[len-1])) return(3);
  for(i=1; i<len; i++) if(isdigit((int)studynr[i])) break;
  for(j=i; j<len; j++) if(studynr[j]!='0') break;
  if(i==j) return(0); /* no initial zeroes */
  for(;j<=len; i++, j++) studynr[i]=studynr[j];
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Convert the PET study number letters to lowercase.
 * Conversion is not done to non-valid study number.
 *
 * @param studynr modified study number
 * @return nonzero in case of failure.
 */
int studynr_to_lowercase(char *studynr) {
  int i, len;

  len=strlen(studynr); if(len<2) return(1);
  if(isdigit((int)studynr[0])) return(2);
  if(!isdigit((int)studynr[len-1])) return(3);
  for(i=0; i<len; i++) {
    if(isdigit((int)studynr[i])) break;
    studynr[i]=(char)tolower((int)studynr[i]);
  }
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
