#include <stdlib.h>

#include "volren/view.h"

/*******************************************************************************
 * vr_init_view - Initialize view
 *
 * RETURNS: N/A
 */

void vr_init_view(
    VRView *view,
    int slices
    )
{
    view->slices = slices;
    view->delta  = VIEW_SLICE_FACTOR / view->slices;
    matrix_copy(IdentityMatrix, 4, view->rotMat);
    matrix_copy(IdentityMatrix, 4, view->invRotMat);

    // TODO: Check if this shall be done here
    matrix_copy(IdentityMatrix, 4, view->RTCMat);
    matrix_copy(IdentityMatrix, 4, view->CTSMat);
    matrix_copy(IdentityMatrix, 4, view->STCMat);
}

