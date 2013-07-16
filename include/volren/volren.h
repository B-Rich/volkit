#ifndef VOLREN_H
#define VOLREN_H

#include "volren/brick.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int nxBricks, nyBricks, nzBricks;
} VRVolumeData;

/*******************************************************************************
 * render_brick - Render brick
 *
 * RETURNS: N/A
 */

void render_brick(
    VRVolumeData *vd,          /* in: Volume data */
    Brick *br,                 /* in: Brick to render */
    float cp[4]                /* in: Slice plane */
    );

#ifdef __cplusplus
}
#endif

#endif

