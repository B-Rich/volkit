#ifndef VG_MATRIX_H
#define VG_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>

typedef float Matrix[4][4];

/*******************************************************************************
 * matrix_assign - Assign element values to 4x4 matrix
 *
 * RETURNS: N/A
 */

#define matrix_assign(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, A)       \
{                                                                              \
    (A)[0][0] = a; (A)[0][1] = b; (A)[0][2] = c; (A)[0][3] = d;                \
    (A)[1][0] = e; (A)[1][1] = f; (A)[1][2] = g; (A)[1][3] = h;                \
    (A)[2][0] = i; (A)[2][1] = j; (A)[2][2] = k; (A)[2][3] = l;                \
    (A)[3][0] = m; (A)[3][1] = n; (A)[3][2] = o; (A)[3][3] = p;                \
}

/*******************************************************************************
 * matrix_inner_product - Calculate inner product of 4x4 matrix
 *
 * RETURNS: N/A
 */

#define matrix_inner_product(a, b, row, col, C)                                \
{                                                                              \
    (C)[row][col] = (a)[row][0] * (b)[0][col] +                                \
                    (a)[row][1] * (b)[1][col] +                                \
                    (a)[row][2] * (b)[2][col] +                                \
                    (a)[row][3] * (b)[3][col];                                 \
}

/*******************************************************************************
 * matrix_mult - Multiply two 4x4 matrices
 *
 * RETURNS: N/A
 */

#define matrix_mult(a, b, C)                                                   \
{                                                                              \
    matrix_inner_product(a, b, 0, 0, C);                                       \
    matrix_inner_product(a, b, 0, 1, C);                                       \
    matrix_inner_product(a, b, 0, 2, C);                                       \
    matrix_inner_product(a, b, 0, 3, C);                                       \
    matrix_inner_product(a, b, 1, 0, C);                                       \
    matrix_inner_product(a, b, 1, 1, C);                                       \
    matrix_inner_product(a, b, 1, 2, C);                                       \
    matrix_inner_product(a, b, 1, 3, C);                                       \
    matrix_inner_product(a, b, 2, 0, C);                                       \
    matrix_inner_product(a, b, 2, 1, C);                                       \
    matrix_inner_product(a, b, 2, 2, C);                                       \
    matrix_inner_product(a, b, 2, 3, C);                                       \
    matrix_inner_product(a, b, 3, 0, C);                                       \
    matrix_inner_product(a, b, 3, 1, C);                                       \
    matrix_inner_product(a, b, 3, 2, C);                                       \
    matrix_inner_product(a, b, 3, 3, C);                                       \
}

/*******************************************************************************
 * matrix_mult_safe - Safe multiplication of two 4x4 matrices
 *
 * RETURNS: N/A
 */

#define matrix_mult_safe(a, b, C)                                              \
{                                                                              \
    Matrix d;                                                                  \
                                                                               \
    matrix_inner_product(a, b, 0, 0, d);                                       \
    matrix_inner_product(a, b, 0, 1, d);                                       \
    matrix_inner_product(a, b, 0, 2, d);                                       \
    matrix_inner_product(a, b, 0, 3, d);                                       \
    matrix_inner_product(a, b, 1, 0, d);                                       \
    matrix_inner_product(a, b, 1, 1, d);                                       \
    matrix_inner_product(a, b, 1, 2, d);                                       \
    matrix_inner_product(a, b, 1, 3, d);                                       \
    matrix_inner_product(a, b, 2, 0, d);                                       \
    matrix_inner_product(a, b, 2, 1, d);                                       \
    matrix_inner_product(a, b, 2, 2, d);                                       \
    matrix_inner_product(a, b, 2, 3, d);                                       \
    matrix_inner_product(a, b, 3, 0, d);                                       \
    matrix_inner_product(a, b, 3, 1, d);                                       \
    matrix_inner_product(a, b, 3, 2, d);                                       \
    matrix_inner_product(a, b, 3, 3, d);                                       \
                                                                               \
    matrix_copy(d, 4, C);                                                      \
}

/*******************************************************************************
 * matrix_transpose - Transpose 4x4 matrix
 *
 * RETURNS: N/A
 */

#define matrix_transpose(a, n, B)                                              \
{                                                                              \
    int i, j;                                                                  \
                                                                               \
    for (i = 0; i < n; i++)                                                    \
    {                                                                          \
        for (j = 0; j < n; j++)                                                \
        {                                                                      \
            (B)[j][i] = (a)[i][j];                                             \
        }                                                                      \
    }                                                                          \
}

/*******************************************************************************
 * matrix_print_prefix - Print 4x4 matrix with prefix
 *
 * RETURNS: N/A
 */

#define matrix_print_prefix(a, b)                                              \
{                                                                              \
    int i, j;                                                                  \
                                                                               \
    for (i = 0; i < 4; i++)                                                    \
    {                                                                          \
        printf("%s", b);                                                       \
        for (j = 0; j < 4; j++)                                                \
        {                                                                      \
            printf(" %9.4f", (a)[i][j]);                                       \
        }                                                                      \
        printf("\n");                                                          \
    }                                                                          \
}

/*******************************************************************************
 * matrix_print - Print 4x4 matrix
 *
 * RETURNS: N/A
 */

#define matrix_print(a)        matrix_print_prefix(a, " ")

/*******************************************************************************
 * matrix_copy - Copy nxn matrix
 *
 * RETURNS: N/A
 */

#define matrix_copy(a, n, B)   { memcpy(B, a, n * n * sizeof(float)); }

/*******************************************************************************
 * matrix_scale - Create 4x4 scale matrix
 *
 * RETURNS: N/A
 */

#define matrix_scale(x, y, z, A)                                               \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    matrix_assign(x,  0.0, 0.0, 0.0,                                           \
                  0.0,  y, 0.0, 0.0,                                           \
                  0.0, 0.0,   z, 0.0,                                          \
                  0.0, 0.0, 0.0, 1.0, b);                                      \
    matrix_mult_safe(A, b, A);                                                 \
}

/*******************************************************************************
 * matrix_translate - Create 4x4 translation matrix
 *
 * RETURNS: N/A
 */

#define matrix_translate(x, y, z, A)                                           \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    matrix_assign(1.0, 0.0, 0.0, 0.0,                                          \
                  0.0, 1.0, 0.0, 0.0,                                          \
                  0.0, 0.0, 1.0, 0.0,                                          \
                    x,   y,   z, 1.0, b) ;                                     \
  matrix_mult_safe(A, b, A);                                                   \
}

/*******************************************************************************
 * matrix_pre_scale - Create 4x4 pre-scale matrix
 *
 * RETURNS: N/A
 */

#define matrix_pre_scale(x, y, z, A)                                           \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    matrix_assign(  x, 0.0, 0.0, 0.0,                                          \
                  0.0,   y, 0.0, 0.0,                                          \
                  0.0, 0.0,   z, 0.0,                                          \
                  0.0, 0.0, 0.0, 1.0, b);                                      \
    matrix_mult_safe(b, A, A);                                                 \
}

/*******************************************************************************
 * matrix_pre_translate - Create 4x4 pre-translation matrix
 *
 * RETURNS: N/A
 */

#define matrix_pre_translate(x, y, z, A)                                       \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    matrix_assign(1.0, 0.0, 0.0, 0.0,                                          \
                  0.0, 1.0, 0.0, 0.0,                                          \
                  0.0, 0.0, 1.0, 0.0,                                          \
                    x,   y,   z, 1.0, b);                                      \
    matrix_mult_safe(b, A, A);                                                 \
}

/*******************************************************************************
 * matrix_xrot - Create 4x4 matrix for rotation around x-axis
 *
 * RETURNS: N/A
 */

#define matrix_xrot(ang, A)                                                    \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    double s = sin(ang);                                                       \
    double c = cos(ang);                                                       \
                                                                               \
    matrix_assign(1.0, 0.0, 0.0, 0.0,                                          \
                  0.0,   c,  -s, 0.0,                                          \
                  0.0,   s,   c, 0.0,                                          \
                  0.0, 0.0, 0.0, 1.0, b);                                      \
    matrix_mult_safe(A, b, A);                                                 \
}

/*******************************************************************************
 * matrix_yrot - Create 4x4 matrix for rotation around y-axis
 *
 * RETURNS: N/A
 */

#define matrix_yrot(ang, A)                                                    \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    double s = sin(ang);                                                       \
    double c = cos(ang);                                                       \
                                                                               \
    matrix_assign(  c, 0.0,   s, 0.0,                                          \
                  0.0, 1.0, 0.0, 0.0,                                          \
                   -s, 0.0,   c, 0.0,                                          \
                  0.0, 0.0, 0.0, 1.0, b);                                      \
    matrix_mult_safe(A, b, A);                                                 \
}

/*******************************************************************************
 * matrix_zrot - Create 4x4 matrix for rotation around z-axis
 *
 * RETURNS: N/A
 */

#define matrix_zrot(ang, A)                                                    \
{                                                                              \
    Matrix b;                                                                  \
                                                                               \
    double s = sin(ang);                                                       \
    double c = cos(ang);                                                       \
                                                                               \
    matrix_assign(  c,  -s, 0.0, 0.0,                                          \
                    s,   c, 0.0, 0.0,                                          \
                  0.0, 0.0, 1.0, 0.0,                                          \
                  0.0, 0.0, 0.0, 1.0, b);                                      \
    matrix_mult_safe(A, b, A);                                                 \
}

extern const Matrix IdentityMatrix;

/*******************************************************************************
 * matrix_invert - Invert 4x4 matrix
 *
 * RETURNS: N/A
 */

void matrix_invert(
    Matrix m1,                 /* in: Matrix to be inverted */
    Matrix m2                  /* out: Resulting inverse matrix */
    );

#ifdef __cplusplus
}
#endif

#endif
