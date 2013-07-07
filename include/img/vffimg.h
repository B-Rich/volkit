#ifndef VFFIMG_H
#define VFFIMG_H

#include "vff/vff.h"
#include "img/img.h"

class VffImg : public Img
{
public:
    int open(const char *fname);
    void close();
    int read(int t);
    int getFrameNr() { return frameNr; }

private:
    static const char *imgmsg[];

    int xSize, ySize, zSize;
    int frameNr, planeNr, pxlNr;

    VFF_mainheader main_header;

    int readMainHeader();
    int read8(int t);
    int read16(int t);
};

#endif
