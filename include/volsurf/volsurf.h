#ifndef VOLSURF_H
#define VOLSURF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float *data;
    int    nx, ny, nz;
    float  dx, dy, dz;
    int    rx, ry, rz;
} Grid;

/*******************************************************************************
 * vs_draw_iso_surface - Draw the isosurface facets
 *
 * RETURNS: N/A
 */

void vs_draw_iso_surface(
    Grid *grid,
    float isolevel
    );

#ifdef __cplusplus
}
#endif

#endif

