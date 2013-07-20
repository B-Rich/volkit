#ifndef SCALAR_H
#define SCALAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

/*******************************************************************************
 * square - Calculate square of number
 *
 * RETURNS: Argument squared
 */

#define square(x)              ((x) * (x))

/*******************************************************************************
 * sign - Return sign of number
 *
 * RETURNS: Sign-bit for number
 */

#define sign(x)                (((x) > 0.0) ? 1 : (((x) < 0.0) ? -1 : 0))

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

