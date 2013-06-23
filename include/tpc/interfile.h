/******************************************************************************

  Copyright (c) 2005,2012 by Turku PET Centre

  interfile.h

  Modification history:
  2005-04-05 (krs) (Roman Krais)
  2012-02-21 Vesa Oikonen

******************************************************************************/
#ifndef _INTERFILE_H_
#define _INTERFILE_H_
/*****************************************************************************/
/* Backup file extension */
#ifndef BACKUP_EXTENSION
#define BACKUP_EXTENSION ".bak"
#endif 
/*****************************************************************************/
extern int interfile_read(char headerName[256], char searchWord[256],
  char returnValue[256], char errorMessage[300]);
/*****************************************************************************/
extern int interfileIsHeader(const char *hdrfile, char *imgfile);
extern int interfileExists(
  const char *fname, char *hdrfile, char *imgfile, int verbose);
/*****************************************************************************/
#endif
