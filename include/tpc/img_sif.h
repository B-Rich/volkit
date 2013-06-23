/******************************************************************************
  Copyright (c)  2013 by Turku PET Centre

  img_sif.h - definitions for img_sif.c

  Version:
  2013-04-04 Vesa Oikonen


******************************************************************************/
#ifndef _IMG_SIF_H
#define _IMG_SIF_H
/*****************************************************************************/
#include "img.h"
#include "sif.h"
/*****************************************************************************/
extern int sif2img(
  SIF *sif, IMG *img, int copy_header, int copy_frames, int copy_counts,
  int verbose
);
extern int img2sif(
  IMG *img, SIF *sif, int copy_header, int copy_frames, int copy_counts,
  int verbose
);
/*****************************************************************************/

/*****************************************************************************/
#endif /* _IMG_SIF_H */

