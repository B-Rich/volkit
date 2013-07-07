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
    "unknown file format"                // 4
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

#if 0
    // Check for magic number
    if(strncmp(main_header.magic_number, VFF_MAGICNR, 4) != 0)
    {
        statmsg = imgmsg[4];
        return 1;
    }
#endif

    // Check if file_type is supported
    // TODO

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

    dimx = main_header.u_size;
    dimy = main_header.w_size;
    dimz = main_header.v_size;
    pxlNr = dimx * dimy;
    planeNr = dimz;

    if (alloc(planeNr, dimx, dimy) > 0)
    {
        statmsg = imgmsg[2];
        close();
        return 1;
    }

    return 0;
}

void VffImg::close()
{
    if (fileOpen)
    {
        fclose(fp);
        fileOpen = false;
    }
}

int VffImg::read(int t)
{
    int xi, yi, zi;
    uint16_t *data = 0, *ptr;

    data = new uint16_t[dimx * dimy * dimz];
    if (data)
    {
        // Read data from file
        int s = fread(data, sizeof(uint16_t), dimx * dimy * dimz, fp);

        // Copy matrix data through volume planes
        ptr = data;
        for(xi = 0; xi < dimx; xi++)
        {
            for(zi = 0; zi < dimz; zi++)
            {
                for(yi = 0; yi < dimy; yi++)
                {
                    uint16_t val = *ptr;
                    m[zi][yi][xi] = float(val);
                    ptr++;
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

