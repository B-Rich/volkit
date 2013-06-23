/******************************************************************************
  Copyright (c) 2004-2012 by Turku PET Centre

  studynr.h

  Version:
  2004-08-23 Vesa Oikonen
    First introduced.
  2004-09-05 VO
    MAX_REGIONNAME_LEN defined.
    Included four new functions.
  2006-07-25 VO
    Included new functions.
  2008-07-11 VO
    Added the externs.
  2010-04-26 VO
    Included new functions.
  2012-06-08 VO
    Included new function.
  2012-09-28 VO
    Part of contents moved into new rname.h.


******************************************************************************/
#ifndef _STUDYNR_H
#define _STUDYNR_H
/*****************************************************************************/
/** Max length of Study number (+1) */
#ifndef MAX_STUDYNR_LEN
#define MAX_STUDYNR_LEN 10
#endif
/*****************************************************************************/
extern int studynr_in_fname(char *fname, char *studynr);
extern int studynr_from_fname(char *fname, char *studynr);
extern int studynr_from_fname2(char *fname, char *studynr, int force);
extern int studynr_match(char *studynr1, char *studynr2);
extern int studynr_validity_check2(char *studynr, int zero_ok);
extern int studynr_validity_check(char *studynr);
extern int studynr_rm_zeroes(char *studynr);
extern int studynr_to_lowercase(char *studynr);
/*****************************************************************************/
extern int fnmatch(char *key, char *fname);
extern int fncasematch(char *key, char *fname);
/*****************************************************************************/
#endif

