#ifndef VR_VIEW_H
#define VR_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volgeo/matrix.h"

#define VIEW_SLICE_FACTOR      1.0     /* Slices distance factor */

typedef struct
{
    int    slices;                     /* Number of slices per brick */
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

/*******************************************************************************
 * vr_init_view - Initialize view
 *
 * RETURNS: N/A
 */

void vr_init_view(
    VRView *view,
    int slices
    );

#ifdef __cplusplus
}
#endif

#endif

