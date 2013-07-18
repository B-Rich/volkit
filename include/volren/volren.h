#ifndef VOLREN_H
#define VOLREN_H

#include "volren/matrix.h"
#include "volren/brick.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float delta;                       /* Distance between two slices */
    Matrix WTSMat;                     /* World-to-screen matrix */
} VRView;

typedef struct
{
    VRView *view;                      /* View specific parameters */
} VRState;

typedef struct
{
    Matrix VTWMat;                     /* Volume-to-world matrix */
    Matrix VTRMat;                     /* Volume-to-rotated matrix */
    Brick **brick;                     /* Brick storage */
    Brick **sbrick;                    /* Sorted bricks */
    int nxBricks, nyBricks, nzBricks;  /* Number of brick in each direction */
    int nBricks;                       /* Total number of bricks */
} VRVolumeData;

/*******************************************************************************
 * render_brick - Render brick
 *
 * RETURNS: N/A
 */

void render_brick(
    VRState *state,
    VRVolumeData *vd,
    Brick *b,
    int direction
    );

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

