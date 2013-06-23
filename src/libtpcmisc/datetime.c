/******************************************************************************

  Copyright (c) 2010 Turku PET Centre

  File: datetime.c
  Description: Functions for processing date and time.

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
  2010-05-18 Vesa Oikonen
    First created.
  2010-05-19 Vesa Oikonen
    Additional functions.
  2010-08-19 Vesa Oikonen
    Additional functions.
  2010-12-14 VO
    Fixed a bug in get_datetime() with non-international date formats.
    Added function get_date().


******************************************************************************/
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/*****************************************************************************/
#include "datetime.h"
/*****************************************************************************/
#include "substitutions.h"
/*****************************************************************************/
/* How many days come before each month (0-12). */
static const unsigned short __mon_yday[2][13] = {
    /* Normal years. */
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    /* Leap years. */
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified string contains date in correct international format
 *  (YYYY-MM-DD). String must start with date, but any contents after
 *  it is ignored.
\return Returns 0 if date is in correct format, -1 if format is correct but
        date is invalid, and otherwise <>0.
 */
int isdate(
  /** String to be checked; not changed in this routine */
  char *str
) {
  int Y, M, D, n;
  if(strlen(str)<10) return 1;
  if(str[4]!='-' || str[7]!='-') return 2;
  if(strncasecmp(str, "YYYY-MM-DD", 10)==0) return -1;
  n=sscanf(str, "%4d-%2d-%2d", &Y, &M, &D); if(n!=3) return 3;
  if(M>12 || D>31) return -1;
  if(Y<0 || M<1 || D<1) return -1;
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified string contains date in correct format
 *  (DD.MM.YYYY or DD/MM/YYYY). String must start with date, but any contents
 *  after it is ignored.
\return Returns 0 if date is in correct format, -1 if format is correct but
        date is invalid, and otherwise <>0.
 */
int isdate2(
  /** String to be checked; not changed in this routine */
  char *str,
  /** Pointer to allocated string where date is written in international format;
   *  enter NULL, if not needed. */
  char *intdate
) {
  int Y, M, D, n;
  if(strlen(str)<10) return 1;
  if(str[2]!='.' && str[2]!='/') return 2;
  if(str[5]!='.' && str[5]!='/') return 2;
  if(strncasecmp(str, "DD.MM.YYYY", 10)==0 ||
     strncasecmp(str, "DD/MM/YYYY", 10)==0) return -1;
  n=sscanf(str, "%2d/%2d/%4d", &D, &M, &Y);
  if(n<3) n=sscanf(str, "%2d.%2d.%4d", &D, &M, &Y);
  if(n!=3) return 3;
  if(M>12 || D>31) return -1;
  if(Y<0 || M<1 || D<1) return -1;
  if(intdate!=NULL) sprintf(intdate, "%04d-%02d-%02d", Y, M, D);
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified string contains date in correct format
 *  (DD.MM.YY or DD/MM/YY). String must start with date, but any contents
 *  after it is ignored.
\return Returns 0 if date is in correct format, -1 if format is correct but
        date is invalid, and otherwise <>0.
 */
int isdate3(
  /** String to be checked; not changed in this routine */
  char *str,
  /** Pointer to allocated string where date is written in international format;
   *  enter NULL, if not needed. */
  char *intdate
) {
  int Y, M, D, n;
  if(strlen(str)<8) return 1;
  if(str[2]!='.' && str[2]!='/') return 2;
  if(str[5]!='.' && str[5]!='/') return 2;
  if(strncasecmp(str, "DD.MM.YY", 8)==0 ||
     strncasecmp(str, "DD/MM/YY", 8)==0) return -1;
  n=sscanf(str, "%2d/%2d/%2d", &D, &M, &Y);
  if(n<3) n=sscanf(str, "%2d.%2d.%2d", &D, &M, &Y);
  if(n!=3) return 3;
  if(Y>99 || M>12 || D>31) return -1;
  if(Y<0 || M<1 || D<1) return -1;
  if(intdate!=NULL) {
    if(Y>=70) Y+=1900; else Y+=2000;
    sprintf(intdate, "%04d-%02d-%02d", Y, M, D);
  }
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified integer contains date in format YYYYMMDD.
\return Returns 0 if date is in correct format, -1 if format is correct but
        date is invalid, and otherwise <>0.
 */
int isdate4(
  /** Integer to be checked; not changed in this routine */
  int dateint,
  /** Year is written in this pointer; enter NULL if not needed */
  int *year,
  /** Month is written in this pointer; enter NULL if not needed */
  int *month,
  /** Day is written in this pointer; enter NULL if not needed */
  int *day
) {
  int Y, M, D, n;
  if(dateint<1201 || dateint>99991231) return 1;
  n=dateint/100; D=dateint-100*n; Y=n/100; M=n-100*Y;
  if(M>12 || D>31) return -1;
  if(Y<1 || M<1 || D<1) return -1;
  if(year!=NULL) *year=Y;
  if(month!=NULL) *month=M;
  if(day!=NULL) *day=D;
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified string contains time in correct format
 *  (hh:mm:ss). String must start with time, but any contents after
 *  it is ignored.
\return Returns 0 if time is in correct format, -1 if format is correct but
        time is invalid, and otherwise <>0.
 */
int istime(
  /** String to be checked; not changed in this routine */
  char *str
) {
  int h, m, s, n;
  if(strlen(str)<8) return 1;
  if(str[2]!=':' || str[5]!=':') return 2;
  if(strncasecmp(str, "hh:mm:ss", 8)==0) return -1;
  n=sscanf(str, "%d:%d:%d", &h, &m, &s); if(n!=3) return 3;
  if(h<0 || h>23 || m<0 || m>59 || s<0 || s>59) return -1;
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified string contains date and time in correct format
 *  (YYYY-MM-DD hh:mm:ss, DD.MM.YYYY hh:mm:ss, or DD.MM.YY hh:mm:ss).
 *  String must start with date, but any contents after time is ignored.
\return Returns 0 if date and time are in correct format, <0 if format is
        correct but date or time is invalid, and otherwise <>0.
 */
int isdatetime(
  /** String to be checked; not changed in this routine */
  char *str,
  /** Pointer to allocated string where date and time is written in
   *  international format (YYYY-MM-DD hh:mm:ss);
   *  enter NULL, if not needed. */
  char *intdate
) {
  int ret1, ret2, len, type=0;
  char correct_date[20], *time_ptr=NULL;

  len=strlen(str); if(len<17) return 1;
  if((ret1=isdate(str))<=0) {
    type=1; strncpy(correct_date, str, 10); correct_date[10]=(char)0; 
  } else if((ret1=isdate2(str, correct_date))<=0) type=2;
  else if((ret1=isdate3(str, correct_date))<=0) type=3;
  else {return 2;}
  if(type==1 || type==2) {
    if(str[10]!=' ' && str[10]!='\t') return 3;
    time_ptr=str+11;
  } else if(type==3) {
    if(str[8]!=' ' && str[8]!='\t') return 3;
    time_ptr=str+9;
  }
  ret2=istime(time_ptr);
  if(ret1>0) {
    if(ret2>=0) return(100*ret1+ret2); else return(100*ret2);
  }
  if(ret2>0) return(10*ret2);
  if(ret1<0) {if(ret2<0) return(-3); else return(-1);}
  if(ret2<0) return(-2);
  if(intdate!=NULL) {
    sprintf(intdate, "%10.10s %8.8s", correct_date, time_ptr);
  }
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Reads time and date from a standard string representation of date and time.
\return Returns 0 when successful, <>0 in case of an error.
 */
int get_datetime(
  /** Pointer to string that contains date and time in one of the formats
   *  YYYY-MM-DD hh:mm:ss, DD.MM.YYYY hh:mm:ss, or DD/MM/YY hh:mm:ss
   *  This string is not modified. */  
  char *str,
  /** Pointer to allocated struct tm where date and time is written */
  struct tm *date
) {
  char buf[32];
  int ret, n, YYYY=0, MM=0, DD=0, hh=0, mm=0, ss=0;

  ret=isdatetime(str, buf); if(ret!=0) return(ret);
  n=sscanf(buf, "%d-%d-%d %d:%d:%d", &YYYY, &MM, &DD, &hh, &mm, &ss);
  if(n!=6) return(40);
  date->tm_year=YYYY-1900; date->tm_mday=DD; date->tm_mon=MM-1;
  date->tm_hour=hh; date->tm_min=mm; date->tm_sec=ss; date->tm_isdst=-1;
  ret=strftime(buf, 32, "%Y-%m-%d %H:%M:%S", date);
  if(ret==0) return(50);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Reads date from a standard string representation of date.
\return Returns 0 when successful, <>0 in case of an error.
 */
int get_date(
  /** Pointer to string that contains date in one of the formats
   *  YYYY-MM-DD, DD/MM/YYYY, DD.MM.YYYY, DD/MM/YY, or DD.MM.YYYY.
   *  This string is not modified. */  
  char *str,
  /** Pointer to allocated struct tm where the date is written */
  struct tm *date
) {
  char buf[32];
  int ret, n, YYYY=0, MM=0, DD=0;

  if(strlen(str)<8) return 1;
  if((ret=isdate(str))<=0) {
    strncpy(buf, str, 10); buf[10]=(char)0; 
  } else {
    ret=isdate2(str, buf);
    if(ret>0) ret=isdate3(str, buf);
  }
  if(ret>0) return 2;
  n=sscanf(buf, "%d-%d-%d", &YYYY, &MM, &DD);
  if(n!=3) return(3);
  date->tm_year=YYYY-1900; date->tm_mday=DD; date->tm_mon=MM-1;
  date->tm_hour=0; date->tm_min=0; date->tm_sec=0; date->tm_isdst=-1;
  ret=strftime(buf, 32, "%Y-%m-%d %H:%M:%S", date);
  if(ret==0) return(4);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/* Division for long integers */
long int math_div(long int a, long int b)
{
  return a/b - (a%b < 0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Return nonzero if 'year' is a leap year.
 */
int isleapyear(long year)
{
  return (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Calculates the number of leap years between year1 and year2.
 */
long int leaps_between(long int year1, long int year2)
{
  long int leaps1, leaps2;

  leaps1 = math_div(year1-1, 4) - math_div(year1-1, 100) + math_div(year1-1, 400);
  leaps2 = math_div(year2-1, 4) - math_div(year2-1, 100) + math_div(year2-1, 400);
  return(leaps2-leaps1);
}
/*****************************************************************************/

/*****************************************************************************/
/** Convert calendar time to local broken-down time.
 *  This function is copied from GNU C Library with tiny modifications. */
void time_to_tm(
  /** number of seconds elapsed since 00:00:00 on January 1, 1970, UTC;
   *  can be negative to represent times before 1970 */
  time_t totalsecs,
  /** offset seconds adding to totalsecs (e.g. -timezone) */
  int offset,
  /** pointer to struct tm variable to receive broken-down time */
  struct tm *result
) {
  long int days, rem, y, yg;
  const unsigned short *ip;
 
  days=totalsecs/86400; rem=totalsecs%86400; rem+=offset;
  while(rem<0) {rem+=86400; --days;}
  while(rem>=86400) {rem-=86400; ++days;}
  result->tm_hour=rem/3600;
  rem%=3600; result->tm_min=rem/60;
  result->tm_sec=rem%60;

  /* January 1, 1970 was a Thursday. */
  result->tm_wday=(4+days)%7;
  if(result->tm_wday<0) result->tm_wday+=7;
  y=1970;
  while(days<0 || days>=(isleapyear(y)?366:365)) {
    /* Guess a corrected year, assuming 365 days per year. */
    yg=y+math_div(days, 365);
    /* Adjust days and y to match the guessed year. */
    days-=(yg-y)*365 + leaps_between(y, yg);
    y=yg;
  }
  result->tm_year=y-1900; result->tm_yday=days;
  ip=__mon_yday[isleapyear(y)];
  for(y=11; days<ip[y]; y--) continue;
  days-=ip[y];
  result->tm_mon=y;
  result->tm_mday=days+1;
}
/*****************************************************************************/

/*****************************************************************************/

