#ifndef PLANE_H
#define PLANE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float a, b, c, d;          /* Plane equation: ax + by + cz - d = 0 */
} Plane;

#ifdef __cplusplus
}
#endif

#endif

