#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include "volgeo/cube.h"
#include "volgeo/matrix.h"
#include "volren/plane.h"
#include "volren/brick.h"
#include "volren/volren.h"

/*******************************************************************************
 * get_brick_vertices - Get brick vertices in transformed space
 *
 * RETURNS: N/A
 */

static void get_brick_vertices(
    Brick *br,
    VRState *state,
    Coord cpt[8],
    VRVolumeData *vd,
    Matrix RTTMat
    )
{
    int i;
    Matrix BTRMat;

    /* Locate brick cube at the proper location and rotate it */
    matrix_copy(IdentityMatrix, 4, BTRMat);
    matrix_translate(1.0, 1.0, 1.0, BTRMat);
    matrix_scale(0.5, 0.5, 0.5, BTRMat);
    matrix_translate(br->xOff, br->yOff, br->zOff, BTRMat);
    matrix_scale(1.0 / (float) vd->nxBricks,
                 1.0 / (float) vd->nyBricks,
                 1.0 / (float) vd->nzBricks, BTRMat);
    matrix_scale(2.0, 2.0, 2.0, BTRMat);
    matrix_translate(-1.0, -1.0, -1.0, BTRMat);
    matrix_mult_safe(BTRMat, vd->VTRMat, BTRMat);

    /* Transform unit cube */
    for (i = 0; i < 8; i++)
    {
        coord_transform(PlusMinusCube[i], BTRMat, cpt[i]);
    }

    /* Store invers transformation so texture coords are in range [0.0 1.0] */
    matrix_invert(BTRMat, RTTMat);
    matrix_translate(1.0, 1.0, 1.0, RTTMat);
    matrix_scale(0.5, 0.5, 0.5, RTTMat);
    matrix_translate(br->txOff, br->tyOff, br->tzOff, RTTMat);
    matrix_scale(br->txScl, br->tyScl, br->tzScl, RTTMat);
}

/*******************************************************************************
 * enable_brick - Enable brick texture
 *
 * RETURNS: N/A
 */

static void enable_brick(
    VRState *state,
    Brick *b
    )
{
    if (b->texId)
    {
        glEnable(GL_TEXTURE_3D);
    }
}

/*******************************************************************************
 * load_brick - Load brick into texture memory
 *
 * RETURNS: N/A
 */

static void load_brick(
    VRState *state,
    VRVolumeData *vd,
    Brick *b
    )
{
    if (b->texId)
    {
        glBindTexture(GL_TEXTURE_3D, b->texId);
        if (vd->drawInterp)
        {
            glTexParameteri(GL_TEXTURE_3D_EXT,
                            GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_3D_EXT,
                            GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_3D_EXT,
                            GL_TEXTURE_MIN_FILTER,
                            GL_NEAREST);
            glTexParameteri(GL_TEXTURE_3D_EXT,
                            GL_TEXTURE_MAG_FILTER,
                            GL_NEAREST);
        }
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA,
                     b->xRes, b->yRes, b->zRes, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, b->data);
    }

    enable_brick(state, b);
}

/*******************************************************************************
 * get_plane_cube_intersect - Get intersection polygon for plane and cube
 *
 * RETURNS: Number of polygon vertices for intersection
 */

static int get_plane_cube_intersect(
    float a,
    float b,
    float c,
    float d,
    Coord tl[8],
    Coord cpt[8]
    )
{
    int i, j, n;
    int span;
    int faces[6];
    int lastedge, lastface, face;
    float pleval, plevalnxt;
    float t1, t2;

    for (i = 0; i < 6; i++)
    {
        faces[i] = 0;
    }

    i = 0;
    n = 0;

    pleval = cpt[CubeEdgeVertices[0][0]][0] * a + 
             cpt[CubeEdgeVertices[0][0]][1] * b +
             cpt[CubeEdgeVertices[0][0]][2] * c + d;

    plevalnxt = cpt[CubeEdgeVertices[0][1]][0] * a + 
                cpt[CubeEdgeVertices[0][1]][1] * b +
                cpt[CubeEdgeVertices[0][1]][2] * c + d;

    /* If sign bits are different, endpoints of edge span slicing plane */
    span = signbit(pleval) ^ signbit(plevalnxt);

    while (i < 12 && !span) 
    {
        i++;

        pleval = cpt[CubeEdgeVertices[i][0]][0] * a + 
                 cpt[CubeEdgeVertices[i][0]][1] * b +
                 cpt[CubeEdgeVertices[i][0]][2] * c + d;

        plevalnxt = cpt[CubeEdgeVertices[i][1]][0] * a + 
                    cpt[CubeEdgeVertices[i][1]][1] * b +
                    cpt[CubeEdgeVertices[i][1]][2] * c + d;

        span = signbit(pleval) ^ signbit(plevalnxt);
    }

    if (i < 12)
    {
        face     = CubeEdgeFaces[i][0];
        lastface = CubeEdgeFaces[i][1];
        lastedge = -1;

        while (faces[lastface] < 2) 
        {
            j = 0;
            if (faces[face] > 1) 
            {
                if (faces[CubeEdgeFaces[lastedge][0]] < 2)
                {
                    face = CubeEdgeFaces[lastedge][0];
                }
                else
                {
                    face = CubeEdgeFaces[lastedge][1];
                }
            }

            while (faces[face] < 2)
            {
                i = CubeFaceEdges[face][j];

                pleval = cpt[CubeEdgeVertices[i][0]][0] * a + 
                         cpt[CubeEdgeVertices[i][0]][1] * b +
                         cpt[CubeEdgeVertices[i][0]][2] * c + d;

                plevalnxt = cpt[CubeEdgeVertices[i][1]][0] * a + 
                            cpt[CubeEdgeVertices[i][1]][1] * b +
                            cpt[CubeEdgeVertices[i][1]][2] * c + d;

                span = signbit(pleval) ^ signbit(plevalnxt);

                if ((lastedge != i) && span)
                {
                    if (plevalnxt == pleval)
                    {
                        fprintf(stderr,"GetPoly: will get divide by 0\n");
                    }

                    t1 = 1.0 / (plevalnxt - pleval);
                    t1 *= plevalnxt;
                    t2 = (1.0 - t1);

                    tl[n][0] = cpt[CubeEdgeVertices[i][0]][0] * t1 +
                               cpt[CubeEdgeVertices[i][1]][0] * t2;

                    tl[n][1] = cpt[CubeEdgeVertices[i][0]][1] * t1 +
                               cpt[CubeEdgeVertices[i][1]][1] * t2;

                    tl[n][2] = cpt[CubeEdgeVertices[i][0]][2] * t1 +
                               cpt[CubeEdgeVertices[i][1]][2] * t2;

                    lastedge = i;
                    faces[CubeEdgeFaces[i][0]]++;
                    faces[CubeEdgeFaces[i][1]]++;
                    n++;
                }
                j++;
            }
        }
    }

    return n;
}

/*******************************************************************************
 * render_slices - Render slices that make up the brick
 *
 * RETURNS: N/A
 */

static void render_slices(
    Brick *b,
    VRState *state,
    Coord cpt[8],
    int direction
    )
{
    int i, j, n;
    float d;
    float min_z, max_z;
    int slice_count;
    GLfloat p[8][3];

    /* Get slice distance from state */
    float delta = state->view->delta;
    float dz = delta * (float) direction;

    min_z =  HUGE;
    max_z = -HUGE;

    /* Find minimum and maximum z-coordinates */
    for (i = 0; i < 8; i++)
    {
        if (min_z > cpt[i][2])
        {
            min_z = cpt[i][2];
        }
        if (max_z < cpt[i][2])
        {
            max_z = cpt[i][2];
        }
    }

    min_z = floor(min_z / delta) * delta;
    max_z = floor(max_z / delta) * delta;

    /* For each slice */
    slice_count = 0;
    for (d = min_z; (direction == 1) ? (d <= max_z) : (d >= max_z); d += dz)
    {
        /* Get intersection polygon */
        n = get_plane_cube_intersect(0.0, 0.0, 1.0, -d, p, cpt);
        if (n > 2)
        {
            slice_count++;

            /* Draw slice */
            glBegin(GL_POLYGON);
            for (j = 0; j < n; j++)
            {
                glTexCoord3fv(p[j]);
                glVertex3fv(p[j]);
            }
            glEnd();
        }
    }
}

/*******************************************************************************
 * render_plane - Render slice on clipping plane
 *
 * RETURNS: N/A
 */

static void render_plane(
    VRState *state,
    Brick *b,
    Coord cpt[8],
    float cp[4]
    )
{
    int j, n;
    GLfloat p[8][3];

    /* Get vertices for plane */
    n = get_plane_cube_intersect(cp[0], cp[1], cp[2], cp[3], p, cpt);

    if (n > 2)
    {
        /* Draw plane */
        glBegin(GL_POLYGON);
        for (j = 0; j < n; j++)
        {
            glTexCoord3fv(p[j]);
            glVertex3fv(p[j]);
        }
        glEnd();
    }
}

/*******************************************************************************
 * vr_render_brick - Render brick
 *
 * RETURNS: N/A
 */

void vr_render_brick(
    VRState *state,
    VRVolumeData *vd,
    Brick *b,
    int direction
    )
{
    int i;
    Matrix RTTMat;
    Coord cpt[8];
    float cp[MAX_CLIP_PLANES][4];

    /* Get vertices for brick */
    get_brick_vertices(b, state, cpt, vd, RTTMat);

    /* Restore texture coordinate space to range [0.0 1.0] */
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf((GLfloat *) RTTMat);
    glMatrixMode(GL_MODELVIEW);

    /* If slice mode, get clipping plane */
    if (state->mode->sliceMode)
    {
        vr_define_clip_planes(state, cp);
    }

    /* Load brick into texture memory */
    load_brick(state, vd, b);

    /* Draw brick slices */
    render_slices(b, state, cpt, direction);

    if (state->mode->sliceMode)
    {
        glBlendFunc(GL_ONE, GL_ZERO);

        /* Draw all clipping planes */
        for (i = 0; i < state->planeData->nPlanes; i++)
        {
            /* Only draw plane if it is facing the viewer */
            if (state->planeData->plane[i].facing == PLANE_FACING_TOWARDS)
            {
                /* Enable all clipping planes except the current one */
                vr_enable_active_clip_planes(state, i);

                /* Draw current plane */
                render_plane(state, b, cpt, cp[i]);

                /* Disable all clipping planes */
                vr_disable_all_clip_planes(state);
            }
        }

        /* Re-enable all clipping planes */
        vr_enable_active_clip_planes(state, -1);

        /* Restore blending for volume rendering */
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

