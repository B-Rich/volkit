/******************************************************************************
  Copyright (c) 2010 by Turku PET Centre

  datetime.h

  Version:
  2010-05-19 Vesa Oikonen
    First created.
  2010-08-19 Vesa Oikonen
    Additional functions.
  2010-12-14 VO
    Added get_date().


******************************************************************************/
#ifndef _DATETIME_H
#define _DATETIME_H
/*****************************************************************************/
#include <time.h>
/*****************************************************************************/
extern int isdate(char *str);
extern int isdate2(char *str, char *intdate);
extern int isdate3(char *str, char *intdate);
extern int isdate4(int dateint, int *year, int *month, int *day);
extern int istime(char *str);
extern int isdatetime(char *str, char *intdate);
extern int get_datetime(char *str, struct tm *date);
extern int get_date(char *str, struct tm *date);
extern long int math_div(long int a, long int b);
extern int isleapyear(long int year);
extern long int leaps_between(long int year1, long int year2);
extern void time_to_tm(time_t totalsecs, int offset, struct tm *result);
/*****************************************************************************/
#endif

