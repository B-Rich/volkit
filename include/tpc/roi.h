/******************************************************************************
  Copyright (c)  1996-2006 by Turku PET Centre

  roi.h - definitions for roi.c

  Version:
  1996-10-08 Vesa Oikonen
  2002-02-09 VO
  2002-11-22 Anne Sepponen
  2003-03-07 AS
  2003-07-02 CL
  2004-09-20 VO
    Changed the comments to doxygen style.
  2004-11-25 CL
  2005-01-20 CL
    Split the old ROI library to oldroi.[ch]
  2005-05-13 Harri Merisaari
    Included roi_Imadeus_IO.h
  2006-10-13 Kaisa Sederholm
    Recovered functions roi_mplane and roi_mframe
  2013-02-12 VO
    roi_Imadeus_IO.h is no more included.


******************************************************************************/
#ifndef _ROI_H
#define _ROI_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
/*****************************************************************************/
#include "libtpcroiv.h"
/*****************************************************************************/
/* Backup file extension */
#ifndef BACKUP_EXTENSION
#define BACKUP_EXTENSION ".bak"
#endif 
/*****************************************************************************/
extern char roierrmsg[128];
/*****************************************************************************/
/* Definitions for ROI types */
#define ROI_RECTANGULAR 0
#define ROI_CIRCULAR 1
#define ROI_ELLIPSE 2
#define ROI_TRACE 3
/*****************************************************************************/
/* ROI data structure */
typedef struct {
  /** Image file for which the ROIs have been defined */
  char imgfile[FILENAME_MAX];
  /** Zoom of the image when ROI was drawn */
  float zoom;
  /** Image reconstruction zoom */
  float recon_zoom;
  /** Matrix number (plane and frame) where ROI was defined */
  int matnum;
  /** Type of ROI (rectangular, circular,...) */
  int type;
  /** ROI status */
  int status;
  /** ROI initial point x, relative to top left corner of the image. */
  int pos_x;
  /** ROI initial point y, relative to top left corner of the image */
  int pos_y;
  /** ROI width for other than trace ROIs. */
  int w;
  /** ROI height for other than trace ROIs. */
  int h;
  /** t is unused. */
  int t;
  /** Number of ROI */
  int roi;
  /** ROI name as specified when drawing */
  char roiname[256];
  /** ROI line definition for trace ROIs; used only if type is ROI_TRACE,
   *  otherwise x and y should be NULL. */
  int *x;
  /** ROI line definition for trace ROIs; used only if type is ROI_TRACE,
   *  otherwise x and y should be NULL. */
  int *y;
  /** ROI line definition for trace ROIs; used only if type is ROI_TRACE,
   *  otherwise x and y should be NULL. */
  int point_nr;
  /** User data. Used only by client programs */
  void *userdata;
} ROI;

/** A doubly linked list for ROIs by CL */
typedef struct _RoiList {
  ROI *roi;
  struct _RoiList *next,*prev;
} RoiList;

/** Definition for a set of ROIs (depracated) */
typedef struct {
  /** Linkd list for ROIs */
  RoiList *rois;
  /** Number of ROIs in list; Deprecated, use the above linked list instead */
  int nr;
  /** Allocated memory for _allocNr ROIs*/
  int _allocNr;
  /** List of the rois (deprecated) */
  ROI *roi;
} ROI_list;

/*****************************************************************************/

/*****************************************************************************/

/* Initialize a ROI_list structure */
extern void roi_init(ROI_list *rl);

/* Delete all ROIs from a ROI_list */
extern void roi_empty(ROI_list *rl);

/* Delete a single ROI from ROI_list, pointed by pointer *roi */
extern void roi_delete(ROI_list *rl,ROI *roi);

/* Delete a single ROI from ROI_list, pointed by index number */
extern void roi_delete_n(ROI_list *rl,int index);

/* Read ROIs from file. They are appended to ROI_list  */
extern int roi_read(const char *fname,ROI_list *rl);

/* Save all ROIs from ROI_list overwriting the existing file */
extern int roi_save(const char *fname, ROI_list *rl); 

/* Append a single ROI into stream *fp */
extern int roi_append(FILE *fp,ROI *roi);

/* Append a single ROI to stream *fp from ROI_list, pointed by index. (indexes start from 0 */
extern int roi_append_n(FILE *fp, ROI_list *rl, int ind); 

extern int roi_onoff(ROI *roi,int dimx,int dimy,float zoom,char **m);

extern int roi_compute_rect(ROI *roi);
extern int roi_compute_circle(ROI *roi);
extern int roi_compute_ellipse(ROI *roi);
extern void roi_print(ROI *roi);
extern int jsqrt(int n);

extern int roi_mplane(int matnum);
extern int roi_mframe(int matnum);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

