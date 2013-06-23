/******************************************************************************

  imgdecay.h    (c) 2007,2012 by Turku PET Centre

  Modification history:
  2007-01-28 VO
  2012-02-24 VO

******************************************************************************/
#ifndef _IMGDECAY_H
#define _IMGDECAY_H
/*****************************************************************************/
#include "img.h"
/*****************************************************************************/
extern int imgDecayCorrection(IMG *img, int mode);
extern char *imgIsotope(IMG *img);
extern int imgSetDecayCorrFactors(IMG *image, int mode);
extern int imgBranchingCorrection(
  IMG *image, int mode, int verbose, char *status);
/*****************************************************************************/
#endif

