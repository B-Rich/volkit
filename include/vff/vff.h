#ifndef VFF_H
#define VFF_H

/* Image type */
#define VFF_TYPE_UNKNOWN       0
#define VFF_TYPE_RASTER        1

/* Image format */
#define VFF_FORMAT_UNKNOWN     0
#define VFF_FORMAT_SLICE       1

typedef struct vff_header
{
    char           magic[5];
    unsigned short rank;
    unsigned short type;
    unsigned short format;
    unsigned short u_size;
    unsigned short v_size;
    unsigned short w_size;
    float          x0_origin;
    float          y0_origin;
    float          z0_origin;
    float          x1_extent;
    float          y1_extent;
    float          z1_extent;
    float          xsize_aspect;
    float          ysize_aspect;
    float          zsize_aspect;
    unsigned long  rawsize;
    unsigned short bands;
    unsigned short bits;
    char           title[32];
} VFF_header;

int vffReadHeader(FILE *fp, VFF_header *h);
void vffPrintHeader(VFF_header *h, FILE *fp);

#endif
