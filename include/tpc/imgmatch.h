/******************************************************************************

  Copyright (c)  2007,2009,2013 by Turku PET Centre

  imgmatch.h - definitions for imgmatch.c

  Modification history:
  2007-01-31 Vesa Oikonen
       First created.
  2009-12-29 VO
       Added struct VOXEL_4D and function imgMaxDifference
  2013-04-02 VO
       Moved from libtpcimgp to libtpcimgio.


******************************************************************************/
#ifndef _IMGMATCH_H
#define _IMGMATCH_H
/*****************************************************************************/
#include "img.h"
/*****************************************************************************/
typedef struct {
  /** IMG column [1..dimx] (from left to right) */
  int x;
  /** IMG row [1..dimy] (from top to bottom) */
  int y;
  /** IMG plane [1..dimz] (from up to down) */
  int z;
  /** IMG time frame [1..dimt] */
  int t;
} VOXEL_4D;
/*****************************************************************************/
extern int imgMatch(IMG *img1, IMG *img2, float accuracy);
extern int imgMatchMatrix(IMG *img1, IMG *img2, double accuracy);
extern int imgMatchHeader(IMG *img1, IMG *img2);
extern int imgMatchTransform(IMG *img1, IMG *img2);
extern int imgMatchFrames(IMG *img1, IMG *img2);
extern int imgMatchPlanes(IMG *img1, IMG *img2);
extern int imgMaxDifference(
  IMG *img1, IMG *img2,
  VOXEL_4D *absdiff, float *abs_max, VOXEL_4D *reldiff, float *rel_max
);
/*****************************************************************************/

/*****************************************************************************/
#endif /* _IMGMATCH_H */

