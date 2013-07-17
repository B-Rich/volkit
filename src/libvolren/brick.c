#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include "cube.h"
#include "volren/matrix.h"
#include "volren/brick.h"
#include "volren/volren.h"

static Matrix BTRMat, RTTMat;

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
 * get_brick_vertices - Get brick vertices in transformed space
 *
 * RETURNS: N/A
 */

void get_brick_vertices(
    Brick *br,
    VRState *state,
    Coord cpt[8],
    VRVolumeData *vd
    )
{
    int i;

    matrix_copy(IdentityMatrix, 4, BTRMat);
    matrix_translate(1.0, 1.0, 1.0, BTRMat);
    matrix_scale(0.5, 0.5, 0.5, BTRMat);
    matrix_translate(br->xOff, br->yOff, br->zOff, BTRMat);
    matrix_scale(1.0 / (float) vd->nxBricks,
                 1.0 / (float) vd->nyBricks,
                 1.0 / (float) vd->nzBricks, BTRMat);
    matrix_scale(2.0, 2.0, 2.0, BTRMat);
    matrix_translate(-1.0, -1.0, -1.0, BTRMat);
    // TODO: matrix_mult_safe(BTRMat, vd->VTRMat, BTRMat);

    /* Transform unit cube */
    for (i = 0; i < 8; i++)
    {
        coord_transform(PlusMinusCube[i], BTRMat, cpt[i]);
    }

    /*
     * Create a transformation which undoes (inverts) the above transformation
     * so the texture coordinates live in the range [0.0, 1.0]
     */
    matrix_invert4(BTRMat, RTTMat);
    matrix_translate(1.0, 1.0, 1.0, RTTMat);
    matrix_scale(0.5, 0.5, 0.5, RTTMat);
    matrix_translate(br->txOff, br->tyOff, br->tzOff, RTTMat);
    matrix_scale(br->txScl, br->tyScl, br->tzScl, RTTMat);
}

/*******************************************************************************
 * draw_slices - Draw slices that make up the brick
 *
 * RETURNS: N/A
 */

static void draw_slices(
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

    // TODO: Do transformation here
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf((GLfloat *) RTTMat);
    glMatrixMode(GL_MODELVIEW);

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
 * render_brick - Render brick
 *
 * RETURNS: N/A
 */

void render_brick(
    VRState *state,
    VRVolumeData *vd,
    Brick *b,
    int direction
    )
{
    Coord cpt[8];

    get_brick_vertices(b, state, cpt, vd);

    draw_slices(b, state, cpt, direction);
}

