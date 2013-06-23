/*******************************************************************************

  Copyright (c)  2002-2009,2012,2013 by Turku PET Centre

  img.h - definitions for img.c

  Version:
  2002-01-05 Vesa Oikonen
  2002-03-28 VO
    sampleDistance included in IMG structure.
  2002-08-23 VO
    _dataType included in IMG structure.
  2002-12-01 VO
    imgDecayCorrection() can be used also to remove decay correction.
  2002-12-03 VO
    imgCopyhdr() included.
  2002-12-23 VO
    patientName included in IMG structure.
    imgIsotope() included.
    decayCorrFactor[] included in IMG structure.
  2003-09-04 VO
    _fileFormat and its definitions included in IMG structure.
  2003-10-04 VO
    One more _fileFormat definition added (Analyze little endian byte order).
  2003-12-14 VO
    Memory for all pixels is allocated in one chunk;
    pointers are changed accordingly.
    (Patient) orientation included in IMG structure.
    Scanner (type) included in IMG structure.
    Scanner type definitions moved here from imgproc.h.
  2003-12-18 VO
    Added struct IMG_PIXEL and IMG_RANGE.
    Defined function imgExtractRange().
  2004-05-23 VO
    Added comments.
  2004-07-26 VO
    More comments.
  2004-08-23 VO
    MAX_STUDYNR_LEN applied.
  2006-10-30 VO
  2007-01-28 VO
    Definitions for library version 1.2.0:
    Added defines for NIfTI-1 file formats.
    Additional scanner types defined.
    Added definitions for error status messages.
    Additions in IMG structure:
    patientID, userProcessCode, studyDescription,
    gapx, gapy, gapz,
    resolutionx, resolutiony, resolutionz,
    prompts and randoms.
  2007-02-11 VO
    Added errstatus in IMG struct.
  2007-02-27 VO
    Added STATUS_WRONGFILETYPE and STATUS_CANNOTERASE.
  2007-03-13 VO
    Added STATUS_CANNOTREAD and STATUS_CANNOTWRITE.
  2007-03-26 VO
    Added variables for polar map in IMG struct.
  2008-07-14 VO
    Added imgAllocateWithHeader().
  2009-02-16 VO
    Added definitions for scanner and image type.
  2012-02-04 VO
    Included brachingFactor in IMG struct.
  2012-02-15 VO
    Included calibrationFactor in IMG struct.
  2012-02-17 VO
    Included IFT struct in IMG struct.
    Correction in comment.
  2012-02-19 VO
    Added definitions for modality.
    Modality added to IMG struct.
  2012-09-21 VO
    Added definitions for physical decay correction, and values are changed:
    previously 0=not corrected, 1=corrected; now 0=unknown, 1=corrected, and
    2=noncorrected.
    Accordingly, IMG struct item decayCorrected was changed to decayCorrection.
  2012-10-19 VO
    Added function ifrReorder().
  2013-03-07 VO
    Added function imgDup().
  2013-03-18 VO
    Added IMG field float quatern[6] for quaternion parameters.
  2013-03-28 VO
    Added more orientation and transformation fields to IMG struct.
  2013-04-04 VO
    Added imgExistentTimes() and imgExistentCounts().


*******************************************************************************/
#ifndef _IMG_H
#define _IMG_H
/******************************************************************************/
#include "studynr.h"
#include "ift.h"
/******************************************************************************/
/** Definitions for img status */
#define IMG_STATUS_UNINITIALIZED 0
#define IMG_STATUS_INITIALIZED   1
#define IMG_STATUS_OCCUPIED      2
#define IMG_STATUS_ERROR         3
/** Definitions for img error status message */
#define IMG_ERR_OK      0
#define IMG_ERR_CALLING 1
#define IMG_ERR_OOM     2
/** Definitions for image type */
#define IMG_TYPE_UNKNOWN  0
#define IMG_TYPE_IMAGE    1
#define IMG_TYPE_RAW      2 /* sinogram */
#define IMG_TYPE_POLARMAP 3
/** Definitions for file format */
#define IMG_UNKNOWN   0
#define IMG_E63       1
#define IMG_E7        2
#define IMG_E7_2D     3
#define IMG_POLARMAP  9
#define IMG_ANA       11
#define IMG_ANA_L     12 /* little endian variant */
#define IMG_INTERFILE 21
#define IMG_NIFTI_1D  31 /* dual file format */   
#define IMG_NIFTI_1S  32 /* single file format */
#define IMG_MICROPET  41
#define IMG_DICOM     100
/** Definitions for physical decay correction */
#define IMG_DC_UNKNOWN 0
#define IMG_DC_CORRECTED 1
#define IMG_DC_NONCORRECTED 2
/** Definitions for modality */
#define IMG_MODALITY_UNKNOWN 0
#define IMG_MODALITY_PET 1
#define IMG_MODALITY_MRI 2
#define IMG_MODALITY_CT 3
#define IMG_MODALITY_SPECT 4
/** Definition for scanner model (system type) */
#define SCANNER_UNKNOWN 0
#define SCANNER_ECAT931 12
#define SCANNER_ADVANCE 12096
#define SCANNER_HRPLUS 3
#define SCANNER_HRRT 4
/* these may change later */
#define SCANNER_MRI 5
#define SCANNER_STEVCT_PET 6
#define SCANNER_STEVCT_CT 7
/* Concorde/MicropET scanners */
#define SCANNER_PRIMATE 2000
#define SCANNER_RODENT 2001
#define SCANNER_MICROPET2 2002
#define SCANNER_FOCUS_220 2500
#define SCANNER_FOCUS_120 2501
#define SCANNER_INVEON_DEDICATED_PET 5000
#define SCANNER_INVEON_MM_PET 5500
#define SCANNER_MR_PET_HEAD_INSERT 6000
#define SCANNER_TUEBINGEN_PET_MR 8000
/** Maximum nr of rings in polar map (based on ECAT 7 polar map header) */
#define MAX_POLARMAP_NUM_RINGS 32
/** Definitions for img status message */
enum {STATUS_OK,STATUS_FAULT,STATUS_NOMEMORY,STATUS_NOFILE,STATUS_UNKNOWNFORMAT,
    STATUS_UNSUPPORTED,STATUS_MISSINGMATRIX,STATUS_NOWRITEPERM,STATUS_DISKFULL,
    STATUS_NOMATLIST,STATUS_INVALIDMATLIST,STATUS_VARMATSIZE,STATUS_NOMAINHEADER,
    STATUS_NOSUBHEADER, STATUS_NOMATRIX, STATUS_UNSUPPORTEDAXIALCOMP,
    STATUS_NOIMGDATAFILE, STATUS_NOHEADERFILE, STATUS_INVALIDHEADER,
    STATUS_NOIMGDATA, STATUS_NOSIFDATA, STATUS_WRONGSIFDATA, 
    STATUS_CANTWRITEIMGFILE, STATUS_CANTWRITEHEADERFILE, STATUS_WRONGFILETYPE, 
    STATUS_CANNOTERASE, STATUS_CANNOTREAD, STATUS_CANNOTWRITE,
    STATUS_UNSUPPORTEDPOLARMAP, STATUS_INVALIDPOLARMAP};
/******************************************************************************/
extern int IMG_TEST;
/******************************************************************************/
typedef struct {
  /** [1..dimx] */
  int x;
  /** [1..dimy] */
  int y;
  /** [1..dimz] */
  int z;
  /** [1..dimt] */
  int f;
} IMG_PIXEL;
typedef struct {
  /** [1..dimx] */
  int x1, x2;
  /** [1..dimy] */
  int y1, y2;
  /** [1..dimz] */
  int z1, z2;
  /** [1..dimt] */
  int f1, f2;
} IMG_RANGE;
/******************************************************************************/
  
/******************************************************************************/
/**
   4D image data structure
 */
typedef struct {

  /*
   *  State of image
   */
  /** Image status (note that this is different from errstatus below):
   *  IMG_STATUS_UNINITIALIZED, IMG_STATUS_INITIALIZED,
   *  IMG_STATUS_OCCUPIED, IMG_STATUS_ERROR   */
  char status;
  /** Pointer to _imgStatusMessage, describing current status */
  const char *statmsg;

  /*
   *  Information on the study
   */
  /** for calibration units see imgUnit() in img.c */
  char unit;
  /** Calibration factor (included in pixel values) */
  float calibrationFactor;
  /** study identification code, i.e. (consequental) study number */
  char studyNr[MAX_STUDYNR_LEN+1];
  /** patient name */
  char patientName[32];
  /** patient id, e.g. 311206-123H */
  char patientID[16];
  /** Name of radiopharmaceutical */
  char radiopharmaceutical[32];
  /** Half-life of isotope (sec) */
  float isotopeHalflife;
  /** Decay correction: IMG_DC_UNKNOWN (0), IMG_DC_CORRECTED (1),
   *  IMG_DC_NONCORRECTED (2) */
  char decayCorrection;
  /** Branching fraction (included in pixel values and in calibrationFactor) */
  float branchingFraction;
  /** Scan start time and date */
  time_t scanStart;
  /** Patient orientation (see ECAT 7.2 format) */
  int orientation;
  /** User process code (which may contain valid study number) */
  char userProcessCode[11];
  /** Study description (currently free text field for user to fill) */
  char studyDescription[32];

  /*
   *  Information on the image
   */
  /** IMG_TYPE_IMAGE, IMG_TYPE_RAW */
  char type;
  /** Reconstruction zoom factor */
  float zoom;
  /** Scanner axial FOV (mm) */
  float axialFOV;
  /** Scanner transaxial FOV (mm) */
  float transaxialFOV;
  /** Scanner sample distance (mm) */
  float sampleDistance;
  /** Pixel size (mm) */
  float sizex;
  /** Pixel size (mm) */
  float sizey;
  /** Pixel size (mm) */
  float sizez;
  /** Gaps between pixels in x direction (mm); negative value means overlap */
  float gapx;
  /** Gaps between pixels in y direction (mm); negative value means overlap */
  float gapy;
  /** Gaps between pixels in z direction (mm); negative value means overlap */
  float gapz;
  /** Image resolution in x direction (mm) */
  float resolutionx;
  /** Image resolution in y direction (mm) */
  float resolutiony;
  /** Image resolution in z direction (mm) */
  float resolutionz;
  /** Saved file-format specific data type; default 0 is always ok */
  int _dataType;
  /** File format: IMG_UNKNOWN, IMG_E63, IMG_E7, IMG_E7_2D, ...
      default 0 is always ok */
  int _fileFormat;
  /** Scanner type */
  int scanner;
  /** Modality */
  int modality;
  /** XFrom codes Q and S (as in NIfTI-1) */
  short int xform[2];
  /** Quaternion parameters b, c, d, and x, y, z shift, and affine transform
   *  parameters for the 1st, 2nd and 3rd row, x[4], y[4], and z[4]
   *  (as in NIfTI-1) */
  float quatern[18];

  /** IFT struct to store any additional header information */
  IFT ift;  

  /*
   *  Definitions for polar map
   */
  /** If data is not a polar map, polarmap_num_rings=0.
      If data is a polar map, polarmap_num_rings is between 1 and 
      MAX_POLARMAP_NUM_RINGS.
   */
  int polarmap_num_rings;
  /** Number of sectors in each polar map ring; defined only in polar map data.
      In case of polar map, dimz=dimy=1, dimx= sum of sectors in each ring.
      Polar map can contain dynamic data (time frames), in that case dimz>1.
   */
  int polarmap_sectors_per_ring[MAX_POLARMAP_NUM_RINGS];
  /** Polar map: fractional distance aong the long axis from base to apex,
   *  as defined in ECAT 7 header */
  float polarmap_ring_position[MAX_POLARMAP_NUM_RINGS];
  /** Polar map ring angle relative to long axis (90 degrees along cylinder,
   *  decreasing to 0 at the apex), as defined in ECAT 7 header */
  short int polarmap_ring_angle[MAX_POLARMAP_NUM_RINGS];
  /** Polar map start angle for rings, as defined in ECAT 7 header */
  short int polarmap_start_angle;

  /*
   *  Image data
   */
  /* Dimensions */
  /** Dimension of Time (t) */
  unsigned short int dimt;
  /** Dimension of Column (c/x) */
  unsigned short int dimx;
  /** Dimension of Row (r/y) */
  unsigned short int dimy;
  /** Dimension of Plane (p/z) */
  unsigned short int dimz;
  /** 'Hidden' pointers for actual data */
  float *_pxl;
  float **_col;
  float ***_row;
  float ****_pln;
  float *_header;
  /* Pointers for data to be used */
  /** Pointer to image data in matrix format m[plane][row][col][frame] */
  float ****m;
  /** Pointer to image data in matrix format plane[plane][row][col][frame] */
  float ****plane;
  /** Pointer to image data in matrix format row[row][col][frame] */
  float ***row;
  /** Pointer to image data in matrix format column[col][frame] */
  float **column;
  /** Pointer to image data in matrix format pixel[frame] */
  float *pixel;
  /** Plane numbers (numbers need not be contiguous with each other) */
  int *planeNumber;

  /*
   *  Frame times
   */
  /** Frame start time (sec) */
  float *start;
  /** Frame end time (sec) */
  float *end;
  /** Frame mid time (sec) */
  float *mid;

  /*
   *  Frame weights
   */
  /** Weights: 0=not weighted, 1=weighted, 2=also SD known */
  char isWeight;
  /** Frame weight factor */
  float *weight;
  /** Frame S.D. for weighting */
  float *sd;
  /** Prompts / frame */
  float *prompts;
  /** Randoms (delayed) / frame */
  float *randoms;
  
  /*
   *  Decay correction factors for each frame
   */
  /** Decay correction factor for each frame; included in pixel values */
  float *decayCorrFactor;

  /** Error status: STATUS_OK, STATUS_FAULT, STATUS_NOMEMORY, etc */
  int errstatus;         

} IMG;
/******************************************************************************/

/******************************************************************************/
/*
 *  Function definitions
 */
/* Initialization and memory handling of img data */
extern void imgInit(IMG *image);
extern void imgEmpty(IMG *image);
extern int imgAllocate(
  IMG *image, int planes, int rows, int columns, int frames);
extern int imgAllocateWithHeader(
  IMG *image, int planes, int rows, int columns, int frames, IMG *image_from);
extern int imgDup(IMG *img1, IMG *img2);
/* Retrieving image information */
extern char *imgStatus(int status_index);
extern void imgSetStatus(IMG *img, int status_index);
extern void imgInfo(IMG *image);
extern int imgCopyhdr(IMG *image1, IMG *image2);
extern int imgExtractRange(IMG *img1, IMG_RANGE r, IMG *img2);
extern int ifrReorder(IMG_RANGE *img_range);
extern int imgExistentTimes(IMG *img);
extern int imgExistentCounts(IMG *img);
/******************************************************************************/

/******************************************************************************/
#endif /* _IMG_H */
