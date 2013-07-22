#ifndef ECAT7IMG_H
#define ECAT7IMG_H

#include "ecat7.h"

#include "volimg/img.h"

class Ecat7Img : public Img
{
public:
    Ecat7Img();
    ~Ecat7Img();

    int open(const char *fname);
    void close();
    int read(int t);

    int getFrameNr() { return frameNr; }

private:
    static const char *imgmsg[];

    ECAT7_mainheader main_header;
    ECAT7_MATRIXLIST mlist;

    int xSize, ySize, zSize;
    int planeNr, frameNr, pxlNr;
    bool matlistLoaded;

    int readMainHeader();
    int readMatrixList();
    int calculateNr();
    int readSubHeader();
    void setFileFormat();
    void setImgUnit();
    void headerToImg();
};

#endif

