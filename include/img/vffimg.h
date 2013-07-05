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

private:
    static const char *imgmsg[];

    int dimx, dimy, dimz, frameNr, planeNr, pxlNr;

    VFF_mainheader main_header;

    int readMainHeader();
};

#endif
