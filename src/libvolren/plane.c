#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>

#include "volren/coord.h"
#include "volren/matrix.h"
#include "volren/volren.h"

/******************************************************************************
 * vr_init_plane - Initialize plane
 *
 * RETURNS: N/A
 */

void vr_init_plane(
    Plane *pl,
    float a,
    float b,
    float c,
    float d,
    int active,
    int loopDir
    )
{
    Coord n;

    n[0] = a;
    n[1] = b;
    n[2] = c;
    coord_normalize(n, n);

    pl->a       = n[0];
    pl->b       = n[1];
    pl->c       = n[2];
    pl->d       = d;
    pl->active  = active;
    pl->loopDir = loopDir;
}

/*******************************************************************************
 * find_points_on_plane - Determin 3 independet point on plane defined by n & d
 *
 * RETURNS: N/A
 */

static void find_points_on_plane(
    Coord n,
    float d,
    Coord p1,
    Coord p2,
    Coord p3
    )
{
    Coord v1, v2;

    /* First point is the proj of the origin along the normal into the plane */
    p1[0] = n[0] * (-d);
    p1[1] = n[1] * (-d);
    p1[2] = n[2] * (-d);

    /* Find a vector indenpendent to the normal */
    if ((fabs(n[0]) < 0.01) && (fabs(n[1]) < 0.01))
    {
        /* Normal points mostly towards z, choose x axis */
        v1[0] = 1.0 * ((n[2] < 0) ? -1.0 : 1.0);
        v1[1] = 0.0;
        v1[2] = 0.0;

    }
    else
    {
        /* Normal points away from z, rotate normal about z */
        v1[0] =  n[1];
        v1[1] = -n[0];
        v1[2] =  n[2];
    }

    /* Cross the normal and indepent vector to get an orthogonal vector */
    coord_cross_product(n, v1, v2);

    /* Cross the normal and the orthogonal vector to get a second one */
    coord_cross_product(n, v2, v1);

    /* The second and third points are just the first offset by the 
       computed orthogonal vectors */
    coord_add(p1, v1, p2);
    coord_add(p1, v2, p3);
}

/******************************************************************************
 * vr_define_clip_planes - Flush clipping planes to renderer
 *
 * RETURNS: N/A
 */

void vr_define_clip_planes(
    VRState *state,
    float ucp[][4]
    )
{
    int i, j;
    Coord p1, p2, p3, n;
    float w1, w2, w3, d;

    GLdouble cp[MAX_CLIP_PLANES][4];

    for (i = 0; i < state->planeData->nPlanes; i++)
    {
        n[0] = state->planeData->plane[i].a;
        n[1] = state->planeData->plane[i].b;
        n[2] = state->planeData->plane[i].c;

        /* Transform the plane normal by the world rotation */
        coord_transform(n, state->view->invRotMat, n);

        d = state->planeData->plane[i].d;

        /* Define the clip plane */
        cp[i][0] = n[0];
        cp[i][1] = n[1];
        cp[i][2] = n[2];
        cp[i][3] = d;

        glClipPlane(GL_CLIP_PLANE0 + i, cp[i]);

        /* Find three independent points on the plane */
        n[0] = state->planeData->plane[i].a;
        n[1] = state->planeData->plane[i].b;
        n[2] = state->planeData->plane[i].c;
        find_points_on_plane(n, d, p1, p2, p3);

        /* Transform the points and do the homogenous divide */
        coord_transw(p1, state->view->WTSMat, w1);
        coord_transw(p2, state->view->WTSMat, w2);
        coord_transw(p3, state->view->WTSMat, w3);
        coord_transform(p1, state->view->WTSMat, p1);
        coord_transform(p2, state->view->WTSMat, p2);
        coord_transform(p3, state->view->WTSMat, p3);
        coord_div(p1, w1, p1);
        coord_div(p2, w2, p2);
        coord_div(p3, w3, p3);

        /* Create two vectors from the three points */
        coord_sub(p2, p1, p2);
        coord_sub(p3, p1, p3);

        /* The important vector is the cross product of two transfrmd vectors */
        coord_cross_product(p2, p3, p1);

        /* Determine direction */
        if (p1[2] < 0)
        {
            state->planeData->plane[i].facing = PLANE_FACING_AWAY;
        }
        else
        {
            state->planeData->plane[i].facing = PLANE_FACING_TOWARDS;
        }
    }

    if (ucp != NULL)
    {
        for (i = 0; i < state->planeData->nPlanes; i++)
        {
            for (j = 0; j < 4; j++)
            {
	        ucp[i][j] = (float) cp[i][j];
            }
        }
    }
}

/*******************************************************************************
 * vr_enable_active_clip_planes - Enable all active clip planes
 *
 * RETURNS: N/A
 */

void vr_enable_active_clip_planes(
    VRState *state,
    int exclude
    )
{
    int i;

    for (i = 0; i < state->planeData->nPlanes; i++)
    {
        if (state->planeData->plane[i].active && (i != exclude))
        {
            glEnable(GL_CLIP_PLANE0 + i);
        }
        else
        {
            glDisable(GL_CLIP_PLANE0 + i);
        }
    }
}

/*******************************************************************************
 * vr_disable_all_clip_planes - Disable all clip planes
 *
 * RETURNS: N/A
 */

void vr_disable_all_clip_planes(
    VRState *state
    )
{
    int i;

    for (i = 0; i < state->planeData->nPlanes; i++)
    {
        glDisable(GL_CLIP_PLANE0 + i);
    }
}

