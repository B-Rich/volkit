#ifndef VOLREN_H
#define VOLREN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volren/matrix.h"
#include "volren/brick.h"
#include "volren/plane.h"

#define MAX_CLIP_PLANES        6       /* Maximum number of clipping planes */

typedef struct
{
    float delta;                       /* Distance between two slices */

    Matrix rotMat;                     /* Global rotation matrix */
    Matrix invRotMat;                  /* Inverted global rotation matrix */

    Matrix WTSMat;                     /* World-to-screen matrix */
} VRView;

typedef struct
{
    Plane plane[MAX_CLIP_PLANES];      /* Clip planes */
    int nPlanes;                       /* Number of active clip planes */
} VRPlaneData;

typedef struct _VRState
{
    VRView *view;                      /* View specific parameters */
    VRPlaneData *planeData;            /* Clipping plane parameters */
} VRState;

typedef struct _VRVolumeData
{
    int drawInterp;                    /* Draw interpolated textures */

    Matrix rotMat;                     /* Global rotation matrix */
    Matrix invRotMat;                  /* Inverted global rotation matrix */

    Matrix VTWMat;                     /* Volume-to-world matrix */
    Matrix VTRMat;                     /* Volume-to-rotated matrix */

    Brick **brick;                     /* Brick storage */
    Brick **sbrick;                    /* Sorted bricks */
    int nxBricks, nyBricks, nzBricks;  /* Number of brick in each direction */
    int nBricks;                       /* Total number of bricks */
} VRVolumeData;

/*******************************************************************************
 * render_volume - Render volume
 *
 * RETURNS: N/A
 */

void render_volume(
    VRState *state,
    VRVolumeData *vd
    );

#ifdef __cplusplus
}
#endif

#endif

