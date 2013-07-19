#ifndef PLANE_H
#define PLANE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PLANE_FACING_AWAY      0
#define PLANE_FACING_TOWARDS   1

typedef struct
{
    float a, b, c, d;          /* Plane equation: ax + by + cz - d = 0 */
    float minD, maxD;          /* Plane extents */
    int facing;                /* The plane is facing the viewer */
    int active;                /* The plane is active */
    int loopDir;               /* Direction in which the plane is looping */
} Plane;

struct _VRState;

/******************************************************************************
 * define_clip_planes - Flush clipping planes to renderer
 *
 * RETURNS: N/A
 */

void define_clip_planes(
    struct _VRState *state,
    float *ucp
    );

/*******************************************************************************
 * enable_active_clip_planes - Enable all active clip planes
 *
 * RETURNS: N/A
 */

void enable_active_clip_planes(
    struct _VRState *state,
    int exclude
    );

/*******************************************************************************
 * disable_all_clip_planes - Disable all clip planes
 *
 * RETURNS: N/A
 */

void disable_all_clip_planes(
    struct _VRState *state
    );

#ifdef __cplusplus
}
#endif

#endif

