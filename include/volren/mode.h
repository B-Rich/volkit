#ifndef VR_MODE_H
#define VR_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int sliceMode;             /* Draw slice at clip plane */
} VRMode;

/*******************************************************************************
 * vr_init_mode - Initialize mode object
 *
 * RETURNS: N/A
 */

void vr_init_mode(
    VRMode *mode,
    int sliceMode
    );

#ifdef __cplusplus
}
#endif

#endif

