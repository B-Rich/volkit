/******************************************************************************

  imgunit.h    (c) 2006-2008 by Turku PET Centre

  2006-10-30 Vesa Oikonen
  2007-02-02 VO
    Added enum for IMG units.
  2008-07-24 VO
    Introduced new functions.

******************************************************************************/
#ifndef _IMGUNIT_H
#define _IMGUNIT_H
/*****************************************************************************/
#include "img.h"
#include "ecat7.h"
/*****************************************************************************/
enum {
  /*  0 */ IMGUNIT_UNKNOWN,
  /*  1 */ IMGUNIT_CPS,
  /*  2 */ IMGUNIT_COUNTS,
  /*  3 */ IMGUNIT_KBQ_PER_ML,
  /*  4 */ IMGUNIT_SEC_KBQ_PER_ML,
  /*  5 */ IMGUNIT_PER_SEC,
  /*  6 */ IMGUNIT_PER_MIN,
  /*  7 */ IMGUNIT_ML_PER_ML,
  /*  8 */ IMGUNIT_ML_PER_DL,
  /*  9 */ IMGUNIT_ML_PER_ML_PER_MIN,
  /* 10 */ IMGUNIT_ML_PER_DL_PER_MIN,
  /* 11 */ IMGUNIT_UNITLESS,
  /* 12 */ IMGUNIT_NCI_PER_ML,
  /* 13 */ IMGUNIT_MBQ_PER_ML,
  /* 14 */ IMGUNIT_BQ_PER_ML,
  /* 15 */ IMGUNIT_UCI_PER_ML,
  /* 16 */ IMGUNIT_UMOL_PER_MIN_PER_100G,
  /* 17 */ IMGUNIT_MG_PER_MIN_PER_100G
};
/*****************************************************************************/
extern int imgUnitId(char *unit);
extern void imgUnitFromEcat(IMG *img, int ecat_unit);
extern void imgUnitFromEcat7(IMG *img, ECAT7_mainheader *h);
extern int imgUnitToEcat6(IMG *img);
extern void imgUnitToEcat7(IMG *img, ECAT7_mainheader *h);
extern char *imgUnit(int dunit);
extern int imgSetUnit(IMG *img, char *unit);
/*****************************************************************************/
#endif

