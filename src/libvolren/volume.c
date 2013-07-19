#include <stdio.h>
#include <string.h>

#include "volren/volren.h"

static void sort_bricks(
    int l,
    int r,
    Brick **blist
    )
{
    int i, j;
    Brick *x, *w;

    i = l;
    j = r;
    x = blist[(l + r) / 2];

    do {
        while (blist[i]->center[2] < x->center[2]) i++;
        while (blist[j]->center[2] > x->center[2]) j--;

        if ( i <= j ) {
            w        = blist[i];
            blist[i] = blist[j];
            blist[j] = w;

            i++;
            j--;
        }
    } while (i <= j);

    if (l < j)
    {
        sort_bricks(l, j, blist);
    }
    if (i < r)
    {
        sort_bricks(i, r, blist);
    }
}

static void sort_volume_bricks(
    VRState *state,
    int direction,
    VRVolumeData *vd
    )
{
    int i;
    float w;
    Coord center;
    Brick *b;

    /* Copy the current bricks to the sorted brick list */
    memcpy(vd->sbrick, vd->brick, vd->nBricks * sizeof(Brick *));

    /* Rotate the brick centers into the eye coordinate system */
    for (i = 0; i < vd->nBricks; i++)
    {
        b = vd->sbrick[i];

        center[0] = (b->xOff + 0.5) * 2.0 / vd->nxBricks - 1.0;
        center[1] = (b->yOff + 0.5) * 2.0 / vd->nyBricks - 1.0;
        center[2] = (b->zOff + 0.5) * 2.0 / vd->nzBricks - 1.0;

        coord_transform(center, vd->VTWMat, center);
        coord_transz(center, state->view->WTSMat, b->center);
        coord_transw(center, state->view->WTSMat, w);
        b->center[2] /= -w * (float) direction;
    }

    /* Sort the bricks, smaller z to bigger z in the eye coordinate system */
    sort_bricks(0, vd->nBricks - 1, vd->sbrick);
}

/*******************************************************************************
 * render_volume - Render volume
 *
 * RETURNS: N/A
 */

void render_volume(
    VRState *state,
    VRVolumeData *vd
    )
{
    int i;

    /* Sort bricks */
    sort_volume_bricks(state, 1, vd);

    /* Render all bricks */
    /* TODO: Check why this has to be drawn in reverse order */
    for (i = vd->nBricks - 1; i >= 0; i--)
    {
        render_brick(state, vd, vd->sbrick[i], 1);
    }
}
