#ifndef COORD_H
#define COORD_H

#include <stdio.h>
#include "volren/scalar.h"
#include "volren/matrix.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef float Coord[3];

/*******************************************************************************
 * coord_dot_product - Calcualte dot product between vectors
 *
 * RETURNS: N/A
 */

#define coord_dot_product(c1, c2, Z)                                           \
{                                                                              \
    Z = (c1)[0] * (c2)[0] + (c1)[1] * (c2)[1] + (c1)[2] * (c2)[2];             \
}

/*******************************************************************************
 * coord_cross_product - Calcualte cross product between vectors
 *
 * RETURNS: N/A
 */

#define coord_cross_product(c1, c2, C3)                                        \
{                                                                              \
    (C3)[0] = (c1)[1] * (c2)[2] - (c1)[2] * (c2)[1];                           \
    (C3)[1] = (c1)[2] * (c2)[0] - (c1)[0] * (c2)[2];                           \
    (C3)[2] = (c1)[0] * (c2)[1] - (c1)[1] * (c2)[0];                           \
}

/*******************************************************************************
 * coord_assign - Assign coordinate from another coordinate
 *
 * RETURNS: N/A
 */

#define coord_assign(c1,  C2)                                                  \
{                                                                              \
    (C2)[0] = (c1)[0];                                                         \
    (C2)[1] = (c1)[1];                                                         \
    (C2)[2] = (c1)[2];                                                         \
}                                                                              \

/*******************************************************************************
 * coord_fill - Assign coordinate from components
 *
 * RETURNS: N/A
 */

#define coord_fill(a, b, c, C1)                                                \
{                                                                              \
    (C1)[0] = (a);                                                             \
    (C1)[1] = (b);                                                             \
    (C1)[2] = (c);                                                             \
}

/*******************************************************************************
 * coord_add - Component addition of two coordinates
 *
 * RETURNS: N/A
 */

#define coord_add(c1,  c2,  C3)                                                \
{                                                                              \
    (C3)[0] = (c1)[0] + (c2)[0];                                               \
    (C3)[1] = (c1)[1] + (c2)[1];                                               \
    (C3)[2] = (c1)[2] + (c2)[2];                                               \
}

/*******************************************************************************
 * coord_sub - Component substraction of two coordinates
 *
 * RETURNS: N/A
 */

#define coord_sub(c1,  c2,  C3)                                                \
{                                                                              \
    (C3)[0] = (c1)[0] - (c2)[0];                                               \
    (C3)[1] = (c1)[1] - (c2)[1];                                               \
    (C3)[2] = (c1)[2] - (c2)[2];                                               \
}

/*******************************************************************************
 * coord_mul - Component scalar multiplication for coordinate
 *
 * RETURNS: N/A
 */

#define coord_mult(c1,  s,  C2)                                                \
{                                                                              \
    (C2)[0] = s * (c1)[0];                                                     \
    (C2)[1] = s * (c1)[1];                                                     \
    (C2)[2] = s * (c1)[2];                                                     \
}

/*******************************************************************************
 * coord_div - Component scalar division for coordinate
 *
 * RETURNS: N/A
 */

#define coord_div(c1,  s,  C2)                                                 \
{                                                                              \
    float __divisor;                                                           \
                                                                               \
    __divisor = 1.0 / (s);                                                     \
    coord_mult(c1, __divisor, C2);                                             \
}


/*******************************************************************************
 * coord_sqr - Square lendth of vector
 *
 * RETURNS: N/A
 */

#define coord_sqr(c1,  Z)                                                      \
{                                                                              \
    coord_dot_product(c1, c1, Z);                                              \
}

/*******************************************************************************
 * coord_norm - Normal vector
 *
 * RETURNS: N/A
 */

#define coord_norm(c1, Z)                                                      \
{                                                                              \
    Z = sqrt(square((c1)[0]) +                                                 \
             square((c1)[1]) +                                                 \
             square((c1)[2]));                                                 \
}

/*******************************************************************************
 * coord_norm - Normalize vector
 *
 * RETURNS: N/A
 */

#define coord_normalize(c1, C2)                                                \
{                                                                              \
    float __magnitude;                                                         \
                                                                               \
    coord_norm(c1, __magnitude);                                               \
    coord_div(c1, __magnitude, C2);                                            \
}

/*******************************************************************************
 * coord_transform - Transform coordinate by matrix
 *
 * RETURNS: N/A
 */

#define coord_transform(c1, a, C2)                                             \
{                                                                              \
    Coord __tmp;                                                               \
                                                                               \
    __tmp[0] = a[0][0] * (c1)[0] +                                             \
               a[1][0] * (c1)[1] +                                             \
               a[2][0] * (c1)[2] +                                             \
               a[3][0];                                                        \
                                                                               \
    __tmp[1] = a[0][1] * (c1)[0] +                                             \
               a[1][1] * (c1)[1] +                                             \
               a[2][1] * (c1)[2] +                                             \
               a[3][1];                                                        \
                                                                               \
    __tmp[2] = a[0][2] * (c1)[0] +                                             \
               a[1][2] * (c1)[1] +                                             \
               a[2][2] * (c1)[2] +                                             \
               a[3][2];                                                        \
                                                                               \
    coord_assign(__tmp, C2);                                                   \
}

/*******************************************************************************
 * coord_transx - Transform of coordinate by matrix x-part
 *
 * RETURNS: N/A
 */

#define coord_transx(c1, a, c2)                                                \
{                                                                              \
    c2[0] = a[0][0] * (c1)[0] +                                                \
            a[1][0] * (c1)[1] +                                                \
            a[2][0] * (c1)[2] +                                                \
            a[3][0];                                                           \
}

/*******************************************************************************
 * coord_transy - Transform coordinate by matrix y-part
 *
 * RETURNS: N/A
 */

#define coord_transy(c1, a, c2)                                                \
{                                                                              \
    c2[1] = a[0][1] * (c1)[0] +                                                \
            a[1][1] * (c1)[1] +                                                \
            a[2][1] * (c1)[2] +                                                \
            a[3][1];                                                           \
}

/*******************************************************************************
 * coord_transz - Transform of coordinate by matrix z-part
 *
 * RETURNS: N/A
 */

#define coord_transz(c1, a, c2)                                                \
{                                                                              \
    c2[2] = a[0][2] * (c1)[0] +                                                \
            a[1][2] * (c1)[1] +                                                \
            a[2][2] * (c1)[2] +                                                \
            a[3][2];                                                           \
}

/*******************************************************************************
 * coord_transw - Transform of coordinate by matrix w-part
 *
 * RETURNS: N/A
 */

#define coord_transw(c1, a, w)                                                 \
{                                                                              \
    w = a[0][3] * (c1)[0] +                                                    \
        a[1][3] * (c1)[1] +                                                    \
        a[2][3] * (c1)[2] +                                                    \
        a[3][3];                                                               \
}

/*******************************************************************************
 * coord_point_edge_check - Edge check point
 *
 * RETURNS: N/A
 */

#define coord_point_edge_check(c0, c1, pt, SIDE)                               \
{                                                                              \
    float __A , __B , __C;                                                     \
                                                                               \
    __A = (c0)[1] - (c1)[1];                                                   \
    __B = (c1)[0] - (c0)[0];                                                   \
    __C = ((c1)[0] - (c0)[0])*(c0)[1] + ((c0)[1] - (c1)[1])*(c0)[0];           \
                                                                               \
    SIDE = signbit(__A * (pt)[0] + __B * (pt)[1] - __C);                       \
}

/*******************************************************************************
 * coord_reflect - Reflect coordinate
 *
 * RETURNS: N/A
 */

#define coord_reflect(c1, c2, cos_gamma, C3)                                   \
{                                                                              \
    (C3)[0] = 2.0 * (c2)[0] * cos_gamma - (c1)[0];                             \
    (C3)[1] = 2.0 * (c2)[1] * cos_gamma - (c1)[1];                             \
    (C3)[2] = 2.0 * (c2)[2] * cos_gamma - (c1)[2];                             \
}

/*******************************************************************************
 * coord_project - Project coordinate onto plane defined by normal
 *
 * RETURNS: N/A
 */

#define coord_project(c1, c2, cos_gamma, C3)                                   \
{                                                                              \
    (C3)[0] = (c1)[0] - (c2)[0] * cos_gamma;                                   \
    (C3)[1] = (c1)[1] - (c2)[1] * cos_gamma;                                   \
    (C3)[2] = (c1)[2] - (c2)[2] * cos_gamma;                                   \
}

/*******************************************************************************
 * coord_perspective - Perspective projection matrix
 *
 * RETURNS: N/A
 */

#define coord_perspective(fovy, near, far, aspect, M)                          \
{                                                                              \
    float ctan;                                                                \
                                                                               \
    ctan = -1.0 / tan(fovy * 0.5);                                             \
                                                                               \
    matrix_assign(ctan/aspect,    0,                   0,  0,                  \
                  0,           ctan,                   0,  0,                  \
                  0,              0,  1.0 / (far - near), -1,                  \
                  0,              0, near / (far - near),  0, M);              \
}

/*******************************************************************************
 * coord_print - Print coordinate
 *
 * RETURNS: N/A
 */

#define coord_print(c) printf("%9.4f %9.4f %9.4f", (c)[0], (c)[1], (c)[2]);

/*******************************************************************************
 * coord_orient
 *
 * RETURNS: N/A
 */

void coord_orient(
    Coord c1,
    Coord c2,
    Matrix M
    );

/*******************************************************************************
 * coord_view
 *
 * RETURNS: N/A
 */

void coord_view(
    Coord c1,
    Coord c2,
    float theta,
    Matrix M
    );

/*******************************************************************************
 * coord_look
 *
 * RETURNS: N/A
 */

void coord_look(
    Coord c1,
    Coord c2,
    Matrix M
    );

#ifdef __cplusplus
}
#endif

#endif

