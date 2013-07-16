#ifndef SCALAR_H
#define SCALAR_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIGMA                  (0.00001)

#define SIGN(x)                (((x) > 0.0) ? 1 : (((x) < 0.0) ? -1 : 0))

#define SQUARE(x)              ((x) * (x))

#define MIN(x, y)              ((x) > (y) ? (y) : (x))
#define MAX(x, y)              ((x) < (y) ? (y) : (x))

#define CLAMP(v, n, x)         (((v) < (n)) ? (n) : (((v) > (x)) ? (x) : (v)))

/*******************************************************************************
 * power_of_two - Determine if integer is a power-of-two
 *
 * RETURNS: 1 if power-of-two, otherwise 0
 */

int power_of_two(
    int s                      /* in: Integer to check */
    );

/*******************************************************************************
 * next_power_of_two - Determine if integer up to limit is a power-of-two
 *
 * RETURNS: 1 if power-of-two, otherwise 0
 */

int next_power_of_two(
    int s                      /* in: Integer limit to check */
    );

#ifdef __cplusplus
}
#endif

#endif

