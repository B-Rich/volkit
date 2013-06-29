#ifndef IMG_H
#define IMG_H

#define MAX_STUDYNR_LEN	80

/** Definitions for img status */
#define IMG_STATUS_UNINITIALIZED 0
#define IMG_STATUS_INITIALIZED   1
#define IMG_STATUS_OCCUPIED      2
#define IMG_STATUS_ERROR         3

/** Definitions for image type */
#define IMG_TYPE_IMAGE 1
#define IMG_TYPE_RAW   2

/** Definitions for file format */
#define IMG_UNKNOWN   0
#define IMG_E63       1
#define IMG_E7        2
#define IMG_E7_2D     3
#define IMG_ANA       11
#define IMG_ANA_L     12 /* little endian variant */
#define IMG_INTERFILE 21

/** Definition for scanner type */
#define SCANNER_UNKNOWN 0
#define SCANNER_ECAT931 12
#define SCANNER_ADVANCE 12096
#define SCANNER_HRPLUS 3
#define SCANNER_HRRT 4

#include "colormap.h"

class Img {

  /*
   * Pivate members
   */

private:

  /** Status strings */
  static char *statusMessage[];

  /* Dimensions */

  /** Dimension of Column (c/x) */
  unsigned short int dimx;

  /** Dimension of Row (r/y) */
  unsigned short int dimy;

  /** Dimension of Plane (p/z) */
  unsigned short int dimz;

  /** 'Hidden' pointers for actual data */
  float *_col;
  float **_row;
  float ***_pln;

  /*
   * Shareable members
   */

protected:

  /*
   *  State of image
   */

  /** Image status */
  char status;

  /** Pointer to statusMessage, describing current status */
  char *statmsg;

  /*
   *  Information on the study
   */

  /** units */
  char unit;

  /** study identification code, i.e. (consequental) study number */
  char studyNr[MAX_STUDYNR_LEN + 1];

  /** and patient name */
  char patientName[32];

  /** Name of radiopharmaceutical */
  char radiopharmaceutical[32];

  /** Half-life of isotope (sec) */
  float isotopeHalflife;

  /** Decay correction: 0=not corrected, 1=corrected */
  char decayCorrected;

  /** Scan start time and date */
  time_t scanStart;

  /** Patient orientation (see ECAT 7.2 format) */
  int orientation;

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

  /** Volxel size (mm) */
  float sizex, sizey, sizez;

  /** Saved data type; default 0 is always ok */
  int _dataType;

  /** File format: IMG_UNKNOWN, IMG_E63, IMG_E7, IMG_E7_2D, ...
      default 0 is always ok */
  int _fileFormat;

  /** Scanner type */
  int scanner;

  /*
   *  Image data
   */

  /* Pointers for data to be used */

  /** Pointer to image data in matrix format m[plane][row][col] */
  float ***m;

  /** Pointer to image data in matrix format plane[plane][row][col] */
  float ***plane;

  /** Pointer to image data in matrix format row[row][col] */
  float **row;

  /** Pointer to image data in matrix format column[col] */
  float *column;

  /** Plane numbers (numbers need not be consequential) */
  int *planeNumber;

  /*
   *  Frame times
   */

  /** Frame start time (sec) */
  float start;

  /** Frame end time (sec) */
  float end;

  /** Frame mid time (sec) */
  float mid;

  /*
   *  Decay correction factors for each frame
   */

  /** Decay correction factor for each frame; included in pixel values */
  float decayCorrFactor;
                              
  /*
   * Public interface
   */

public:

  Img();
  ~Img();

  void dealloc();
  int alloc(int planes, int rows, int columns);

  float getMax();
  void getRGBAData(
    unsigned long *buf,
    float lowLimit,
    float highLimit,
    ColorMap *cmap,
    unsigned char alpha = 255
  );
  int getDimx();
  int getDimy();
  int getDimz();
  char *getStatusMessage();
};
  
#endif

