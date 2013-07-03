/******************************************************************************

  File:        substitution.c
  Description: Functions for ANSI substitutions for better compatibility.

  Copyright (c) 2007,2010 Turku PET Centre

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details:
  http://www.gnu.org/copyleft/lesser.html

  You should have received a copy of the GNU Lesser General Public License
  along with this library/program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi

  Modification history:
  2005-01-05 Harri Merisaari
    Created.
  2010-07-01 Vesa Oikonen
    Added strcasestr().


******************************************************************************/
#ifdef __STRICT_ANSI__
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "include/substitutions.h"
/*!
 * Allocates memory and copies into
 * it the string addressed by s, including the terminating
 * character. User should free the allocated memory.
 *
 * @param s input string
 * @return pointer to allocated string
 */
char* strdup(const char* s) {
	void* r;
	size_t length;
	length = strlen(s)+1;
	r = malloc(length);
	if(r == NULL) return NULL;
	return (char*)memcpy(r, s, length);
}

/*!
 * Subfunction that converts character into lower case letter.
 * Character is expected to be in ASCII coding.
 *
 * @param c character
 * @return same character as an lower case
 */
char tolowercase(char c) {
	/* shift if necessary */
	if(c < 97) return c+32;
	else return c;
}


/*!
 * Case-insensitive string comparison between the strings
 * referenced by the pointers s1 and s2.
 *
 * @param s input string
 * @return positive integer if, disregarding case, string s1 is
 * lexically greater than string s2; zero if, other than case the two strings
 * are identical; and a negative integer if, disregarding case, string s1 is
 * lexically less than string s2.
 */
int strcasecmp(char *s1, char *s2) {
	char* t1;
	char* t2;
	t1 = s1;
	t2 = s2;
	/* characters are compared until 1st string ends or characters do not match */
	while(*t1 != '\0' && tolowercase(*(unsigned char *) t1) == tolowercase(*(unsigned char *) t2)) {
      		t1++;
      		t2++;
    	}
    	/* convert characters into ASCII codes and compare them */
        return tolowercase(*(unsigned char *) t1) - tolowercase(*(unsigned char *) t2);
}

#endif


#ifndef _GNU_SOURCE

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "substitutions.h"

#define touppercase(c) (toupper(((unsigned char)(c))))

#ifdef NO_STRCASESTR
/** Case-insensitive version of strstr().
\return Returns a pointer to the beginning of the first occurrence, or NULL
        if not found.
 */
char *strcasestr(
  /** Pointer to string in which substring is seached */
  char *src,
  /** Pointer to substring which is searced for in source string */
  char *sub
) {
  char *a=src, *e=sub;
  if(!src || !*src || !sub || !*sub) return 0;
  while(*a && *e) {
    if(touppercase(*a)!=touppercase(*e)) {
      ++src; a=src; e=sub;
    } else {
      ++a; ++e;
    }
  }
  return *e ? 0 : src;
}
#endif

#endif

