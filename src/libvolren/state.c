#include <stdlib.h>

#include "volren/state.h"

/*******************************************************************************
 * create_state - Create renderer state object
 *
 * RETURNS: Pointer to state or NULL
 */

VRState* create_state(
    int slices,
    int sliceMode,
    VRPlaneData *planeData
    )
{
    VRState *state;

    /* Allocate continious block for state members */
    state = (VRState *) malloc(sizeof(VRState) +
                               sizeof(VRView) +
                               sizeof(VRMode) +
                               sizeof(VRPlaneData));
    if (state != NULL)
    {
        /* Setup pointer into memory for each member */
        state->view      = (VRView *)      &state[1];
        state->mode      = (VRMode *)      &state->view[1];
        state->planeData = (VRPlaneData *) &state->mode[1];

        /* Initialize members */
        init_state(state, slices, sliceMode, planeData);
    }

    return state;
}

/*******************************************************************************
 * init_state - Initialize renderer state
 *
 * RETURNS: N/A
 */

void init_state(
    VRState *state,
    int slices,
    int sliceMode,
    VRPlaneData *planeData
    )
{
    int i;

    init_view(state->view, slices);
    init_mode(state->mode, sliceMode);
    if (planeData != NULL)
    {
        state->planeData->nPlanes = planeData->nPlanes;
        for (i = 0; i < planeData->nPlanes; i++)
        {
            init_plane(&state->planeData->plane[i],
                       planeData->plane[i].a,
                       planeData->plane[i].b,
                       planeData->plane[i].c,
                       planeData->plane[i].d,
                       planeData->plane[i].active,
                       planeData->plane[i].loopDir);
        }
    }
    else
    {
        for (i = 0; i < MAX_CLIP_PLANES; i++)
        {
            init_plane(&state->planeData->plane[i], 0.0, 0.0, 0.0, 0.0, 0, 0);
        }
    }
}

/*******************************************************************************
 * delete_state - Delete renderer state
 *
 * RETURNS: N/A
 */

void delete_state(
    VRState *state
    )
{
    free(state);
}

