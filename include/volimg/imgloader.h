#ifndef IMGLOADER_H
#define IMGLOEADER_H

#include "volimg/img.h"

class ImgLoader
{
public:
    static Img* open(const char *fname);
};

#endif

