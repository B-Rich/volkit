#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include "cube.h"
#include "volren/matrix.h"
#include "volren/brick.h"
#include "volren/volren.h"

/*******************************************************************************
 * get_plane_cube_intersect - Get intersection polygon for plane and cube
 *
 * RETURNS: Number of polygon vertices for intersection
 */

static int get_plane_cube_intersect(
    float a,                   /* in: Plane equation A (normal x) */
    float b,                   /* in: Plane equation B (normal y) */
    float c,                   /* in: Plane equation C (normal z) */
    float d,                   /* in: Plane equation D (offset) */
    coord tl[8],               /* out: Intersection vertices */
    coord cpt[8]               /* in: Cube vertices */
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

    pleval = cpt[edgevertices[i][0]][0] * a + 
             cpt[edgevertices[i][0]][1] * b +
             cpt[edgevertices[i][0]][2] * c + d;

    plevalnxt = cpt[edgevertices[i][1]][0] * a + 
                cpt[edgevertices[i][1]][1] * b +
                cpt[edgevertices[i][1]][2] * c + d;

    /* If sign bits are different, endpoints of edge span slicing plane */
    span = signbit(pleval) ^ signbit(plevalnxt);

    while (i < 12 && !span) 
    {
        i++;

        pleval = cpt[edgevertices[i][0]][0] * a + 
                 cpt[edgevertices[i][0]][1] * b +
                 cpt[edgevertices[i][0]][2] * c + d;

        plevalnxt = cpt[edgevertices[i][1]][0] * a + 
                    cpt[edgevertices[i][1]][1] * b +
                    cpt[edgevertices[i][1]][2] * c + d;

        span = signbit(pleval) ^ signbit(plevalnxt);
    }

    if (i < 12)
    {
        face = edgefaces[i][0];
        lastface = edgefaces[i][1];
        lastedge = -1;

        while (faces[lastface] < 2) 
        {
            j = 0;
            if (faces[face] > 1) 
            {
                if (faces[edgefaces[lastedge][0]] < 2)
                {
                    face = edgefaces[lastedge][0];
                }
                else
                {
                    face = edgefaces[lastedge][1];
                }
            }

            while (faces[face] < 2)
            {
                i = faceedges[face][j];

                pleval = cpt[edgevertices[i][0]][0] * a + 
                         cpt[edgevertices[i][0]][1] * b +
                         cpt[edgevertices[i][0]][2] * c + d;

                plevalnxt = cpt[edgevertices[i][1]][0] * a + 
                            cpt[edgevertices[i][1]][1] * b +
                            cpt[edgevertices[i][1]][2] * c + d;

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

                    tl[n][0] = cpt[edgevertices[i][0]][0] * t1 +
                               cpt[edgevertices[i][1]][0] * t2;

                    tl[n][1] = cpt[edgevertices[i][0]][1] * t1 +
                               cpt[edgevertices[i][1]][1] * t2;

                    tl[n][2] = cpt[edgevertices[i][0]][2] * t1 +
                               cpt[edgevertices[i][1]][2] * t2;

                    lastedge = i;
                    faces[edgefaces[i][0]]++;
                    faces[edgefaces[i][1]]++;
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
    VRVolumeData *vd,
    Brick *br,
    coord cpt[8]
    )
{
    static matrix BTRMat;
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

    // TODO: Texture matrix
}

/*******************************************************************************
 * draw_slice - Draw volume slice within a brick
 *
 * RETURNS: N/A
 */

static void draw_slice(
    Brick *b,                  /* in: Brick to draw */
    coord cpt[8],              /* in: Brick bounding box */
    float cp[4]                /* in: Slice plane */
    )
{
    int i, n;
    float p[8][3];

    n = get_plane_cube_intersect(cp[0], cp[1], cp[2], cp[3], p, cpt);
    if (n > 2)
    {
        glBegin(GL_POLYGON);
            for (i = 0; i < n; i++)
            {
#ifdef DEBUG
                coord_print(p[i]);
                printf("\n");
#endif
                glTexCoord3fv(p[i]);
                glVertex3fv(p[i]);
            }
        glEnd();
    }
}

/*******************************************************************************
 * render_brick - Render brick
 *
 * RETURNS: N/A
 */

void render_brick(
    VRVolumeData *vd,          /* in: Volume data */
    Brick *br,                 /* in: Brick to render */
    float cp[4]                /* in: Slice plane */
    )
{
    coord cpt[8];

    get_brick_vertices(vd, br, cpt);

    draw_slice(br, cpt, cp);
}

#ifdef MAIN
int main(void)
{
    int i, n;
    VRVolumeData vd;
    Brick br;
    float cp[4];

    /* Initialize volume data */
    vd.nxBricks = 1;
    vd.nyBricks = 1;
    vd.nzBricks = 1;

    /* Initialize brick */
    br.xOff = 0.0;
    br.yOff = 0.0;
    br.zOff = 0.0;

    /* Initialize plane */
    cp[0] = 0.707;
    cp[1] = 0.0;
    cp[2] = 0.707;
    cp[3] = 0.5;

    render_brick(&vd, &br, cp);

    return 0;
}
#endif

