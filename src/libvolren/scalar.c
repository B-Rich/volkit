#include "volren/scalar.h"

/*******************************************************************************
 * power_of_two - Determine if integer is a power-of-two
 *
 * RETURNS: 1 if power-of-two, otherwise 0
 */

int power_of_two(
    int s                      /* in: Integer to check */
    )
{
    int result;
    unsigned long p = 1;

    while (p < s)
    {
        p = p << 1;
    }

    if (p == s)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}

/*******************************************************************************
 * next_power_of_two - Determine if integer up to limit is a power-of-two
 *
 * RETURNS: 1 if power-of-two, otherwise 0
 */

int next_power_of_two(
    int s                      /* in: Integer limit to check */
    )
{
    int result;
    unsigned long p = 1;

    if (power_of_two(s))
    {
        result = s;
    }
    else
    {
        while (p < s)
        {
            p = p << 1;
        }
        result = p;
    }

    return result;
}

