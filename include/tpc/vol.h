/******************************************************************************
  Copyright (c)  2003,2004,2012 by Turku PET Centre

  vol.h - definitions for vol.c

  Version:
  2003-12-18 Vesa Oikonen
    Added 3D structures VOL and SVOL and related functions.
  2004-01-29 VO
    Added functions vol2img() and svol2img.
  2004-08-23 VO
    Some Doxygen style comments.
  2012-10-19 VO
    Added functions vrdRead(), vrdReorder(), and string_to_xyz().
  2012-10-23 VO
    Interfaces of volAvg() and volMax() changed.


******************************************************************************/
#ifndef _VOL_H
#define _VOL_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
extern int VOL_TEST;
/*****************************************************************************/

/*****************************************************************************/
typedef struct {
  int x;  /* [1..dimx] */
  int y;  /* [1..dimy] */
  int z;  /* [1..dimz] */
} VOL_PIXEL;
typedef struct {
  int x1, x2;  /* [1..dimx] */
  int y1, y2;  /* [1..dimy] */
  int z1, z2;  /* [1..dimz] */
} VOL_RANGE;  
/*****************************************************************************/
  
/*****************************************************************************/
/*
 *  3D volume data structure - 4-byte float voxels
 */
typedef struct {

  /** Volume status */
  char status;
  /** Pointer to _imgStatusMessage, describing current status */
  char *statmsg;
  /** Orientation */
  int orientation;
  /** Pixel size in all dimensions (mm) */
  float sizex, sizey, sizez;
  /** Dimensions of Column (c/x), Row (r/y), Plane (p/z) */
  unsigned short int dimx, dimy, dimz;
  /** Hidden pointers for actual data */
  float *_vxl, *_col, **_row, ***_pln;
  /** Pointers for data to be used */
  float ***v, ***plane, **row, *column, *voxel;

} VOL;
/*****************************************************************************/
/*
 *  3D volume data structure - 2-byte short int voxels
 */
typedef struct {

  /** Volume status */
  char status;
  /** Pointer to _imgStatusMessage, describing current status */
  char *statmsg;
  /** Orientation */
  int orientation;
  /** Pixel size in all dimensions (mm) */
  float sizex, sizey, sizez;
  /** Dimensions of Column (c/x), Row (r/y), Plane (p/z) */
  unsigned short int dimx, dimy, dimz;
  /** Scaling factor */
  float scale_factor;
  /** Hidden pointers for actual data */
  short int *_vxl, *_col, **_row, ***_pln;
  /** Pointers for data to be used */
  short int ***v, ***plane, **row, *column, *voxel;

} SVOL;
/*****************************************************************************/

/*****************************************************************************/
/*
 *  Function definitions
 */
/* 4-byte floats */
extern void volInit(VOL *vol);
extern void volEmpty(VOL *vol);
extern int volAllocate(VOL *vol, int planes, int rows, int columns);
extern int img2vol(IMG *img, VOL *vol, int frame);
extern int vol2img(VOL *vol, IMG *img, int frame);
extern void volInfo(VOL *vol, FILE *fp);
extern void volContents(VOL *vol, VOL_RANGE r, FILE *fp);
extern int volMax(
  VOL *vol, VOL_RANGE *r,
  VOL_PIXEL *maxp, float *maxv, VOL_PIXEL *minp, float *minv
);
extern int volAvg(VOL *vol, VOL_RANGE *r, float *avg);
/* 2-byte short ints */
extern void svolInit(SVOL *svol);
extern void svolEmpty(SVOL *svol);
extern int svolAllocate(SVOL *svol, int planes, int rows, int columns);
extern int img2svol(IMG *img, SVOL *svol, int frame);
extern int svol2img(SVOL *svol, IMG *img, int frame);
extern void svolInfo(SVOL *svol, FILE *fp);
/* General */
extern int vrdReorder(VOL_RANGE *vol_range);
extern int vrdRead(char *vdffile, VOL_RANGE *vol_range, char *status);
extern int string_to_xyz(char *str, int *x, int *y, int *z);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* _VOL_H */

