#ifndef VR_VIEW_H
#define VR_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volren/matrix.h"

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
 * create_view - Create view
 *
 * RETURNS: Pointer to view of NULL
 */

VRView* create_view(
    int slices
    );

/*******************************************************************************
 * init_view - Initialize view
 *
 * RETURNS: N/A
 */

void init_view(
    VRView *view,
    int slices
    );

/*******************************************************************************
 * delete_view - Delete view
 *
 * RETURNS: N/A
 */

void delete_view(
    VRView *view
    );

#ifdef __cplusplus
}
#endif

#endif

