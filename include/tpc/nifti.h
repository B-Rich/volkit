/******************************************************************************

  Copyright (c) 2013 by Turku PET Centre

  nifti.h

  Version:
  2013-03-17 Vesa Oikonen
  2013-03-22 VO
       Added define NIFTI_HEADER_EXTENDER_SIZE.
       Added niftiWriteReader().
  2013-03-26 VO
       Added functions niftiCreateFNames() and niftiRemove().

******************************************************************************/
#ifndef _NIFTI_H
#define _NIFTI_H
/*****************************************************************************/
/* Backup file extension */
#ifndef BACKUP_EXTENSION
#define BACKUP_EXTENSION ".bak"
#endif 
/*****************************************************************************/
#define NIFTI_HEADER_SIZE 348
#define NIFTI_HEADER_EXTENDER_SIZE 4
/*****************************************************************************/
/* Define NIFTI1 units */
#define NIFTI_UNITS_UNKNOWN 0
#define NIFTI_UNITS_METER   1
#define NIFTI_UNITS_MM      2
#define NIFTI_UNITS_MICRON  4
#define NIFTI_UNITS_SEC     8
#define NIFTI_UNITS_MSEC   16
#define NIFTI_UNITS_USEC   24
/*****************************************************************************/
/* Define NIFTI1 datatypes (same as Analyze datatypes) */
#define NIFTI_DT_NONE 0
#define NIFTI_DT_UNKNOWN 0
#define NIFTI_DT_BINARY 1
#define NIFTI_DT_UNSIGNED_CHAR 2
#define NIFTI_DT_SIGNED_SHORT 4
#define NIFTI_DT_SIGNED_INT 8
#define NIFTI_DT_FLOAT 16
#define NIFTI_DT_COMPLEX 32
#define NIFTI_DT_DOUBLE 64
#define NIFTI_DT_RGB 128
#define NIFTI_DT_ALL 255
/*****************************************************************************/
/** Nifti-1 header, 348 bytes */
typedef struct {
  /** Must be 348 (byte offset 0) */
  int sizeof_hdr;
  /** Unused (byte offset 4) */
  char data_type[10];
  /** Unused (byte offset 14) */
  char db_name[18];
  /** Unused (byte offset 32) */
  int extents;
  /** Unused (byte offset 36) */
  short int session_error;
  /** Unused (byte offset 38) */
  char regular;
  /** MRI slice ordering (byte offset 39) */
  char dim_info;

  /** Data array dimensions; dim[0] is for the nr of dimensions,
   *  1,2,3 are for space, 4 is for time, 5 is for storing multiple values
   *  at each spatiotemporal voxel. (byte offset 40) */
  short int dim[8];
  /** 1st intent parameter (byte offset 56) */
  float intent_p1;
  /** 2nd intent parameter (byte offset 60) */
  float intent_p2;
  /** 3rd intent parameter (byte offset 64) */
  float intent_p3;
  /** NIFTI_INTENT_* (byte offset 68) */
  short int intent_code;
  /** Data type (byte offset 70) */
  short int datatype;
  /** Nr of bits per voxel (byte offset 72) */
  short int bitpix;
  /** First slice index (byte offset 74) */
  short int slice_start;
  /** Grid spacings starting from pixdim[1]; pixdim[0] contains orientation
   *  (byte offset 76) */
  float pixdim[8];
  /** Offset into .nii file (byte offset 108) */
  float vox_offset;
  /** Data scaling: slope (byte offset 112); pixel values should be scaled
   *  as scl_slope*x + scl_inter */
  float scl_slope;
  /** Data scaling: offset (byte offset 116); pixel values should be scaled
   *  as scl_slope*x + scl_inter */
  float scl_inter;
  /** Last slice index (byte offset 120) */
  short int slice_end;
  /** Slice timing order (byte offset 122) */
  char slice_code;
  /** Units of pixdim[1..4] (byte offset 123) */
  char xyzt_units;
  /** Max display intensity (byte offset 124) */
  float cal_max;
  /** Min display intensity (byte offset 128) */
  float cal_min;
  /** Time for 1 slice (byte offset 132) */
  float slice_duration;
  /** Time axis shift (byte offset 136) */
  float toffset;
  /** Unused (byte offset 140) */
  int glmax;
  /** Unused (byte offset 144) */
  int glmin;

  /** Free text field for study description (byte offset 148) */
  char descrip[80];
  /** Auxiliary filename (byte offset 228) */
  char aux_file[24];
  /** NIFTI_XFORM_* code (byte offset 252) */
  short int qform_code;
  /** NIFTI_XFORM_* code (byte offset 254) */
  short int sform_code;
  /** Quaternion b parameter (byte offset 256) */
  float quatern_b;
  /** Quaternion c parameter (byte offset 260) */
  float quatern_c;
  /** Quaternion d parameter (byte offset 264) */
  float quatern_d;
  /** Quaternion x shift (byte offset 268) */
  float qoffset_x;
  /** Quaternion y shift (byte offset 272) */
  float qoffset_y;
  /** Quaternion z shift (byte offset 276) */
  float qoffset_z;
  /** 1st row affine transformation (byte offset 280) */
  float srow_x[4];
  /** 2nd row affine transformation (byte offset 296) */
  float srow_y[4];
  /** 2rd row affine transformation (byte offset 312) */
  float srow_z[4];
  /** Meaning of data (Offset 0) (byte offset 328) */
  char intent_name[16];
  /** "ni1\0" (dual file) or "n+1\0" (single file) (byte offset 344) */
  char magic[4];
} NIFTI_1_HEADER;

/** This structure represents a 4-byte string that should follow the
    binary nifti_1_header data in a NIFTI-1 header file. 
 */
typedef struct {
 /** If the char values are {1,0,0,0}, the file is expected to contain
  *  extensions, values of {0,0,0,0} imply the file does not contain extensions.
  *  Other sequences of values are not currently defined. */
  char extension[4];
} NIFTI_EXTENDER;

/** Combination of all NIFTI headers */
typedef struct {
  /** NIfTI-1 header */
  NIFTI_1_HEADER h;
  /** Extension; obligatory in .nii file */ 
  NIFTI_EXTENDER e;
  /** Specifies whether data is stored on disk as little endian (1),
   *  or big endian (0). */
  int byte_order;
} NIFTI_DSR;
/*****************************************************************************/

/*****************************************************************************/
extern int niftiExists(
  const char *dbname, char *hdrfile, char *imgile, char *siffile,
  NIFTI_DSR *header, int verbose, char *status
);
extern int niftiCreateFNames(
  const char *filename, char *hdrfile, char *imgfile, char *siffile,
  int fileformat
);
extern int niftiRemove(
  const char *dbname, int fileformat, int verbose
);
extern int niftiReadHeader(
  char *filename, NIFTI_DSR *h, int verbose, char *status
);
extern int niftiPrintHeader(NIFTI_DSR *h, FILE *fp);
extern void niftiRemoveFNameExtension(char *fname);
extern int niftiReadImagedata(
  FILE *fp, NIFTI_DSR *h, int frame, float *data, int verbose, char *status
);
extern int niftiWriteHeader(
  char *filename, NIFTI_DSR *dsr, int verbose, char *status
);
/*****************************************************************************/

/*****************************************************************************/
#endif
