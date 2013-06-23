/******************************************************************************

  Library file: idcrypt.c
  Description:  Functions for encryption/decryption of subject names and other
                identification information in string form.

  Copyright (c) 2004 Turku PET Centre

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
  2004-12-14 Calle Laakkonen
    First version.
  2004-12-22 Vesa Oikonen
    Changes in comments, not affecting compiled code.

******************************************************************************/

#include <stdlib.h>
#include <string.h>

/** Return idcrypt module version info */
const char *libpet_idcrypt_version(void) {
    return "2004-12-14";
}

/** Scramble characters in ASCII range 32-126 using the Vigenere Cipher.
    Other characters are discarded.
\return Returns 0 if successful
    */
int id_crypt(
  /** Original string to be encrypted/decrypted */
  const char *string,
  /** Keyword string */
  const char *key,
  /** Encrypted/decrypted string */
  char *out,
  /** Set to 1 when decrypting, or to 0 when encrypting */
  int decrypt
) {
    char *keystr;
    int len;
    int r;

    len=strlen(string);
    if(len==0) return 0;

    keystr = malloc(len);
    if(!keystr) return 1;

    if(len>strlen(key)) {
        for(r=0;r<len;r++) {
            keystr[r]=key[r%strlen(key)]-32;
            if(keystr[r]>94) keystr[r]=94;
        }
    } else {
        for(r=0;r<len;r++) {
            keystr[r]=key[r]-32;
            if(keystr[r]>94) keystr[r]=94;
        }
    }

    for(r=0;r<len;r++) {
        int c=(unsigned char)string[r]-32;
        if(c>94) c=94;
        if(decrypt) {
            c=c-keystr[r];
            if(c<0) c=(95-(-c)%95);
        } else {
            c=(c+keystr[r])%95;
        }
        out[r]=c+32;
    }
    out[r]=0;
    free(keystr);
    return 0;
}

