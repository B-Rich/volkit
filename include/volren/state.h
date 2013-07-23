#ifndef VR_STATE_H
#define VR_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volren/matrix.h"
#include "volren/view.h"
#include "volren/mode.h"
#include "volren/plane.h"

#define MAX_CLIP_PLANES        6       /* Maximum number of clipping planes */

typedef struct
{
    Plane plane[MAX_CLIP_PLANES];      /* Clip planes */
    int   nPlanes;                     /* Number of active clip planes */
} VRPlaneData;

typedef struct _VRState
{
    VRView      *view;                 /* View specific parameters */
    VRMode      *mode;                 /* Rendering mode parameters */
    VRPlaneData *planeData;            /* Clipping plane parameters */
} VRState;

/*******************************************************************************
 * vr_create_state - Create renderer state object
 *
 * RETURNS: Pointer to state or NULL
 */

VRState* vr_create_state(
    int slices,
    int sliceMode,
    VRPlaneData *planeData
    );

/*******************************************************************************
 * vr_init_state - Initialize renderer state
 *
 * RETURNS: N/A
 */

void vr_init_state(
    VRState *state,
    int slices,
    int sliceMode,
    VRPlaneData *planeData
    );

/*******************************************************************************
 * vr_delete_state - Delete renderer state
 * 
 * RETURNS: N/A
 */
 
void vr_delete_state(
    VRState *state
    );

#ifdef __cplusplus
}
#endif

#endif

