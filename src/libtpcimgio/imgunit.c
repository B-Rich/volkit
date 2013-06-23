/******************************************************************************

  Copyright (c) 2006-2013 Turku PET Centre

  Library:      imgunit
  Description:  Functions for setting image calibration unit.

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

  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi

  Modification history:
  2006-10-30 Vesa Oikonen
    First created.
  2007-02-02 VO
    imgUnitId() identifies more unit strings.
    Added more IMG units.
    Functions imgUnitFromEcat7() and imgUnit() rewritten.
  2007-17-07 Harri Merisaari
    Modified for optional ANSi compatibility
  2008-07-24 VO
    Added function imgSetUnit().
  2010-05-11 VO
    Function imgUnitFromEcat7() tries to identify unit with imgUnitId()
    always when ECAT 7 calibration_units is 2; previosly in these cases
    unit was assumed to be unknown.
  2011-02-04 VO
    imgUnitToEcat7() writes label "unitless" in the header, previosly header
    was marked to contain no data_units label.
  2012-05-08 VO
    imgUnitId() identifies certain radioactivity concentration units also
    without '/'-character.

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
/*****************************************************************************/
#include "img.h"
#include "substitutions.h"
/*****************************************************************************/
#include "imgunit.h"
/*****************************************************************************/
/** Valid IMG units. */
static char *img_unit_string[] = {
  /*  0 */  "unknown",
  /*  1 */  "cnts/sec",
  /*  2 */  "counts",
  /*  3 */  "kBq/mL",
  /*  4 */  "sec*kBq/mL",
  /*  5 */  "1/sec",
  /*  6 */  "1/min",
  /*  7 */  "mL/mL",
  /*  8 */  "mL/dL",
  /*  9 */  "mL/(mL*min)",
  /* 10 */  "mL/(dL*min)",
  /* 11 */  "unitless",
  /* 12 */  "nCi/mL",
  /* 13 */  "MBq/mL",
  /* 14 */  "Bq/cc",
  /* 15 */  "uCi/cc",
  /* 16 */  "umol/(100g*min)",
  /* 17 */  "mg/(100g*min)",
            0
};
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Identifies the specified unit string as IMG unit.
 *
 * @param unit pointer to string
 * @return unit id number, -1 invalid input, -3 no match found
 */
int imgUnitId(char *unit) {
  if(unit==NULL) return(-1);
  if(strlen(unit)==0)                             return IMGUNIT_UNKNOWN;
  else if(strcasecmp(unit, "unknown")==0)         return IMGUNIT_UNKNOWN;
  else if(strcasecmp(unit, "cnts/sec")==0)        return IMGUNIT_CPS;
  else if(strcasecmp(unit, "counts/sec")==0)      return IMGUNIT_CPS;
  else if(strcasecmp(unit, "ECAT counts/sec")==0) return IMGUNIT_CPS;
  else if(strcasecmp(unit, "cps")==0)             return IMGUNIT_CPS;
  else if(strcasecmp(unit, "counts")==0)          return IMGUNIT_COUNTS;
  else if(strcasecmp(unit, "cnts")==0)            return IMGUNIT_COUNTS;
  else if(strcasecmp(unit, "kBq/cc")==0)          return IMGUNIT_KBQ_PER_ML;
  else if(strcasecmp(unit, "kBqcc")==0)           return IMGUNIT_KBQ_PER_ML;
  else if(strcasecmp(unit, "kBq/mL")==0)          return IMGUNIT_KBQ_PER_ML;
  else if(strcasecmp(unit, "kBqmL")==0)           return IMGUNIT_KBQ_PER_ML;
  else if(strcasecmp(unit, "sec*kBq/cc")==0)      return IMGUNIT_SEC_KBQ_PER_ML;
  else if(strcasecmp(unit, "sec*kBq/mL")==0)      return IMGUNIT_SEC_KBQ_PER_ML;
  else if(strcasecmp(unit, "integral")==0)        return IMGUNIT_SEC_KBQ_PER_ML;
  else if(strcasecmp(unit, "1/sec")==0)           return IMGUNIT_PER_SEC;
  else if(strcasecmp(unit, "1/s")==0)             return IMGUNIT_PER_SEC;
  else if(strcasecmp(unit, "s-1")==0)             return IMGUNIT_PER_SEC;
  else if(strcasecmp(unit, "1/min")==0)           return IMGUNIT_PER_MIN;
  else if(strcasecmp(unit, "min-1")==0)           return IMGUNIT_PER_MIN;
  else if(strcasecmp(unit, "mL/mL")==0)           return IMGUNIT_ML_PER_ML;
  else if(strcasecmp(unit, "mL/cc")==0)           return IMGUNIT_ML_PER_ML;
  else if(strcasecmp(unit, "mL/dL")==0)           return IMGUNIT_ML_PER_DL;
  else if(strcasecmp(unit, "mL/100mL")==0)        return IMGUNIT_ML_PER_DL;
  else if(strcasecmp(unit, "mL/(mL*min)")==0)     return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*mL)")==0)     return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(cc*min)")==0)     return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*cc)")==0)     return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/mL/min")==0)       return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/min/mL")==0)       return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/cc/min")==0)       return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/min/cc")==0)       return IMGUNIT_ML_PER_ML_PER_MIN;
  else if(strcasecmp(unit, "mL/(dL*min)")==0)     return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*dL)")==0)     return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/(100mL*min)")==0)  return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/(min*100mL)")==0)  return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/dL/min")==0)       return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/min/dL")==0)       return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/100mL/min")==0)    return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "mL/min/100mL")==0)    return IMGUNIT_ML_PER_DL_PER_MIN;
  else if(strcasecmp(unit, "unitless")==0)        return IMGUNIT_UNITLESS;
  else if(strcasecmp(unit, "nCi/cc")==0)          return IMGUNIT_NCI_PER_ML;
  else if(strcasecmp(unit, "nCicc")==0)           return IMGUNIT_NCI_PER_ML;
  else if(strcasecmp(unit, "nCi/mL")==0)          return IMGUNIT_NCI_PER_ML;
  else if(strcasecmp(unit, "nCimL")==0)           return IMGUNIT_NCI_PER_ML;
  else if(strcasecmp(unit, "MBq/cc")==0)          return IMGUNIT_MBQ_PER_ML;
  else if(strcasecmp(unit, "MBqcc")==0)           return IMGUNIT_MBQ_PER_ML;
  else if(strcasecmp(unit, "MBq/mL")==0)          return IMGUNIT_MBQ_PER_ML;
  else if(strcasecmp(unit, "MBqmL")==0)           return IMGUNIT_MBQ_PER_ML;
  else if(strcasecmp(unit, "Bq/cc")==0)           return IMGUNIT_BQ_PER_ML;
  else if(strcasecmp(unit, "Bqcc")==0)            return IMGUNIT_BQ_PER_ML;
  else if(strcasecmp(unit, "Bq/mL")==0)           return IMGUNIT_BQ_PER_ML;
  else if(strcasecmp(unit, "BqmL")==0)            return IMGUNIT_BQ_PER_ML;
  else if(strcasecmp(unit, "uCi/cc")==0)          return IMGUNIT_UCI_PER_ML;
  else if(strcasecmp(unit, "uCicc")==0)           return IMGUNIT_UCI_PER_ML;
  else if(strcasecmp(unit, "uCi/mL")==0)          return IMGUNIT_UCI_PER_ML;
  else if(strcasecmp(unit, "uCimL")==0)           return IMGUNIT_UCI_PER_ML;
  else if(strcasecmp(unit, "umol/(100g*min)")==0) return IMGUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/(min*100g)")==0) return IMGUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/100g/min")==0)   return IMGUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "umol/min/100g")==0)   return IMGUNIT_UMOL_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/(100g*min)")==0)   return IMGUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/(min*100g)")==0)   return IMGUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/100g/min")==0)     return IMGUNIT_MG_PER_MIN_PER_100G;
  else if(strcasecmp(unit, "mg/min/100g")==0)     return IMGUNIT_MG_PER_MIN_PER_100G;

  return(-3);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Set IMG calibration unit based on ECAT 6.3 unit
 *
 * @param img target image structure
 * @param ecat_unit calibration unit code number
 */
void imgUnitFromEcat(IMG *img, int ecat_unit) {
  switch(ecat_unit) {
    case  0: /* Unknown               */
      img->unit=IMGUNIT_UNKNOWN; break;
    case  1: /* MBq/mL                */
      img->unit=IMGUNIT_MBQ_PER_ML; break;
    case  2: /* ECAT counts           */
      img->unit=IMGUNIT_COUNTS; break;
    case  3: /* uCi/ml                */
      img->unit=IMGUNIT_UCI_PER_ML; break;
    case  4: /* LMRGlu                */
      img->unit=IMGUNIT_UNKNOWN; break;
    case  5: /* LMRUGlu umol/min/100g */
      img->unit=IMGUNIT_UMOL_PER_MIN_PER_100G; break;
    case  6: /* LMRUGlu mg/min/100g   */
      img->unit=IMGUNIT_MG_PER_MIN_PER_100G; break;
    case  7: /* nCi/mL                */
      img->unit=IMGUNIT_NCI_PER_ML; break;
    case  8: /* Well counts           */
      img->unit=IMGUNIT_CPS; break;
    case  9: /* Becquerels            */
      img->unit=IMGUNIT_BQ_PER_ML; break;
    case 10: /* kBq/mL                */
      img->unit=IMGUNIT_KBQ_PER_ML; break;
    case 11: /* 1/min                 */
      img->unit=IMGUNIT_PER_MIN; break;
    case 12: /* mL/min/100g           */
      img->unit=IMGUNIT_ML_PER_DL_PER_MIN; break;
    case 13: /* sec*kBq/mL            */
      img->unit=IMGUNIT_SEC_KBQ_PER_ML; break;
    case 14: /* sec*nCi/mL            */
      img->unit=IMGUNIT_UNKNOWN; break;
    case 15: /* 1/sec                 */
      img->unit=IMGUNIT_PER_SEC; break;
    case 16: /* Unitless              */
      img->unit=IMGUNIT_UNITLESS; break;
    case 17: /* Unknown               */
    default:
      img->unit=IMGUNIT_UNKNOWN; break;
  }
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Set IMG calibration unit based on ECAT7 main header
 *
 * @param img pointer to IMG data where unit will be set
 * @param h pointer to source ECAT7 main header
 */
void imgUnitFromEcat7(IMG *img, ECAT7_mainheader *h) {
  if(h->calibration_units==0) { /* Not calibrated */
    img->unit=1;
  } else if(h->calibration_units==1) {
    if(h->calibration_units_label==0)
      img->unit=IMGUNIT_BQ_PER_ML;
    else
      img->unit=imgUnitId(h->data_units);
  } else if(h->calibration_units==2) {
    img->unit=imgUnitId(h->data_units);
  } else {
    img->unit=IMGUNIT_UNKNOWN;
  }
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Return ECAT 6.3 calibration unit based on IMG unit
 *
 * @param img image structure
 * @return calibration unit code number
 */
int imgUnitToEcat6(IMG *img) {
  int ecat_unit;
  switch(img->unit) {
    case IMGUNIT_CPS: ecat_unit=9; break;
    case IMGUNIT_COUNTS: ecat_unit=2; break;
    case IMGUNIT_KBQ_PER_ML: ecat_unit=10; break;
    case IMGUNIT_SEC_KBQ_PER_ML: ecat_unit=13; break;
    case IMGUNIT_PER_SEC: ecat_unit=15; break;
    case IMGUNIT_PER_MIN: ecat_unit=11; break;
    case IMGUNIT_ML_PER_ML: ecat_unit=16; break;
    case IMGUNIT_ML_PER_DL: ecat_unit=16; break;
    case IMGUNIT_ML_PER_ML_PER_MIN: ecat_unit=11; break;
    case IMGUNIT_ML_PER_DL_PER_MIN: ecat_unit=12; break;
    case IMGUNIT_UNITLESS: ecat_unit=16; break;
    case IMGUNIT_NCI_PER_ML: ecat_unit=7; break;
    case IMGUNIT_MBQ_PER_ML: ecat_unit=1; break;
    case IMGUNIT_BQ_PER_ML: ecat_unit=9; break;
    case IMGUNIT_UCI_PER_ML: ecat_unit=3; break;
    case IMGUNIT_UMOL_PER_MIN_PER_100G: ecat_unit=5; break;
    case IMGUNIT_MG_PER_MIN_PER_100G: ecat_unit=6; break;
    default: ecat_unit=0; break;
  }
  return(ecat_unit);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Set ECAT 7 mainheader calibration units based on IMG unit
 *
 * @param img source image stucture
 * @param h target Ecat7 main header
 */
void imgUnitToEcat7(IMG *img, ECAT7_mainheader *h) {
  switch(img->unit) {
    case IMGUNIT_CPS:
    case IMGUNIT_COUNTS:
      h->calibration_units=0;
      h->calibration_units_label=1;
      strcpy(h->data_units, "ECAT counts/sec");
      break;
    case IMGUNIT_KBQ_PER_ML:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "kBq/cc");
      break;
    case IMGUNIT_SEC_KBQ_PER_ML:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "sec*kBq/mL");
      break;
    case IMGUNIT_PER_SEC:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "1/sec");
      break;
    case IMGUNIT_PER_MIN:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "1/min");
      break;
    case IMGUNIT_ML_PER_ML:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "mL/mL");
      break;
    case IMGUNIT_ML_PER_DL:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "mL/dL");
      break;
    case IMGUNIT_ML_PER_ML_PER_MIN:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "mL/(mL*min)");
      break;
    case IMGUNIT_ML_PER_DL_PER_MIN:
      h->calibration_units=2;
      h->calibration_units_label=1;
      strcpy(h->data_units, "mL/(dL*min)");
      break;
    case IMGUNIT_UNITLESS:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "unitless");
      break;
    case IMGUNIT_NCI_PER_ML:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "nCi/cc");
      break;
    case IMGUNIT_MBQ_PER_ML:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "MBq/cc");
      break;
    case IMGUNIT_BQ_PER_ML:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "Bq/cc");
      break;
    case IMGUNIT_UCI_PER_ML:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "uCi/cc");
      break;
    case IMGUNIT_UMOL_PER_MIN_PER_100G:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "umol/min/100g");
      break;
    case IMGUNIT_MG_PER_MIN_PER_100G:
      h->calibration_units=1;
      h->calibration_units_label=1;
      strcpy(h->data_units, "mg/min/100g");
      break;
    default:
      h->calibration_units=2;
      h->calibration_units_label=0;
      strcpy(h->data_units, "");
      break;
  }
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Return pointer to string describing the calibrated image data unit
 *
 * @param dunit index of img_unit_string[]
 * @return pointer to string
 */
char *imgUnit(int dunit) {
  int n=0;
  while(img_unit_string[n]!=0) n++;
  if(dunit<0 || dunit>n-1) return(img_unit_string[IMGUNIT_UNKNOWN]);
  else return(img_unit_string[dunit]);
}
/*****************************************************************************/

/*****************************************************************************/
/** Sets the unit in IMG based on specified unit string.
\return Returns 0 if successful.
 */
int imgSetUnit(IMG *img, char *unit)
{
  int new_unit;

  if(img==NULL || unit==NULL) return(1);
  new_unit=imgUnitId(unit); if(new_unit<0) return(1-new_unit);
  img->unit=new_unit;
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/

