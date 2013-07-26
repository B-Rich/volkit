#ifndef VOLSURF_H
#define VOLSURF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volsurf/polygonise.h"

/*******************************************************************************
 * vs_draw_surface - Draw surface facets
 *
 * RETURNS: N/A
 */

void vs_draw_surface(
    int ntri,
    Triangle *tri
    );

#ifdef __cplusplus
}
#endif

#endif

