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
    float  delta;                      /* Distance between two slices */

    Matrix rotMat;                     /* Global rotation matrix */
    Matrix invRotMat;                  /* Inverted global rotation matrix */

    Matrix RTCMat;                     /* Rotated-to-camera matrix */
    Matrix WTCMat;                     /* World-to-camera matrix */
    Matrix CTWMat;                     /* Camera-to-world matrix */
    Matrix CTSMat;                     /* Camera-to-screen matrix */
    Matrix STCMat;                     /* Screen-to-camera matrix */
    Matrix WTSMat;                     /* World-to-screen matrix */
} VRView;

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

typedef struct _VRVolumeData
{
    int    xRes, yRes, zRes;             /* Volume resolution */
    float  xScl, yScl, zScl;             /* Voxel spacing */

    int    drawInterp;                   /* Draw interpolated textures */

    float  xTrn, yTrn, zTrn;             /* Volume translation */
    float  uxScl, uyScl, uzScl;          /* Volume scaling */

    Matrix rotMat;                       /* Global rotation matrix */
    Matrix invRotMat;                    /* Inverted global rotation matrix */

    Matrix VTWMat;                       /* Volume-to-world matrix */
    Matrix WTVMat;                       /* World-to-volume matrix */
    Matrix VTRMat;                       /* Volume-to-rotated matrix */
    Matrix RTVMat;                       /* Rotated-to-volume matrix */

    Brick  **brick;                      /* Brick storage */
    Brick  **sbrick;                     /* Sorted bricks */
    int    nxBricks, nyBricks, nzBricks; /* Number of bricks per axis */
    int    nBricks;                      /* Total number of bricks */
} VRVolumeData;

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

