/* VFF reader
 * Based on Francois Angot algorithm 1996-12-16
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vff/vff.h"

#define MAIN

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

int vffReadHeader(FILE *fp, VFF_header *h)
{
    char line[256], c;
    int i, val1, val2, val3;
   
    if ((fread(h->magic, sizeof(char), 5, fp) < 5) ||
        (strncmp(h->magic, "ncaa", 4)))
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
        if (!strcmp(line, "size"))
        {
            vffReadValue(fp, line);
            sscanf(line, "%d %d %d", &val1, &val2, &val3);
            h->u_size = val1;
            h->v_size = val2;
            h->w_size = val3;
         }
         else if (!strcmp(line, "bits"))
         {
            vffReadValue(fp, line);
            sscanf(line, "%d", &val1);
            h->bits = val1;
        }
	else
        {
            vffReadValue(fp, line);
        }

        /* Consume whitespace */
        while (((c = fgetc(fp)) == ' ') || (c == '\n'));
    }
   
    /* Consume formfeed */
    c = fgetc(fp);

    return 0;
}

#ifdef MAIN
int main(int argc, char *argv[])
{
    FILE *fp;
    VFF_header header;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open file: %s\n", argv[1]);
        return 1;
    }

    if (vffReadHeader(fp, &header) == 0)
    {
        printf("Size: %d, %d, %d\n",
               header.u_size, header.v_size, header.w_size);
        printf("Bits: %d\n", header.bits);
    }
    else
    {
        fprintf(stderr, "Invalid VFF file: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
#endif

