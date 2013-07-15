#include <stdio.h>
#include <math.h>

#include "cube.h"

/*******************************************************************************
 * getPlaneCubeIntersect - Get vertices for intersection between plane and cube
 *
 * RETURNS: Number of polygon vertices for intersection
 *
 */

int getPlaneCubeIntersect(
    float a,                   /* in: Plane equation A (normal x) */
    float b,                   /* in: Plane equation B (normal y) */
    float c,                   /* in: Plane equation C (normal z) */
    float d,                   /* in: Plane equation D (offset) */
    float tl[8][3],            /* out: Intersection vertices */
    float cpt[8][3]            /* in: Cube vertices */
    )
{
    int i, j, n;
    int span;
    int faces[6];
    int lastedge, lastface, face;
    float pleval, plevalnxt;
    float t1, t2;

    for (i = 0; i < 6; i++)
    {
        faces[i] = 0;
    }

    i = 0;
    n = 0;

    pleval = cpt[edgevertices[i][0]][0] * a + 
             cpt[edgevertices[i][0]][1] * b +
             cpt[edgevertices[i][0]][2] * c + d;

    plevalnxt = cpt[edgevertices[i][1]][0] * a + 
                cpt[edgevertices[i][1]][1] * b +
                cpt[edgevertices[i][1]][2] * c + d;

    /* If sign bits are different, endpoints of edge span slicing plane */
    span = signbit(pleval) ^ signbit(plevalnxt);

    while (i < 12 && !span) 
    {
        i++;

        pleval = cpt[edgevertices[i][0]][0] * a + 
                 cpt[edgevertices[i][0]][1] * b +
                 cpt[edgevertices[i][0]][2] * c + d;

        plevalnxt = cpt[edgevertices[i][1]][0] * a + 
                    cpt[edgevertices[i][1]][1] * b +
                    cpt[edgevertices[i][1]][2] * c + d;

        span = signbit(pleval) ^ signbit(plevalnxt);
    }

    if (i < 12)
    {
        face = edgefaces[i][0];
        lastface = edgefaces[i][1];
        lastedge = -1;

        while (faces[lastface] < 2) 
        {
            j = 0;
            if (faces[face] > 1) 
            {
                if (faces[edgefaces[lastedge][0]] < 2)
                {
                    face = edgefaces[lastedge][0];
                }
                else
                {
                    face = edgefaces[lastedge][1];
                }
            }

            while (faces[face] < 2)
            {
                i = faceedges[face][j];

                pleval = cpt[edgevertices[i][0]][0] * a + 
                         cpt[edgevertices[i][0]][1] * b +
                         cpt[edgevertices[i][0]][2] * c + d;

                plevalnxt = cpt[edgevertices[i][1]][0] * a + 
                            cpt[edgevertices[i][1]][1] * b +
                            cpt[edgevertices[i][1]][2] * c + d;

                span = signbit(pleval) ^ signbit(plevalnxt);

                if ((lastedge != i) && span)
                {
                    if (plevalnxt == pleval)
                    {
                        fprintf(stderr,"GetPoly: will get divide by 0\n");
                    }

                    t1 = 1.0 / (plevalnxt - pleval);
                    t1 *= plevalnxt;
                    t2 = (1.0 - t1);

                    tl[n][0] = cpt[edgevertices[i][0]][0] * t1 +
                               cpt[edgevertices[i][1]][0] * t2;

                    tl[n][1] = cpt[edgevertices[i][0]][1] * t1 +
                               cpt[edgevertices[i][1]][1] * t2;

                    tl[n][2] = cpt[edgevertices[i][0]][2] * t1 +
                               cpt[edgevertices[i][1]][2] * t2;

                    lastedge = i;
                    faces[edgefaces[i][0]]++;
                    faces[edgefaces[i][1]]++;
                    n++;
                }
                j++;
            }
        }
    }

    return n;
}

#ifdef MAIN
int main(void)
{
    int i, n;
    coord p[8];

    n = getPlaneCubeIntersect(0.707, 0.0, 0.707, 0.5, p, PlusMinusCube);

    printf("%d polygon points:\n", n);
    for (i = 0; i < n; i++)
    {
        printf("\t%g %g %g\n", p[i][0], p[i][1], p[i][2]);
    }

    return 0;
}
#endif

