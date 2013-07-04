#ifndef ECAT7IMG_H
#define ECAT7IMG_H

#include "ecat7.h"

#include "img/img.h"

class Ecat7Img : public Img
{

private:
    static char *imgmsg[];

    FILE *fp;

    ECAT7_mainheader main_header;
    ECAT7_MATRIXLIST mlist;

    int dimx, dimy, dimz, frameNr, planeNr, pxlNr;
    bool fileOpen, matlistLoaded, imgAllocated;

    int readMainHeader();
    int readMatrixList();
    int calculateNr();
    int readSubHeader();
    void setFileFormat();
    void setImgUnit();
    void headerToImg();

public:
    Ecat7Img();
    Ecat7Img(const char *fname);
    Ecat7Img(const char *fname, int t);
    ~Ecat7Img();

    int open(const char *fname);
    void close();
    int read(int t);

    int getFrameNr() { return frameNr; }
};

#endif

