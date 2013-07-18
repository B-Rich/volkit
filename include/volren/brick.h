#ifndef BRICK_H
#define BRICK_H

#include <GL/gl.h>
#include "volren/coord.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float xOff, yOff, zOff;    /* Brick offset coordinates */
    int xRes, yRes, zRes;      /* Brick size in each dimension */
    Coord center;              /* Rotated location for brick */
    GLuint texId;              /* Texture id */
    float txScl, tyScl, tzScl; /* Texture scale */
    float txOff, tyOff, tzOff; /* Texture offset */
} Brick;

#ifdef __cplusplus
}
#endif

#endif

