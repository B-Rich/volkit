/******************************************************************************
  Copyright (c)  2004 by Turku PET Centre

  idcrypt.h

  Date
  2004-12-14 Calle Laakkonen
    Created file
******************************************************************************/
#ifndef IDCRYPT_H
#define IDCRYPT_H
/*****************************************************************************/

/*
 * Return libpet idcrypt module version
 */
extern const char *libpet_idcrypt_version(void);

/*
 * Scramble characters in ASCII range 32-126
 * using the Vigenere Cipher.
 */
extern int id_crypt(const char *string,const char *key,char *out,int decrypt);


#endif

