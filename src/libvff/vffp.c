#include <stdio.h>

#include "vff/vff.h"

void vffPrintMainheader(VFF_mainheader *h, FILE *fp)
{
    int i;

    fprintf(fp, "rank := %d\n", h->rank);
    fprintf(fp, "type := %d\n", h->type);
    fprintf(fp, "format := %d\n", h->format);
    fprintf(fp, "u_size := %d\n", h->u_size);
    fprintf(fp, "v_size := %d\n", h->v_size);
    fprintf(fp, "w_size := %d\n", h->w_size);
    fprintf(fp, "x0_origin := %g\n", h->x0_origin);
    fprintf(fp, "y0_origin := %g\n", h->y0_origin);
    fprintf(fp, "z0_origin := %g\n", h->z0_origin);
    fprintf(fp, "x1_extent := %g\n", h->x1_extent);
    fprintf(fp, "y1_extent := %g\n", h->y1_extent);
    fprintf(fp, "z1_extent := %g\n", h->z1_extent);
    fprintf(fp, "rawsize := %ld\n", h->rawsize);
    fprintf(fp, "bands := %d\n", h->bands);
    fprintf(fp, "bits := %d\n", h->bits);
    fprintf(fp, "title := %s\n", h->title);
    fprintf(fp, "frames := %d\n", h->frames);

    for (i = 0; i < h->frames; i++)
    {
        fprintf(fp, "%d\t%g %g %g\n",
                i,
                h->frame[i].start_time,
                h->frame[i].mean_time,
                h->frame[i].end_time);
    }
}

