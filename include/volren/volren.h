#ifndef VOLREN_H
#define VOLREN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volren/matrix.h"
#include "volren/view.h"
#include "volren/plane.h"
#include "volren/volume.h"

#define MAX_CLIP_PLANES        6       /* Maximum number of clipping planes */

typedef struct
{
    Plane plane[MAX_CLIP_PLANES];      /* Clip planes */
    int   nPlanes;                     /* Number of active clip planes */
} VRPlaneData;

typedef struct _VRState
{
    VRView      *view;                 /* View specific parameters */
    VRPlaneData *planeData;            /* Clipping plane parameters */
} VRState;

/*******************************************************************************
 * render_volumes - Render volumes
 *
 * RETURNS: N/A
 */

void render_volumes(
    VRState *state,
    VRVolumeData *volumes,
    int nVolumes
    );

#ifdef __cplusplus
}
#endif

#endif

