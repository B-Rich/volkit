#ifndef IMG_H
#define IMG_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "img/colormap.h"

#define MAX_STUDYNR_LEN	80

// Definitions for img status
#define IMG_STATUS_UNINITIALIZED 0
#define IMG_STATUS_INITIALIZED   1
#define IMG_STATUS_OCCUPIED      2
#define IMG_STATUS_ERROR         3

// Definitions for image type
#define IMG_TYPE_IMAGE 1
#define IMG_TYPE_RAW   2

// Definitions for file format
#define IMG_UNKNOWN   0
#define IMG_E63       1
#define IMG_E7        2
#define IMG_E7_2D     3
#define IMG_ANA       11
#define IMG_ANA_L     12 // little endian variant
#define IMG_INTERFILE 21

// Definition for scanner type
#define SCANNER_UNKNOWN 0
#define SCANNER_ECAT931 12
#define SCANNER_ADVANCE 12096
#define SCANNER_HRPLUS 3
#define SCANNER_HRRT 4

class Img
{
public:
    enum Orientation
    {
        ORIENTATION_HORIZONTAL,
        ORIENTATION_SAGITTAL,
        ORIENTATION_CORONAL
    };

    Img();
    ~Img();

    virtual int open(const char *fname, int t) { return -1; }
    virtual void close() { }
    virtual int read(int t) { return -1; }
    virtual int getFrameNr() { return -1; }

    int getDimx() { return dimx; }
    int getDimy() { return dimy; }
    int getDimz() { return dimz; }

    int getWidth();
    int getHeight();
    int getDepth();

    void getData(
        uint32_t *buf,
        ColorMap *cmap,
        uint8_t alpha = 255
    );

    void setOrientation(Orientation o) { imgOrientation = o; }
    void setLimits(float low, float high);

    const char *getStatusMessage() { return statmsg; }

protected:
    // Memory management
    void dealloc();
    int alloc(int planes, int rows, int columns);

    // File status
    bool fileOpen;
    FILE *fp;

    // Image status
    char status;

    // Pointer to statusMessage, describing current status
    const char *statmsg;

    // units
    char unit;

    // study identification code, i.e. (consequental) study number
    char studyNr[MAX_STUDYNR_LEN + 1];

    // patient name
    char patientName[32];

    // Name of radiopharmaceutical
    char radiopharmaceutical[32];

    // Half-life of isotope (sec)
    float isotopeHalflife;

    // Decay correction: 0=not corrected, 1=corrected
    char decayCorrected;

    // Scan start time and date
    time_t scanStart;

    // Patient orientation (see ECAT 7.2 format)
    int orientation;

    // Information on the image
    // IMG_TYPE_IMAGE, IMG_TYPE_RAW
    char type;

    // Reconstruction zoom factor
    float zoom;

    // Scanner axial FOV (mm)
    float axialFOV;

    // Scanner transaxial FOV (mm)
    float transaxialFOV;

    // Scanner sample distance (mm)
    float sampleDistance;

    // Volxel size (mm)
    float sizex, sizey, sizez;

    // Saved data type; default 0 is always ok
    int _dataType;

    // File format: IMG_UNKNOWN, IMG_E63, IMG_E7, IMG_E7_2D, ...
    // default 0 is always ok
    int _fileFormat;

    // Scanner type
    int scanner;

    // Pointer to image data in matrix format m[plane][row][col]
    float ***m;

    // Pointer to image data in matrix format plane[plane][row][col]
    float ***plane;

    // Pointer to image data in matrix format row[row][col]
    float **row;

    // Pointer to image data in matrix format column[col]
    float *column;

    // Plane numbers (numbers need not be consequential)
    int *planeNumber;

    // Frame start time (sec)
    float start;

    // Frame end time (sec)
    float end;

    // Frame mid time (sec)
    float mid;

    // Decay correction factor for each frame; included in pixel values
    float decayCorrFactor;

    // Draw orientation
    Orientation imgOrientation;

    // Upper and lower limit to include when converting to image data
    float lowLimit, highLimit;

private:
    void getHorizontalData(
        uint32_t *buf,
        ColorMap *cmap,
        uint8_t alpha = 255
    );

    void getSagittalData(
        uint32_t *buf,
        ColorMap *cmap,
        uint8_t alpha = 255
    );

    void getCoronalData(
        uint32_t *buf,
        ColorMap *cmap,
        uint8_t alpha = 255
    );

    // Status strings
    static const char *statusMessage[];

    // Dimensions
    unsigned short int dimx;
    unsigned short int dimy;
    unsigned short int dimz;

    // Pointers for actual data
    float *_col;
    float **_row;
    float ***_pln;
};
  
#endif

