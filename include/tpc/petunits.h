/******************************************************************************
  Copyright (c) 2007,2010-2013 by Turku PET Centre

  petunits.h

  Version:
  2007-08-10 Vesa Oikonen
    First introduced.
  2010-07-01 VO
    Added function cunitFromFilename().
  2011-07-01 VO
    Added units Bq, kBq, MBq, GBq, nCi, uCi, and mCi.
  2012-02-24 VO
    Added units %ID, %ID/g and %ID/mL.
  2013-06-17 VO
    Added units CUNIT_G_PER_ML and CUNIT_ML_PER_G


******************************************************************************/
#ifndef _PETUNITS_H
#define _PETUNITS_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
/** Max length of units string (+1), based on ECAT7 format */
#ifndef MAX_UNITS_LEN
#define MAX_UNITS_LEN 31
#endif
/*****************************************************************************/
enum {
  /*  0 */ CUNIT_UNKNOWN,
  /*  1 */ CUNIT_CPS,
  /*  2 */ CUNIT_COUNTS,
  /*  3 */ CUNIT_KBQ_PER_ML,
  /*  4 */ CUNIT_SEC_KBQ_PER_ML,
  /*  5 */ CUNIT_PER_SEC,
  /*  6 */ CUNIT_PER_MIN,
  /*  7 */ CUNIT_ML_PER_ML,
  /*  8 */ CUNIT_ML_PER_DL,
  /*  9 */ CUNIT_ML_PER_ML_PER_MIN,
  /* 10 */ CUNIT_ML_PER_DL_PER_MIN,
  /* 11 */ CUNIT_UNITLESS,
  /* 12 */ CUNIT_NCI_PER_ML,
  /* 13 */ CUNIT_MBQ_PER_ML,
  /* 14 */ CUNIT_BQ_PER_ML,
  /* 15 */ CUNIT_UCI_PER_ML,
  /* 16 */ CUNIT_UMOL_PER_MIN_PER_100G,
  /* 17 */ CUNIT_MG_PER_MIN_PER_100G,
  /* 18 */ CUNIT_UMOL_PER_MIN_PER_DL,
  /* 19 */ CUNIT_MG_PER_MIN_PER_DL,
  /* 20 */ CUNIT_PERCENTAGE,
  /* 21 */ CUNIT_KCPS,
  /* 22 */ CUNIT_MIN_KBQ_PER_ML,
  /* 23 */ CUNIT_BQ,
  /* 24 */ CUNIT_KBQ,
  /* 25 */ CUNIT_MBQ,
  /* 26 */ CUNIT_GBQ,
  /* 27 */ CUNIT_NCI,
  /* 28 */ CUNIT_UCI,
  /* 29 */ CUNIT_MCI,
  /* 30 */ CUNIT_PID,
  /* 31 */ CUNIT_PIDM,
  /* 32 */ CUNIT_PIDV,
  /* 33 */ CUNIT_G_PER_ML, // SUV unit
  /* 34 */ CUNIT_ML_PER_G // SUV unit
};
enum {
  /*  0 */ TUNIT_UNKNOWN,
  /*  1 */ TUNIT_SEC,
  /*  2 */ TUNIT_MIN,
  /*  3 */ TUNIT_UM,
  /*  4 */ TUNIT_MM,
  /*  5 */ TUNIT_CM,
  /*  6 */ TUNIT_M,
  /*  7 */ TUNIT_HOUR,
  /*  8 */ TUNIT_MONTH,
  /*  9 */ TUNIT_YEAR,
  /* 10 */ TUNIT_MSEC
};
/*****************************************************************************/
extern int petCunitId(const char *unit);
extern int petTunitId(const char *timeunit);
/*****************************************************************************/
extern char *petCunit(int cunit);
extern char *petTunit(int tunit);
/*****************************************************************************/
extern int cunitFromFilename(char *fname);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

