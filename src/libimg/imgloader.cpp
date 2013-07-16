#include <string>

#include "img/vffimg.h"
#include "img/ecat7img.h"
#include "img/imgloader.h"

Img* ImgLoader::open(const char *fname)
{
    Img *img = 0;

    std::string filename(fname);
    size_t pos = filename.find_last_of(".");
    std::string ext = filename.substr(pos + 1);
    if (ext.compare("vff") == 0)
    {
        img = new VffImg;
    }
    else if (ext.compare("v") == 0)
    {
        img = new Ecat7Img;
    }

    int result = img->open(fname);
    if (result != 0)
    {
        delete img;
        img = 0;
    }

    return img;
}

