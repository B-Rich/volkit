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
} VRView;

typedef struct
{
    VRView *view;                      /* View specific parameters */
} VRState;

typedef struct
{
    Matrix VTRMat;                     /* Volume-to-rotated matrix */
    int nxBricks, nyBricks, nzBricks;  /* Number of brick in each direction */
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

#ifdef __cplusplus
}
#endif

#endif

