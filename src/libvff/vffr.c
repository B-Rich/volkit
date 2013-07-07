/* VFF reader
 * Based on Francois Angot algorithm 1996-12-16
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vff/vff.h"

static int vffReadValue(FILE *fp, char *line)
{
    int i = 0;
    char c = fgetc(fp);

    while (c != ';')
    {
        line[i++] = c;
        c = fgetc(fp);
    }
    line[i] = '\0';

    return i;
}

static int vffReadLine(FILE *fp, char *line)
{
    int i = 0;
    char c = fgetc(fp);

    while (c != '\n')
    {
        line[i++] = c;
        c = fgetc(fp);
    }
    line[i] = '\0';

    return i;
}

int vffReadMainheader(FILE *fp, VFF_mainheader *h)
{
    char line[256], c;
    int i, i1, i2, i3;
    float f1, f2, f3;

    /* Clear header */
    memset(h, 0, sizeof(VFF_mainheader));

    /* Read magic number including newline */
    fread(line, sizeof(char), 5, fp);

    /* Copy and check magic number */
    strncpy(h->magic_number, line, 4);
    if (strncmp(h->magic_number, VFF_MAGICNR, 4))
    {
        return 1;
    }

    c = fgetc(fp);
    while(c != '\f')
    {
        /* Read key until equal sign */
        i = 0;
        while (c != '=')
        {
            line[i++] = c;
            c = fgetc(fp);
        }
        line[i] = '\0';

        /* Select key */
        if (!strcmp(line, "rank"))
        {
            vffReadValue(fp, line);
            h->rank = atoi(line);
        }
        else if (!strcmp(line, "type"))
        {
            vffReadValue(fp, line);
            if (!strcmp(line, "raster"))
            {
                h->type = VFF_TYPE_RASTER;
            }
            else
            {
                h->type = VFF_TYPE_UNKNOWN;
            }
        }
        else if (!strcmp(line, "format"))
        {
            vffReadValue(fp, line);
            if (!strcmp(line, "slice"))
            {
                h->format = VFF_FORMAT_SLICE;
            }
            else
            {
                h->format = VFF_FORMAT_UNKNOWN;
            }
        }
        else if (!strcmp(line, "size"))
        {
            vffReadValue(fp, line);
            sscanf(line, "%d %d %d", &i1, &i2, &i3);
            h->u_size = i1;
            h->v_size = i2;
            h->w_size = i3;
        }
        else if (!strcmp(line, "origin"))
        {
            vffReadValue(fp, line);
            sscanf(line, "%g %g %g", &f1, &f2, &f3);
            h->x0_origin = f1;
            h->y0_origin = f2;
            h->z0_origin = f3;
        }
        else if (!strcmp(line, "extent"))
        {
            vffReadValue(fp, line);
            sscanf(line, "%g %g %g", &f1, &f2, &f3);
            h->x1_extent = f1;
            h->y1_extent = f2;
            h->z1_extent = f3;
        }
        else if (!strcmp(line, "rawsize"))
        {
            vffReadValue(fp, line);
            h->rawsize = atoi(line);
        }
        else if (!strcmp(line, "bands"))
        {
            vffReadValue(fp, line);
            h->bands = atoi(line);
        }
        else if (!strcmp(line, "bits"))
        {
            vffReadValue(fp, line);
            h->bits = atoi(line);
        }
        else if (!strcmp(line, "title"))
        {
            vffReadValue(fp, line);
            strncpy(h->title, line, 32);
        }
        else if (!strcmp(line, "value"))
        {
            vffReadValue(fp, line);
            sscanf(line, "%g %g", &f1, &f2);
            h->value[0] = f1;
            h->value[1] = f2;
        }
        else if (!strcmp(line, "frames"))
        {
            vffReadLine(fp, line);
            h->frames = atoi(line);
            h->frame = (VFF_frameheader *) malloc(
                sizeof(VFF_frameheader) * h->frames
                );
            if (h->frame == NULL)
            {
                return 1;
            }
            for (i = 0; i < h->frames; i++)
            {
                vffReadLine(fp, line);
                sscanf(line, "%g %g %g", &f1, &f2, &f3);
                h->frame[i].start_time = f1;
                h->frame[i].mean_time  = f2;
                h->frame[i].end_time   = f3;
                if (strchr(line, ';'))
                {
                    break;
                }
            }
        }
	else
        {
            vffReadValue(fp, line);
        }

        /* Consume whitespace */
        while (((c = fgetc(fp)) == ' ') || (c == '\n'));
    }
   
    /* Consume newlines */
    while (fgetc(fp) == '\n');
    h->header_size = ftell(fp);


    if (!h->rawsize)
    {
        h->rawsize = h->u_size * h->v_size * h->w_size * h->bands * (h->bits / 8);
    }

    return 0;
}

#ifdef MAIN
int main(int argc, char *argv[])
{
    FILE *fp;
    VFF_mainheader header;
    char *data;
    unsigned long size;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open file: %s\n", argv[1]);
        return 1;
    }

    if (vffReadMainheader(fp, &header) == 0)
    {
        vffPrintMainheader(&header, stdout);
        data = (char *) malloc(header.rawsize);
        size = fread(data, 1, header.rawsize, fp);
        printf("Read %ld byte(s)\n", size);
    }
    else
    {
        fprintf(stderr, "Invalid VFF file: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
#endif

