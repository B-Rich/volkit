#include <GL/gl.h>

#include "volsurf/polygonise.h"
#include "volsurf/volsurf.h"

#define MAX_TRIANGLES_PER_CUBE 10

/*******************************************************************************
 * vs_draw_iso_surface - Draw the isosurface facets
 *
 * RETURNS: N/A
 */

void vs_draw_iso_surface(
    Grid *grid,
    float isolevel
    )
{
    int i, j, k, ii, jj, kk;
    int t, ntri, index;
    GridCell cell;
    Triangle tri[MAX_TRIANGLES_PER_CUBE];
    long nxny;
    Coord normal;

    nxny = grid->nx * grid->ny;
    glBegin(GL_TRIANGLES);

    for (i = 0; i < grid->nx - 1; i += grid->rx)
    {
        if ((ii = i + grid->rx) >= grid->nx)
        {
            ii = grid->nx - 1;
        }

        for (j = 0; j < grid->ny - 1; j += grid->ry)
        {
            if ((jj = j + grid->ry) >= grid->ny)
            {
                jj = grid->ny - 1;
            }

            for (k = 0; k < grid->nz - 1; k += grid->rz)
            {
                if ((kk = k + grid->rz) >= grid->nz)
                {
                    kk = grid->nz - 1;
                }

                cell.p[0][0] = i  * grid->dx;
                cell.p[0][1] = j  * grid->dy;
                cell.p[0][2] = k  * grid->dz;
                index        = k  * nxny + j * grid->nx + i;
                cell.val[0]  = (float) grid->data[index];

                cell.p[1][0] = ii * grid->dx;
                cell.p[1][1] = j  * grid->dy;
                cell.p[1][2] = k  * grid->dz;
                index        = k  * nxny + j * grid->nx + ii;
                cell.val[1]  = (float) grid->data[index];

                cell.p[2][0] = ii * grid->dx;
                cell.p[2][1] = j  * grid->dy;
                cell.p[2][2] = kk * grid->dz;
                index        = kk * nxny + j * grid->nx + ii;
                cell.val[2]  = (float) grid->data[index];

                cell.p[3][0] = i  * grid->dx;
                cell.p[3][1] = j  * grid->dy;
                cell.p[3][2] = kk * grid->dz;
                index        = kk * nxny + j * grid->nx + i;
                cell.val[3]  = (float) grid->data[index];

                cell.p[4][0] = i  * grid->dx;
                cell.p[4][1] = jj * grid->dy;
                cell.p[4][2] = k  * grid->dz;
                index        = k  * nxny + jj * grid->nx + i;
                cell.val[4]  = (float) grid->data[index];

                cell.p[5][0] = ii * grid->dx;
                cell.p[5][1] = jj * grid->dy;
                cell.p[5][2] = k  * grid->dz;
                index        = k  * nxny + jj * grid->nx + ii;
                cell.val[5]  = (float) grid->data[index];

                cell.p[6][0] = ii * grid->dx;
                cell.p[6][1] = jj * grid->dy;
                cell.p[6][2] = kk * grid->dz;
                index        = kk * nxny + jj * grid->nx + ii;
                cell.val[6]  = (float) grid->data[index];

                cell.p[7][0] = i  * grid->dx;
                cell.p[7][1] = jj * grid->dy;
                cell.p[7][2] = kk * grid->dz;
                index        = kk * nxny + jj * grid->nx + i;
                cell.val[7]  = (float) grid->data[index];

                ntri = vs_polygonise_cube(&cell, isolevel, tri);
                for (t = 0; t < ntri; t++)
                {
                    coord_normal(tri[t].p[0], tri[t].p[1], tri[t].p[2], normal);
                    coord_normalize(normal, normal);
                    glNormal3fv(normal);
                    glVertex3fv(tri[t].p[0]);
                    glVertex3fv(tri[t].p[1]);
                    glVertex3fv(tri[t].p[2]);
                }

            } /* End for k */

        } /* End for j */

    } /* End for i */

    glEnd();
}

