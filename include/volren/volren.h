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
    VRVolumeData *vd,
    Brick *br,
    float cp[4]
    );

#ifdef __cplusplus
}
#endif

#endif

