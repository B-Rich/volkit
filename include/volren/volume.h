#ifndef VR_VOLUME_H
#define VR_VOLUME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volren/matrix.h"
#include "volren/brick.h"

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

struct _VRState;

/*******************************************************************************
 * vr_create_volume - Create volume data
 *
 * RETURNS: Pointer to volume data or NULL
 */

VRVolumeData* vr_create_volume(
    int xRes,
    int yRes,
    int zRes,
    int nxBricks,
    int nyBricks,
    int nzBricks,
    int drawInterp
    );

/*******************************************************************************
 * vr_init_volume - Initialize volume data
 *
 * RETURNS: N/A
 */

void vr_init_volume(
    VRVolumeData *vd,
    int drawInterp
    );

/*******************************************************************************
 * vr_delete_volume - Delete volume data
 *
 * RETURNS: N/A
 */

void vr_delete_volume(
    VRVolumeData *vd
    );

/*******************************************************************************
 * vr_render_volumes - Render volumes
 *
 * RETURNS: N/A
 */

void vr_render_volumes(
    struct _VRState *state,
    VRVolumeData *volumes,
    int nVolumes
    );

#ifdef __cplusplus
}
#endif

#endif

