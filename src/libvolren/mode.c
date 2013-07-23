#include <stdlib.h>

#include "volren/mode.h"

/*******************************************************************************
 * vr_init_mode - Initialize mode object
 *
 * RETURNS: N/A
 */

void vr_init_mode(
    VRMode *mode,
    int sliceMode
    )
{
    mode->sliceMode = sliceMode;
}

