#include <string.h>

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

    // Check for magic number
    if(strncmp(main_header.magic_number, VFF_MAGICNR, 4) != 0)
    {
        statmsg = imgmsg[4];
        return 1;
    }

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
    fp = fopen(fname, "rb");
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
    return 1;
}

