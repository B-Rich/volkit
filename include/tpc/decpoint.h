/******************************************************************************
  Copyright (c) 2006,2008,2009,2012 Turku PET Centre

  decpoint.h

  Version history:
  2006-06-03 Vesa Oikonen
    First introduced.
  2008-07-11 VO
    Added the externs.
  2008-10-10 VO
    Added dec_nr().
  2009-06-09 VO
    Added atof_with_check().
  2012-03-04 VO
    Added functions dec_separator(), dec_separator_change(), and
    strPtrToNextValue().


******************************************************************************/
#ifndef _DECPOINT_H
#define _DECPOINT_H
/*****************************************************************************/
extern int dec_comma_is(char *str);
extern int dec_separator(char *str);
extern void dec_separator_change(char *str, int decsep);
extern double atof_dpi(char *str);
extern int dec_nr(char *str);
extern int atof_with_check(char *double_as_string, double *result_value);
extern char *strPtrToNextValue(char *str, char **nxtp);
/*****************************************************************************/
#endif

