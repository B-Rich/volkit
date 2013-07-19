#ifndef BRICK_H
#define BRICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/gl.h>
#include "volren/coord.h"

typedef struct
{
    float xOff, yOff, zOff;    /* Brick offset coordinates */
    int xRes, yRes, zRes;      /* Brick size in each dimension */
    Coord center;              /* Rotated location for brick */
    GLuint texId;              /* Texture id */
    float txScl, tyScl, tzScl; /* Texture scale */
    float txOff, tyOff, tzOff; /* Texture offset */
    GLvoid *data;              /* Texture data buffer */
} Brick;

struct _VRState;
struct _VRVolumeData;

/*******************************************************************************
 * render_brick - Render brick
 *
 * RETURNS: N/A
 */

void render_brick(
    struct _VRState *state,
    struct _VRVolumeData *vd,
    Brick *b,
    int direction
    );

#ifdef __cplusplus
}
#endif

#endif

