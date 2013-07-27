#ifndef VS_POLYGONISE_H
#define VS_POLYGONISE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volgeo/coord.h"

typedef struct {
    float *data;
    int    nx, ny, nz;
    float  dx, dy, dz;
    int    rx, ry, rz;
} Grid;

typedef struct {
    Coord p[3];                /* Vertices */
    Coord n[3];                /* Normal   */
} Triangle;

/*******************************************************************************
 * vs_polygonise_grid - Generate iso surface from grid
 *
 * RETURNS: Number of triangles
 */

int vs_polygonise_grid(
    Grid *grid,
    float isolevel,
    int maxtri,
    Triangle *tri
    );

#ifdef __cplusplus
}
#endif

#endif

