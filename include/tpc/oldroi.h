#ifndef OLDROI_H
#define OLDROI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "roi.h"

#define roiEmpty roi_empty
#define roiInit roi_init
extern void roiDel(ROI_list *rl, int ind); /* Delete one ROI from list, */
extern int roiComputeRect(ROI *roi);
extern int roiComputeCirc(ROI *roi);
extern int roiComputeElli(ROI *roi);

#define roiPrint roi_print
#if 0
#define roiMPlane roi_mplane
#define roiMFrame roi_mframe
#endif
extern int roiRead(const char *fname, ROI_list *rl);
extern int roiFillGaps(int *x, int *y, int nr, int **ffx, int **ffy);
extern int roiOnOff(ROI *roi,int dimx,int dimy,float zoom,char **m);

extern int roiSave(const char *fname, ROI_list *rl); /* Save all ROIs in list */
extern int roiAppend(char *fname, ROI_list *rl, int ind); 

#ifdef __cplusplus
}
#endif

#endif

