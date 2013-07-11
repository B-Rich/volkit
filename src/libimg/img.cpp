#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>

extern "C" {
#include "ecat7.h"
#include "swap.h"
}

#include "img/img.h"

const char *Img::statusMessage[] =
{
    "ok",                                // 0
    "fault in calling routine",          // 1
    "out of memory"                      // 2
};

Img::Img()
{
    fileOpen = false;
    fp = 0;
    status = IMG_STATUS_INITIALIZED;
    statmsg = statusMessage[0];
    type = 0;
    unit = 0;
    zoom = 0.0;
    radiopharmaceutical[0] = (char) 0;
    isotopeHalflife = 0.0;
    decayCorrected = (char)0;
    unit = 0;
    scanStart = 0;
    orientation = 0;
    axialFOV = transaxialFOV = sampleDistance = 0.0;
    studyNr[0] = patientName[0] = (char) 0;
    sizex = sizey = sizez = 0;
    _dataType = 0;
    _fileFormat = 0;
    scanner = 0;
    dimx = dimy = dimz = 0;
    m = (float***) 0;
    column = (float*) 0;
    row = (float**) 0;
    plane = (float***) 0;
    planeNumber = (int*) 0;
    start = end = mid = 0;
    decayCorrFactor = 0;
    imgOrientation = ORIENTATION_HORIZONTAL;
    lowLimit = 0.0;
    highLimit = 65535.0;
    imgTransparency = TRANSPARENCY_NONE;
}

Img::~Img()
{
    dealloc();
}

void Img::dealloc()
{
    if(status >= IMG_STATUS_OCCUPIED)
    {

        if(_col)
        {
            delete _col;
        }

        if(_row)
        {
            delete _row;
        }

        if(_pln)
        {
            delete _pln;
        }

        if(dimz > 0)
        {
            delete planeNumber;
        }

        // Set variables
        Img();

        // Set status
        status = IMG_STATUS_INITIALIZED;
    }
}

int Img::alloc(int planes, int rows, int columns)
{
    int zi, ri;
    float **rptr, *cptr;

    // Check arguments
    statmsg = statusMessage[1];
    if(status == IMG_STATUS_UNINITIALIZED)
    {
        return 1;
    }

    if(planes < 1 || rows < 1 || columns < 1)
    {
        return 1;
    }

    if(status >= IMG_STATUS_OCCUPIED)
    {
        dealloc();
    }

    // Allocate memory for header data
    statmsg = statusMessage[2];
    planeNumber = new int[planes];
    if(!planeNumber)
    {
        return 1;
    }

    // Allocate memory for image data
    _pln = new float**[planes];
    if(!_pln)
    {
        delete planeNumber;
        return 1;
    }

    _row = new float*[planes * rows];
    if(!_row)
    {
        delete planeNumber;
        delete _pln;
        return 1;
    }

    _col = new float[planes * rows * columns];
    if(!_col)
    {
        delete planeNumber;
        delete _pln;
        delete _row;
        return 1;
    }

    // Set data pointers
    rptr = _row;
    cptr = _col;

    for(zi = 0; zi < planes; zi++)
    {
        _pln[zi] = rptr;
        for(ri = 0; ri < rows; ri++)
        {
            *rptr++ = cptr;
            cptr += rows;
        } // End for ri
    } // End for zi

    m = _pln;
    plane = _pln;
    column = _col;
    row = _row;

    // Ok
    dimz = planes;
    dimy = rows;
    dimx = columns;

    statmsg = statusMessage[0];
    status = IMG_STATUS_OCCUPIED;

    return 0;
}

void Img::setLimits(float low, float high)
{
    lowLimit = low;
    highLimit = high;
}

int Img::getWidth()
{
    int result;

    switch(imgOrientation)
    {
        case ORIENTATION_HORIZONTAL:
            result = dimx;
            break;

        case ORIENTATION_SAGITTAL:
            result = dimy;
            break;

        case ORIENTATION_CORONAL:
            result = dimx;
            break;
    }

    return result;
}

int Img::getHeight()
{
    int result;

    switch(imgOrientation)
    {
        case ORIENTATION_HORIZONTAL:
            result = dimy;
            break;

        case ORIENTATION_SAGITTAL:
            result = dimz;
            break;

        case ORIENTATION_CORONAL:
            result = dimz;
            break;
    }

    return result;
}

int Img::getDepth()
{
    int result;

    switch(imgOrientation)
    {
        case ORIENTATION_HORIZONTAL:
            result = dimz;
            break;

        case ORIENTATION_SAGITTAL:
            result = dimx;
            break;

        case ORIENTATION_CORONAL:
            result = dimy;
            break;
    }

    return result;
}

void Img::getData(
    uint32_t *buf,
    ColorMap *cmap,
    int x1, int x2,
    int y1, int y2,
    int z1, int z2
    )
{
    if (x2 < 0)
    {
        x2 = getWidth();
    }

    if (y2 < 0)
    {
        y2 = getHeight();
    }

    if (z2 < 0)
    {
        z2 = getDepth();
    }

    switch(imgOrientation)
    {
        case ORIENTATION_HORIZONTAL:
            getHorizontalData(buf, cmap, x1, x2, y1, y2, z1, z2);
            break;

        case ORIENTATION_SAGITTAL:
            getSagittalData(buf, cmap, x1, x2, y1, y2, z1, z2);
            break;

        case ORIENTATION_CORONAL:
            getCoronalData(buf, cmap, x1, x2, y1, y2, z1, z2);
            break;
    }
}

void Img::getHorizontalData(
    uint32_t *buf,
    ColorMap *cmap,
    int x1, int x2,
    int y1, int y2,
    int z1, int z2
    )
{
    uint8_t *img = (uint8_t *) buf;
    float k = 1.0 / (highLimit - lowLimit);
    float numColors = float(cmap->getNumColors() - 1);
    float result;

    for (int zi = z2 - 1; zi >= z1; zi--)
    {
        for (int yi = y2 - 1; yi >= y1; yi--)
        {
            for (int xi = x1; xi < x2; xi++)
            {
                float val = m[zi][yi][xi];
                if (val > lowLimit && val < highLimit)
                {
                    result = (val - lowLimit) * k;
                    if (result < 0.0) result = 0.0;
                    if (result > 1.0) result = 1.0;
                }
                else if (val > highLimit)
                {
                    result = 1.0;
                }
                else
                {
                    result = 0.0;
                }

                int out = int(numColors * result);
                *img++ = cmap->r(out);
                *img++ = cmap->g(out);
                *img++ = cmap->b(out);
                if (imgTransparency == TRANSPARENCY_NONE)
                {
                    *img++ = 255;;
                }
                if (imgTransparency == TRANSPARENCY_VOXEL)
                {
                    *img++ = uint8_t(out);
                }
            } // End for xi
        } // End for yi
    } // End for zi
}

void Img::getSagittalData(
    uint32_t *buf,
    ColorMap *cmap,
    int y1, int y2,
    int z1, int z2,
    int x1, int x2
    )
{
    uint8_t *img = (uint8_t *) buf;
    float k = 1.0 / (highLimit - lowLimit);
    float numColors = float(cmap->getNumColors() - 1);
    float result;

    for (int xi = x2 - 1; xi >= x1; xi--)
    {
        for (int zi = z2 - 1; zi >= z1; zi--)
        {
            for (int yi = y1; yi < y2; yi++)
            {
                float val = m[zi][yi][xi];
                if (val > lowLimit && val < highLimit)
                {
                    result = (val - lowLimit) * k;
                    if (result < 0.0) result = 0.0;
                    if (result > 1.0) result = 1.0;
                }
                else if (val > highLimit)
                {
                    result = 1.0;
                }
                else
                {
                    result = 0.0;
                }

                int out = int(numColors * result);
                *img++ = cmap->r(out);
                *img++ = cmap->g(out);
                *img++ = cmap->b(out);
                if (imgTransparency == TRANSPARENCY_NONE)
                {
                    *img++ = 255;;
                }
                if (imgTransparency == TRANSPARENCY_VOXEL)
                {
                    *img++ = uint8_t(out);
                }
            } // End for yi
        } // End for zi
    } // End for xi
}

void Img::getCoronalData(
    uint32_t *buf,
    ColorMap *cmap,
    int x1, int x2,
    int z1, int z2,
    int y1, int y2
    )
{
    uint8_t *img = (uint8_t *) buf;
    float k = 1.0 / (highLimit - lowLimit);
    float numColors = float(cmap->getNumColors() - 1);
    float result;

    for (int yi = y2 - 1; yi >= y1; yi--)
    {
        for (int zi = z2 - 1; zi >= z1; zi--)
        {
            for (int xi = x1; xi < x2; xi++)
            {
                float val = m[zi][yi][xi];
                if (val > lowLimit && val < highLimit)
                {
                    result = (val - lowLimit) * k;
                    if (result < 0.0) result = 0.0;
                    if (result > 1.0) result = 1.0;
                }
                else if (val > highLimit)
                {
                    result = 1.0;
                }
                else
                {
                    result = 0.0;
                }

                int out = int(numColors * result);
                *img++ = cmap->r(out);
                *img++ = cmap->g(out);
                *img++ = cmap->b(out);
                if (imgTransparency == TRANSPARENCY_NONE)
                {
                    *img++ = 255;;
                }
                if (imgTransparency == TRANSPARENCY_VOXEL)
                {
                    *img++ = uint8_t(out);
                }
            } // End for xi
        } // End for zi
    } // End for yi
}

