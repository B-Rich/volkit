#include <stdio.h>

#include "volren/matrix.h"

matrix IdentityMatrix =
{
    { 1.0, 0.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 0.0, 1.0 }
};

/*******************************************************************************
 * matrix_invert4 - Invert 4x4 matrix
 *
 * RETURNS: N/A
 */

void matrix_invert4(
    matrix m1,                 /* in: Matrix to be inverted */
    matrix m2                  /* out: Resulting inverse matrix */
    )
{
    float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    float det;

    a = m1[0][0]; b = m1[1][0]; c = m1[2][0]; d = m1[3][0];
    e = m1[0][1]; f = m1[1][1]; g = m1[2][1]; h = m1[3][1];
    i = m1[0][2]; j = m1[1][2]; k = m1[2][2]; l = m1[3][2];
    m = m1[0][3]; n = m1[1][3]; o = m1[2][3]; p = m1[3][3];

    det = d*g*j*m - c*h*j*m - d*f*k*m + b*h*k*m + c*f*l*m -
          b*g*l*m - d*g*i*n + c*h*i*n + d*e*k*n - a*h*k*n -
          c*e*l*n + a*g*l*n + d*f*i*o - b*h*i*o - d*e*j*o +
          a*h*j*o + b*e*l*o - a*f*l*o - c*f*i*p + b*g*i*p +
          c*e*j*p - a*g*j*p - b*e*k*p + a*f*k*p;
  
    if (det != 0.0)
    {
        m2[0][0] = (-(h*k*n) + g*l*n + h*j*o - f*l*o - g*j*p + f*k*p) / det;
        m2[1][0] = (d*k*n - c*l*n - d*j*o + b*l*o + c*j*p - b*k*p) / det;
        m2[2][0] = (-(d*g*n) + c*h*n + d*f*o - b*h*o - c*f*p + b*g*p) / det;
        m2[3][0] = (d*g*j - c*h*j - d*f*k + b*h*k + c*f*l - b*g*l) / det;
        m2[0][1] = (h*k*m - g*l*m - h*i*o + e*l*o + g*i*p - e*k*p) / det;
        m2[1][1] = (-(d*k*m) + c*l*m + d*i*o - a*l*o - c*i*p + a*k*p) / det;
        m2[2][1] = (d*g*m - c*h*m - d*e*o + a*h*o + c*e*p - a*g*p) / det;
        m2[3][1] = (-(d*g*i) + c*h*i + d*e*k - a*h*k - c*e*l + a*g*l) / det;
        m2[0][2] = (-(h*j*m) + f*l*m + h*i*n - e*l*n - f*i*p + e*j*p) / det;
        m2[1][2] = (d*j*m - b*l*m - d*i*n + a*l*n + b*i*p - a*j*p) / det;
        m2[2][2] = (-(d*f*m) + b*h*m + d*e*n - a*h*n - b*e*p + a*f*p) / det;
        m2[3][2] = (d*f*i - b*h*i - d*e*j + a*h*j + b*e*l - a*f*l) / det;
        m2[0][3] = (g*j*m - f*k*m - g*i*n + e*k*n + f*i*o - e*j*o) / det;
        m2[1][3] = (-(c*j*m) + b*k*m + c*i*n - a*k*n - b*i*o + a*j*o) / det;
        m2[2][3] = (c*f*m - b*g*m - c*e*n + a*g*n + b*e*o - a*f*o) / det;
        m2[3][3] = (-(c*f*i) + b*g*i + c*e*j - a*g*j - b*e*k + a*f*k) / det;
    }
    else
    {
        fprintf(stderr, "Unable invert signular matrix\n");
        matrix_copy(IdentityMatrix, 4, m2);
    }
}

