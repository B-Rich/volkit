#include <iostream>
#include <string.h>
#include <stdint.h>

extern "C" {
#include "vff/vff.h"
}

#include "img/vffimg.h"

const char *VffImg::imgmsg[] =
{
    "ok",                                // 0
    "fault in calling routine",          // 1
    "out of memory",                     // 2
    "cannot open file",                  // 3
    "unknown file format",               // 4
    "unsupported file type"              // 5
};

int VffImg::readMainHeader()
{
    int ret;

    // Read main header
    ret = vffReadMainheader(fp, &main_header);
    if(ret)
    {
        statmsg = imgmsg[4];
        return 1;
    }

    // Check for magic number
    if (strncmp(main_header.magic_number, VFF_MAGICNR, 4) != 0)
    {
        statmsg = imgmsg[4];
        return 1;
    }

    // Check if depth is supported
    if (main_header.bits != VFF_DEPTH8 &&
        main_header.bits != VFF_DEPTH16)
    {
        statmsg = imgmsg[5];
        return 1;
    }

    // Set file format
    _fileFormat = IMG_VFF;

    statmsg = imgmsg[0];

    return 0;
}

int VffImg::open(const char *fname)
{
    if(!fname)
    {
        statmsg = imgmsg[1];
        return 1;
    }

    // Open file for read
    fp = fopen(fname, "r");
    if (!fp)
    {
        statmsg = imgmsg[3];
        return 1;
    }
    fileOpen = true;

    // Read main header
    if (readMainHeader() > 0)
    {
        close();
        return 1;
    }

    xSize = main_header.u_size;
    ySize = main_header.w_size;
    zSize = main_header.v_size;
    pxlNr = xSize * ySize;
    planeNr = zSize;
    if (main_header.frames)
    {
        frameNr = main_header.frames;
    }
    else
    {
        frameNr = 1;
    }

    if (main_header.value[0])
    {
        setLimits(main_header.value[1], main_header.value[0]);
    }

    if (alloc(planeNr, xSize, ySize) > 0)
    {
        statmsg = imgmsg[2];
        close();
        return 1;
    }

    return 0;
}

void VffImg::close()
{
    // dealloc checks if image is occupied
    dealloc();

    if (fileOpen)
    {
        fclose(fp);
        fileOpen = false;
    }
}

int VffImg::read(int t)
{
    int ret;

    switch(main_header.bits)
    {
        case VFF_DEPTH8:
            ret = -1; //read8(t);
            break;

        case VFF_DEPTH16:
            ret = read16(t);
            break;

        default:
            ret = -1;
            break;
    }
}

int VffImg::read8(int t)
{
    int xi, yi, zi;
    uint8_t *data = 0, *ptr;
    long volSize = xSize * ySize * zSize;

    data = new uint8_t[volSize];
    if (data)
    {
        // Read data from file
        fseek(
            fp,
            main_header.header_size + sizeof(uint8_t) * volSize * t,
            SEEK_SET
            );
        int s = fread(data, sizeof(uint8_t), volSize, fp);

        // Copy matrix data through volume planes
        ptr = data;
        for(xi = 0; xi < xSize; xi++)
        {
            for(zi = 0; zi < zSize; zi++)
            {
                for(yi = 0; yi < ySize; yi++)
                {
                    uint8_t val = *ptr++;
                    m[zi][yi][xi] = float(val);
                } // End for yi
            } // End for zi
        } // End for xi

        delete data;
    }
    else
    {
        statmsg = imgmsg[3];
        return 1;
    }

    return 0;
}

int VffImg::read16(int t)
{
    int xi, yi, zi;
    uint16_t *data = 0, *ptr;
    long volSize = xSize * ySize * zSize;

    data = new uint16_t[volSize];
    if (data)
    {
        // Read data from file
        fseek(
            fp,
            main_header.header_size + sizeof(uint16_t) * volSize * t,
            SEEK_SET
            );
        int s = fread(data, sizeof(uint16_t), volSize, fp);

        // Copy matrix data through volume planes
        ptr = data;
        for(xi = 0; xi < xSize; xi++)
        {
            for(zi = 0; zi < zSize; zi++)
            {
                for(yi = 0; yi < ySize; yi++)
                {
                    uint16_t val = *ptr++;
                    m[zi][yi][xi] = float(val);
                } // End for yi
            } // End for zi
        } // End for xi

        delete data;
    }
    else
    {
        statmsg = imgmsg[3];
        return 1;
    }

    return 0;
}

