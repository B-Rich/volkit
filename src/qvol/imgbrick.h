#ifndef IMGBRICK
#define IMGBRICK

#include <stdint.h>

struct ImgBrick
{
    ImgBrick(int w, int h, int d, uint32_t *dat)
        : width(w),
          height(h),
          depth(d),
          data(dat) { }
    ~ImgBrick()
    {
        delete data;
    }

    int width, height, depth;
    uint32_t *data;
};

#endif
