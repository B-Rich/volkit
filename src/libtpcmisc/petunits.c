/*****************************************************************************

  Copyright (c) 2007,2010-2013 Turku PET Centre

  File:        units.c
  Description: Check and set units of PET data.

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 3 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License along
  with this library; if not, write to the Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

  Turku PET Centre hereby disclaims all copyright interest in the library
  written by Vesa Oikonen.

  2007-08-10 Juhani Knuuti
  Director, Professor
  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi/

  Modification history:
  2007-08-10 Vesa Oikonen
    First created.
  2010-07-01 VO
    Added function cunitFromFilename().
  2011-07-01 VO
    Added units Bq, kBq, MBq, GBq, nCi, uCi, and mCi, to
    enable working with injected doses.
  2012-02-24 VO
    Added units %ID, %ID/g and %ID/mL.
  2013-06-17 VO
    Added units CUNIT_G_PER_ML and CUNIT_ML_PER_G


******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
/*****************************************************************************/
#include "petunits.h"
#include "substitutions.h"
/*****************************************************************************/

/*****************************************************************************/
/** Valid PET data calibration (y axis) units.
 *  Max MAX_UNITS_LEN characters (Currently 31).
 *  Try to use these with IMG and DFT structs */
static char *cunit_string[] = {
  /* CUNIT_UNKNOWN               */  "unknown",
  /* CUNIT_CPS                   */  "cnts/sec",
  /* CUNIT_COUNTS                */  "counts",
  /* CUNIT_KBQ_PER_ML            */  "kBq/mL",
  /* CUNIT_SEC_KBQ_PER_ML        */  "sec*kBq/mL",
  /* CUNIT_PER_SEC               */  "1/sec",
  /* CUNIT_PER_MIN               */  "1/min",
  /* CUNIT_ML_PER_ML             */  "mL/mL",
  /* CUNIT_ML_PER_DL             */  "mL/dL",
  /* CUNIT_ML_PER_ML_PER_MIN     */  "mL/(mL*min)",
  /* CUNIT_ML_PER_DL_PER_MIN     */  "mL/(dL*min)",
  /* CUNIT_UNITLESS              */  "unitless",
  /* CUNIT_NCI_PER_ML            */  "nCi/mL",
  /* CUNIT_MBQ_PER_ML            */  "MBq/mL",
  /* CUNIT_BQ_PER_ML             */  "Bq/cc",
  /* CUNIT_UCI_PER_ML            */  "uCi/cc",
  /* CUNIT_UMOL_PER_MIN_PER_100G */  "umol/(100g*min)",
  /* CUNIT_MG_PER_MIN_PER_100G   */  "mg/(100g*min)",
  /* CUNIT_UMOL_PER_MIN_PER_DL   */  "umol/(dL*min)",
  /* CUNIT_MG_PER_MIN_PER_DL     */  "mg/(dL*min)",
  /* CUNIT_PERCENTAGE            */  "%",
  /* CUNIT_KCPS                  */  "kcps",
  /* CUNIT_MIN_KBQ_PER_ML        */  "min*kBq/mL",
  /* CUNIT_BQ                    */  "Bq",
  /* CUNIT_kBQ                   */  "kBq",
  /* CUNIT_MBQ                   */  "MBq",
  /* CUNIT_GBQ                   */  "GBq",
  /* CUNIT_NCI                   */  "nCi",
  /* CUNIT_uCI                   */  "uCi",
  /* CUNIT_mCI                   */  "mCi",
  /* CUNIT_PID                   */  "%ID",
  /* CUNIT_PIDM                  */  "%ID/g",
  /* CUNIT_PIDV                  */  "%ID/mL",
  /* CUNIT_G_PER_ML              */  "g/mL",
  /* CUNIT_ML_PER_G              */  "mL/g",
  0
};
/** Valid PET time (x axis) units. */
static char *tunit_string[] = {
  /*  TUNIT_UNKNOWN */  "unknown",
  /*  TUNIT_SEC */      "sec",
  /*  TUNIT_MIN */      "min",
  /*  TUNIT_UM */       "um",
  /*  TUNIT_MM */       "mm",
  /*  TUNIT_CM */       "cm",
  /*  TUNIT_M */        "m",
  /*  TUNIT_HOUR */     "h",
  /*  TUNIT_MONTH */    "months",
  /*  TUNIT_YEAR */     "y",
  /*  TUNIT_MSEC */     "msec",
  0
};
/*****************************************************************************/

/*****************************************************************************/
/** Identify the specified units string as PET data unit.
\return Returns the unit id number. 
 */
int petCunitId(const char *unit)
{
  if(unit==NULL) return CUNIT_UNKNOWN;
  if(strlen(unit)==0)                             return CUNIT_UNKNOWN;
  else if(strcasecmp(unit, "unknown")==0)         return CUNIT_UNKNOWN;
  else if(strcasecmp(unit, "cnts/sec")==0)        return CUNIT_CPS;
  else if(strcasecmp(unit, "counts/sec")==0)      return CUNIT_CPS;
  else if(strcasecmp(unit, "ECAT counts/sec")==0) return CUNIT_CPS;
  else if(strcasecmp(unit, "cps")==0)             return CUNIT_CPS;
  else if(strcasecmp(unit, "counts")==0)          return CUNIT_COUNTS;
  else if(strcasecmp(unit, "cnts")==0)            return CUNIT_COUNTS;
  else if(strcasecmp(unit, "kBq/cc")==0)          return CUNIT_KBQ_PER_ML;
  else if(strcasecmp(unit, "kBq/mL")==0)          return CUNIT_KBQ_PER_ML;
  else if(strcasecmp(unit, "sec*kBq/cc")==0)      return CUNIT_SEC_KBQ_PER_ML;
  else if(strcasecmp(unit, "sec*kBq/mL")==0)      return CUNIT_SEC_KBQ_PER_ML;
  else if(strcasecmp(unit, "integral")==0)        return CUNIT_SEC_KBQ_PER_ML;
  else if(strcasecmp(unit, "1/sec")==0)           return CUNIT_PER_SEC;
  else if(strcasecmp(unit, "1/s")==0)             return CUNIT_PER_SEC;
  else if(strcasecmp(unit, "s-1")==0)             return CUNIT_PER_SEC;
  else if(strcasecmp(unit, "1/min")==0)           return CUNIT_PER_MIN;
  else if(strcasecmp(unit, "min-1")==0)           return CUNIT_PER_MIN;
  else if(strcasecmp(unit, "mL/mL")==0)           return CUNIT_ML_PER_ML;
  else if(strcasecmp(unit, "mL/cc")==0)           return CUNIT_ML_PER_ML;
  else if(strcasecmp(unit, "mL/dL")==0)           return CUNIT_ML_PER_DL;
  else if(strcasecmp(unit, "mL/100mL")==0)        return CUNIT_ML_PER_DL;
  else if(strcasecmp(unit, "mL/(mL*min)")==0)     return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*mL)")==0)     return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(cc*min)")==0)     return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*cc)")==0)     return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/mL/min")==0)       return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/min/mL")==0)       return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/cc/min")==0)       return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/min/cc")==0)       return CUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(dL*min)")==0)     return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*dL)")==0)     return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/(100mL*min)")==0)  return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*100mL)")==0)  return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/dL/min")==0)       return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/min/dL")==0)       return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/100mL/min")==0)    return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/min/100mL")==0)    return CUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "unitless")==0)        return CUNIT_UNITLESS;
  else if(strcasecmp(unit, "nCi/cc")==0)          return CUNIT_NCI_PER_ML;
  else if(strcasecmp(unit, "nCi/mL")==0)          return CUNIT_NCI_PER_ML;
  else if(strcasecmp(unit, "MBq/cc")==0)          return CUNIT_MBQ_PER_ML;
  else if(strcasecmp(unit, "MBq/mL")==0)          return CUNIT_MBQ_PER_ML;
  else if(strcasecmp(unit, "Bq/cc")==0)           return CUNIT_BQ_PER_ML;
  else if(strcasecmp(unit, "Bq/mL")==0)           return CUNIT_BQ_PER_ML;
  else if(strcasecmp(unit, "uCi/cc")==0)          return CUNIT_UCI_PER_ML;
  else if(strcasecmp(unit, "uCi/mL")==0)          return CUNIT_UCI_PER_ML;
  else if(strcasecmp(unit, "umol/(100g*min)")==0) return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/(min*100g)")==0) return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/100g/min")==0)   return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/min/100g")==0)   return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/(dL*min)")==0)   return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/(min*dL)")==0)   return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/dL/min")==0)     return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/min/dL")==0)     return CUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/(100g*min)")==0)   return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/(min*100g)")==0)   return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/100g/min")==0)     return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/min/100g")==0)     return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/(dL*min)")==0)     return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/(min*dL)")==0)     return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/dL/min")==0)       return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/min/dL")==0)       return CUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "%")==0)               return CUNIT_PERCENTAGE;
  else if(strcasecmp(unit, "kcps")==0)            return CUNIT_KCPS;
  else if(strcasecmp(unit, "min*kBq/cc")==0)      return CUNIT_MIN_KBQ_PER_ML;
  else if(strcasecmp(unit, "min*kBq/mL")==0)      return CUNIT_MIN_KBQ_PER_ML;
  else if(strcasecmp(unit, "Bq")==0)              return CUNIT_BQ;
  else if(strcasecmp(unit, "kBq")==0)             return CUNIT_KBQ;
  else if(strcasecmp(unit, "MBq")==0)             return CUNIT_MBQ;
  else if(strcasecmp(unit, "GBq")==0)             return CUNIT_GBQ;
  else if(strcasecmp(unit, "nCi")==0)             return CUNIT_NCI;
  else if(strcasecmp(unit, "uCi")==0)             return CUNIT_UCI;
  else if(strcasecmp(unit, "mCi")==0)             return CUNIT_MCI;
  else if(strcasecmp(unit, "%ID")==0)             return CUNIT_PID;
  else if(strcasecmp(unit, "% ID")==0)            return CUNIT_PID;
  else if(strcasecmp(unit, "%ID/g")==0)           return CUNIT_PIDM;
  else if(strcasecmp(unit, "% ID/g")==0)          return CUNIT_PIDM;
  else if(strcasecmp(unit, "%ID/mL")==0)          return CUNIT_PIDV;
  else if(strcasecmp(unit, "% ID/mL")==0)         return CUNIT_PIDV;
  else if(strcasecmp(unit, "%ID/cc")==0)          return CUNIT_PIDV;
  else if(strcasecmp(unit, "% ID/cc")==0)         return CUNIT_PIDV;
  else if(strcasecmp(unit, "g/mL")==0)            return CUNIT_G_PER_ML;
  else if(strcasecmp(unit, "g/cc")==0)            return CUNIT_G_PER_ML;
  else if(strcasecmp(unit, "mL/g")==0)            return CUNIT_ML_PER_G;
  else if(strcasecmp(unit, "cc/g")==0)            return CUNIT_ML_PER_G;

  return CUNIT_UNKNOWN;
}
/*****************************************************************************/

/*****************************************************************************/
/** Identifies the specified string as PET time (x axis) units.
\return Returns the timeunit id number.
 */
int petTunitId(const char *timeunit)
{
  if(timeunit==NULL) return TUNIT_UNKNOWN;
  if(strlen(timeunit)==0)                         return TUNIT_UNKNOWN;
  else if(strcasecmp(timeunit,  "unknown")==0)    return TUNIT_UNKNOWN;
  else if(strncasecmp(timeunit, "seconds", 3)==0) return TUNIT_SEC;
  else if(strcmp(timeunit,      "s")==0)          return TUNIT_SEC;
  else if(strncasecmp(timeunit, "minutes", 3)==0) return TUNIT_MIN;
  else if(strcasecmp(timeunit,  "um")==0)         return TUNIT_UM;
  else if(strcasecmp(timeunit,  "mm")==0)         return TUNIT_MM;
  else if(strcasecmp(timeunit,  "cm")==0)         return TUNIT_CM;
  else if(strcasecmp(timeunit,  "m")==0)          return TUNIT_M;
  else if(strcasecmp(timeunit,  "h")==0)          return TUNIT_HOUR;
  else if(strcasecmp(timeunit,  "months")==0)     return TUNIT_MONTH;
  else if(strcasecmp(timeunit,  "y")==0)          return TUNIT_YEAR;
  else if(strcasecmp(timeunit,  "msec")==0)       return TUNIT_MSEC;
  return TUNIT_UNKNOWN;
}
/*****************************************************************************/

/*****************************************************************************/
/** Return pointer to string describing the calibration data units */
char *petCunit(
  /** index of PET_data units_string[] */
  int cunit
) {
  int n=0;
  while(cunit_string[n]!=0) n++;
  if(cunit<0 || cunit>n-1) return(cunit_string[CUNIT_UNKNOWN]);
  else return(cunit_string[cunit]);
}
/*****************************************************************************/

/*****************************************************************************/
/** Return pointer to string describing the time unit */
char *petTunit(
  /** index of PET_time unit_string[] */
  int tunit
) {
  int n=0;
  while(tunit_string[n]!=0) n++;
  if(tunit<0 || tunit>n-1) return(tunit_string[TUNIT_UNKNOWN]);
  else return(tunit_string[tunit]);
}
/*****************************************************************************/

/*****************************************************************************/
/** Tries to find calibration unit from filename.
\return Returns CUNIT, which is CUNIT_UNKNOWN if not successful.
 */
int cunitFromFilename(
  /** Pointer to filename, where calibration unit is tried to be found */
  char *fname
) {
  char *cptr;
  int i;

  if(fname==NULL || strlen(fname)<3) return CUNIT_UNKNOWN;
  for(i=0; i<2; i++) {
    if(i==0) /* First, look in the extension */
      {cptr=strrchr(fname, '.'); if(cptr==NULL) {cptr=fname; i++;}}
    else /* Then, look into whole filename */
      cptr=fname;
    if(strcasestr(cptr, "KBQ")!=NULL) return CUNIT_KBQ_PER_ML;
    if(strcasestr(cptr, "MBQ")!=NULL) return CUNIT_MBQ_PER_ML;
    if(strcasestr(cptr, "BQ")!=NULL) return CUNIT_BQ_PER_ML;
    if(strcasestr(cptr, "NCI")!=NULL) return CUNIT_NCI_PER_ML;
    if(strcasestr(cptr, "KCPS")!=NULL) return CUNIT_KCPS;
    if(strcasestr(cptr, "CPS")!=NULL) return CUNIT_CPS;
  }
  return CUNIT_UNKNOWN;
}
/*****************************************************************************/

/*****************************************************************************/
