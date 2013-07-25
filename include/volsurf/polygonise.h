#ifndef VS_POLYGONISE_H
#define VS_POLYGONISE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volgeo/coord.h"

typedef struct {
    Coord p[8];
    Coord n[8];
    float val[8];
} GridCell;

typedef struct {
    Coord p[3];                /* Vertices */
    Coord c;                   /* Centroid */
    Coord n[3];                /* Normal   */
} Triangle;

/*******************************************************************************
 * vs_polygonise_cube -
 *
 * Given a grid cell and an isolevel, calculate the triangular
 * facets requied to represent the isosurface through the cell.
 * Return the number of triangular facets, the array "triangles"
 * will be loaded up with the vertices at most 5 triangular facets.
 * 0 will be returned if the grid cell is either totally above
 * of totally below the isolevel.
 *
 * RETURNS: Number of triangles or zero
 */

int vs_polygonise_cube(
    GridCell *g,
    float isolevel,
    Triangle *tri
    );

#ifdef __cplusplus
}
#endif

#endif

