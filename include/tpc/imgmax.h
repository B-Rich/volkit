/******************************************************************************

  imgmax.h    (c) 2007,2009,2012 by Turku PET Centre

  2007-01-24 VO
  2007-03-25 VO
  2009-12-01 VO
  2012-10-19 VO
  2012-10-27 VO
  2012-12-04 VO

******************************************************************************/
#ifndef _IMGMAX_H
#define _IMGMAX_H
/*****************************************************************************/
#include "img.h"
/*****************************************************************************/
extern int imgMax(IMG *img, float *maxvalue);
extern int imgAbsMax(IMG *img, float *maxvalue);
extern int imgRangeMinMax(IMG *img, IMG_RANGE *r, IMG_PIXEL *maxp, float *maxv,
  IMG_PIXEL *minp, float *minv);
extern int imgMinMax(IMG *img, float *minvalue, float *maxvalue);
extern int imgFrameMinMax(IMG *img, int frame, float *minvalue, float *maxvalue);
extern int imgReadMinMax(const char *fname, float *fmin, float *fmax);
extern int imgSmoothMax(IMG *img, float *maxvalue, IMG_PIXEL *p);
extern int imgGetPeak(IMG *img, float beforeTime, IMG_PIXEL *p, int verbose);
extern int imgAvg(IMG *img, IMG_RANGE *r, float *avg);
/*****************************************************************************/
#endif

