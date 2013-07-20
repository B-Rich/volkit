#include <stdio.h>
#include <string.h>

#include "volren/matrix.h"
#include "volren/volren.h"

/*******************************************************************************
 * build_volume_matrix - Setup volume matrices
 *
 * RETURNS: N/A
 */

void build_volume_matrix(
    VRState *state,
    VRVolumeData *vd
    )
{
    /* Build the Volume-to-world matrix */
    matrix_copy(IdentityMatrix, 4, vd->VTWMat);
    matrix_scale(vd->xScl, vd->yScl, vd->zScl, vd->VTWMat);
    matrix_scale(vd->uxScl, vd->uyScl, vd->uzScl, vd->VTWMat);
    matrix_mult_safe(vd->VTWMat, vd->invRotMat, vd->VTWMat);
    matrix_translate(vd->xTrn, vd->yTrn, vd->zTrn, vd->VTWMat);
    matrix_invert(vd->VTWMat, vd->WTVMat);

    /* Build the Volume-to-rotated matrix */
    matrix_mult(vd->VTWMat, state->view->invRotMat, vd->VTRMat);
    matrix_invert(vd->VTRMat, vd->RTVMat);
}

/*******************************************************************************
 * sort_bricks - Sort bricks in correct order for rendering
 *
 * RETURNS: N/A
 */

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

/*******************************************************************************
 * sort_volume_bricks - Calculate brick location as sort for rendering
 *
 * RETURNS: N/A
 */

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
 * render_volumes - Render volumes
 *
 * RETURNS: N/A
 */

void render_volumes(
    VRState *state,
    VRVolumeData *volumes,
    int nVolumes
    )
{
    int i, j;
    VRVolumeData *vd;

    /* Setup clipping planes */
    define_clip_planes(state, NULL);
    enable_active_clip_planes(state, -1);

    /* Build world-to-camera matrix */
    matrix_mult(state->view->invRotMat,
                state->view->RTCMat,
                state->view->WTCMat);
    matrix_invert(state->view->WTCMat, state->view->CTWMat);

    /* Build world-to-screen matrix */
    matrix_mult(state->view->WTCMat,
                state->view->CTSMat,
                state->view->WTSMat);

    for (i = 0; i < nVolumes; i++)
    {
        /* Get current volume data */
        vd = &volumes[i];

        /* Build per volume matrices */
        build_volume_matrix(state, vd);

        /* Sort bricks */
        sort_volume_bricks(state, 1, vd);

        /* Render all bricks */
        /* TODO: Check why this has to be drawn in reverse order */
        for (j = vd->nBricks - 1; j >= 0; j--)
        {
            render_brick(state, vd, vd->sbrick[j], 1);
        }
    }
}

