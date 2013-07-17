#ifndef VFF_H
#define VFF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* Magic number */
#define VFF_MAGICNR            "ncaa"

/* Image type */
#define VFF_TYPE_UNKNOWN       0
#define VFF_TYPE_RASTER        1

/* Image format */
#define VFF_FORMAT_UNKNOWN     0
#define VFF_FORMAT_SLICE       1

/* File types */
#define VFF_DEPTH8             8
#define VFF_DEPTH16            16

typedef struct vff_frameheader
{
    float            start_time;
    float            mean_time;
    float            end_time;
} VFF_frameheader;

typedef struct vff_mainheader
{
    char             magic_number[5];
    unsigned short   rank;
    unsigned short   type;
    unsigned short   format;
    unsigned short   u_size;
    unsigned short   v_size;
    unsigned short   w_size;
    float            x0_origin;
    float            y0_origin;
    float            z0_origin;
    float            x1_extent;
    float            y1_extent;
    float            z1_extent;
    float            xsize_aspect;
    float            ysize_aspect;
    float            zsize_aspect;
    unsigned long    rawsize;
    unsigned short   bands;
    unsigned short   bits;
    char             title[32];
    float            value[2];
    unsigned short   frames;
    VFF_frameheader  *frame;
    long             header_size;
} VFF_mainheader;

int vffReadMainheader(FILE *fp, VFF_mainheader *h);
void vffPrintMainheader(VFF_mainheader *h, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif

