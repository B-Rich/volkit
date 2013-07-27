#include <GL/gl.h>

#include "volsurf/volsurf.h"

/*******************************************************************************
 * vs_draw_surface - Draw surface facets
 *
 * RETURNS: N/A
 */

void vs_draw_surface(
    int ntri,
    Triangle *tri
    )
{
    int i;

    glBegin(GL_TRIANGLES);
    for (i = 0; i < ntri; i++)
    {
        glNormal3fv(tri[i].n[0]);
        glVertex3fv(tri[i].p[0]);
        glNormal3fv(tri[i].n[1]);
        glVertex3fv(tri[i].p[1]);
        glNormal3fv(tri[i].n[2]);
        glVertex3fv(tri[i].p[2]);
    }
    glEnd();
}

