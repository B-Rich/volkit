#ifndef VG_CUBE_H
#define VG_CUBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "volgeo/coord.h"

extern const int CubeEdgeVertices[12][2];
extern const int CubeFaceEdges[6][4];
extern const int CubeEdgeFaces[12][2];
extern const Coord UnitCube[8];
extern const Coord PlusMinusCube[8];

#ifdef __cplusplus
}
#endif

#endif

