#include <stdlib.h>

#include "volren/view.h"

/*******************************************************************************
 * create_view - Create view
 *
 * RETURNS: Pointer to view of NULL
 */

VRView* create_view(
    int slices
    )
{
    VRView *view;

    view = (VRView *) malloc(sizeof(VRView));
    if (view != NULL)
    {
        init_view(view, slices);
    }

    return view;
}

/*******************************************************************************
 * init_view - Initialize view
 *
 * RETURNS: N/A
 */

void init_view(
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

/*******************************************************************************
 * delete_view - Delete view
 *
 * RETURNS: N/A
 */

void delete_view(
    VRView *view
    )
{
    free(view);
}

