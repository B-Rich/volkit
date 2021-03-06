#include "volgeo/coord.h"

#define SIGMA1                 (0.00001)
#define SIGMA2                 (0.000001)

/*******************************************************************************
 * coord_orient
 *
 * RETURNS: N/A
 */

void coord_orient(
    Coord c1,
    Coord c2,
    Matrix M
    )
{
    float d, c1_z, c1_y;
    Coord cc1, cc2;
    Matrix Rx, Ry, Rz, M1, M2;

    d = 1.0 / sqrt(square(c1[1]) + square(c1[2]));

    c1_z = c1[2] * d;
    c1_y = c1[1] * d;

    matrix_assign(1,     0,    0, 0,
                  0,  c1_z, c1_y, 0,
                  0, -c1_y, c1_z, 0,
                  0,     0,    0, 1, Rx);
                                
    coord_transform(c1, Rx, cc1);
                                
    matrix_assign( cc1[2],   0, cc1[0], 0,
                       0,    1,      0, 0,
                  -cc1[0],   0, cc1[2], 0,
                       0,    0,      0, 1, Ry);
                                
    matrix_mult(Rx,  Ry, M1);
                                
    coord_transform(c2, M1, cc2);
                                
    matrix_assign(cc2[0], -cc2[1],  0, 0,
                  cc2[1],  cc2[0],  0, 0,
                      0,        0,  1, 0,
                      0,        0,  0, 1, Rz);

    matrix_mult(M1,  Rz, M2);
    matrix_transpose(M2, 4, M);
}

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
    )
{
    float d, c1_y, c1_z;
    Coord cc0, cc1;
    Matrix Rx, Ry, Rz, M0, M1, T0;

    matrix_assign(     1,      0,      0, 0,
                       0,      1,      0, 0,
                       0,      0,      1, 0,
                  -c1[0], -c1[1], -c1[2], 1, T0);

    coord_sub(c2, c1, cc0);

    d = sqrt(square(cc0[1]) + square(cc0[2]));

    if (d > SIGMA1)
    {
        d = 1.0 / d;              

        c1_z = -cc0[2] * d;
        c1_y =  cc0[1] * d;
    }
    else
    {
        c1_z = -1.0;
        c1_y =  0.0;
    } 

    matrix_assign( 1,     0,     0, 0,
                   0,  c1_z, -c1_y, 0,
                   0,  c1_y,  c1_z, 0,
                   0,     0,     0, 1, Rx);

    matrix_mult(T0, Rx, M0);
                       

    coord_transform(cc0, Rx, cc1);

    d = sqrt(square(cc1[0]) + square(cc1[2]));

    if (d > SIGMA2)
    {
        d = 1.0 / d;

        cc1[2] *= -d;
        cc1[0] *=  d;
    }
    else
    {       
        cc1[2] = -1.0;
        cc1[0] =  0.0;
    }

    matrix_assign(cc1[2],     0, -cc1[0], 0,
                      0,      1,       0, 0,
                  cc1[0],     0,  cc1[2], 0,
                      0,      0,       0, 1, Ry);

    matrix_mult(M0, Ry, M1);

    matrix_assign( cos(theta),  sin(theta),  0, 0,
                  -sin(theta),  cos(theta),  0, 0,
                            0,           0,  1, 0,
                            0,           0,  0, 1, Rz);

   matrix_mult(M1,  Rz, M);
}

/*******************************************************************************
 * coord_look
 *
 * RETURNS: N/A
 */

void coord_look(
    Coord c1,
    Coord c2,
    Matrix M
    )
{
    float d, c1_y, c1_z, x, y;
    Coord cc0, cc1;
    Matrix Rx, Ry, Rz, M0, M1, T0;
    Coord rms,rms1 ;

    matrix_assign(     1,      0,      0, 0,
                       0,      1,      0, 0,
                       0,      0,      1, 0,
                  -c1[0], -c1[1], -c1[2], 1, T0);

    coord_sub(c2, c1, cc0);

    d = sqrt(square(cc0[1]) + square(cc0[2]));

    if (d > SIGMA1)
    {
        d = 1.0 / d;              

        c1_z = -cc0[2] * d;
        c1_y =  cc0[1] * d;
    }
    else
    {
        c1_z = -1.0;
        c1_y =  0.0;
    }

    matrix_assign( 1,     0,     0, 0,
                   0,  c1_z, -c1_y, 0,
                   0,  c1_y,  c1_z, 0,
                   0,     0,     0, 1, Rx);

    coord_transform(cc0, Rx, cc1);

    d = sqrt(square(cc1[0]) + square(cc1[2]));

    if (d > SIGMA2)
    {
        d = 1.0 / d;

       cc1[2] *= -d;
       cc1[0] *=  d;
    }
    else
    {       
        cc1[2] = -1.0;
        cc1[0] =  0.0;
    }


    matrix_assign(cc1[2],     0, -cc1[0], 0,
                      0,       1,      0, 0,
                  cc1[0],     0,  cc1[2], 0,
                      0,       0,      0, 1, Ry);

    matrix_mult(Rx, Ry, M1);

    rms[0] = 0.782608;
    rms[1] = 0.0;
    rms[2] = 0.622514;
    coord_transform(rms, M1, rms1);
    x = rms1[0];
    y = rms1[1];

    d = sqrt(square(x) + square(y));

    x /= d;
    y /= d;

    matrix_assign(x, -y,  0, 0,
                  y,  x,  0, 0,
                  0,  0,  1, 0,
                  0,  0,  0, 1, Rz);

    matrix_mult(M1,  Rz, M0);
    matrix_mult(T0,  M0, M);
}

