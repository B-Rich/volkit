#ifndef CUBE_H
#define CUBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volren/coord.h"

extern int edgevertices[12][2];
extern int faceedges[6][4];
extern int edgefaces[12][2];
extern coord UnitCube[8];
extern coord PlusMinusCube[8];

#ifdef __cplusplus
}
#endif

#endif

