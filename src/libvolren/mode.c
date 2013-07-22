#include <stdlib.h>

#include "volren/mode.h"

/*******************************************************************************
 * create_mode - Create mode object
 *
 * RETURNS: Pointer to mode of NULL
 */

VRMode* create_mode(
    int sliceMode
    )
{
    VRMode *mode;

    mode = (VRMode *) malloc(sizeof(VRMode));
    if (mode != NULL)
    {
        init_mode(mode, sliceMode);
    }

    return mode;
}

/*******************************************************************************
 * init_mode - Initialize mode object
 *
 * RETURNS: N/A
 */

void init_mode(
    VRMode *mode,
    int sliceMode
    )
{
    mode->sliceMode = sliceMode;
}

/*******************************************************************************
 * init_delete - Delete mode object
 *
 * RETURNS: N/A
 */

void delete_mode(
    VRMode *mode
    )
{
    free(mode);
}

