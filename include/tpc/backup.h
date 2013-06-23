/******************************************************************************
  Copyright (c) 2008,2010 by Turku PET Centre

  backup.h


  Modification history:
  2008-07-14 Vesa Oikonen
    First created.
  2010-10-27 VO
    Added function fileCopy().


******************************************************************************/
#ifndef _BACKUP_H
#define _BACKUP_H
/*****************************************************************************/
extern int backupExistingFile(char *filename, char *backup_ext, char *status);
extern int fileCopy(char *filename1, char *filename2, char *status);
/*****************************************************************************/
#endif

