/******************************************************************************
  Copyright (c) 2012 by Turku PET Centre

  rname.h

  Version:
  2012-09-28 Vesa Oikonen
    First created.
    Defined MAX_REGIONSUBNAME_LEN to value 6 which has been hard-coded.


******************************************************************************/
#ifndef _RNAME_H
#define _RNAME_H
/*****************************************************************************/
/** Max length of Region name (+1) */
#ifndef MAX_REGIONNAME_LEN
#define MAX_REGIONNAME_LEN 20
#endif
/** Max length of Region name subfield (+1) */
#ifndef MAX_REGIONSUBNAME_LEN
#define MAX_REGIONSUBNAME_LEN 6
#endif
/*****************************************************************************/
extern int rnameSplit(char *rname, char *name1, char *name2, char *name3,
  int max_name_len);
extern int rnameMatch(char *rname, int rnr, char *test_str);
extern int rnameRmDots(char *rname1, char *rname2);
extern int rnameCatenate(char *rname, int max_rname_len,
  char *name1, char *name2, char *name3, char space);
/*****************************************************************************/
#endif

