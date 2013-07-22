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
 * create_mode - Create mode object
 *
 * RETURNS: Pointer to mode of NULL
 */

VRMode* create_mode(
    int sliceMode
    );

/*******************************************************************************
 * init_mode - Initialize mode object
 *
 * RETURNS: N/A
 */

void init_mode(
    VRMode *mode,
    int sliceMode
    );

/*******************************************************************************
 * init_delete - Delete mode object
 *
 * RETURNS: N/A
 */

void delete_mode(
    VRMode *mode
    );

#ifdef __cplusplus
}
#endif

#endif

