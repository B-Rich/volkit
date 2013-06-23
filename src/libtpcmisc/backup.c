/******************************************************************************

  Copyright (c) 2008-2010 Turku PET Centre

  File: backup.c

  Description:
  Functions for file copying and making backup.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  For more information on open source licenses see http://opensource.org.

  Modification history:
  2008-07-14 Vesa Oikonen
    First created.
  2009-01-15 VO
    Change in one return status text.
  2010-10-27 VO
    Added function fileCopy().


******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
/*****************************************************************************/
#include "backup.h"
#include "substitutions.h"
/*****************************************************************************/

/*****************************************************************************/
/** Check if specified file exists; rename existing file to a backup file.
 *  If also backup file exists, then remove that.
\return Returns 0, if successful, and <> in case of an error.
 */
int backupExistingFile(
  /** Name of file which, if it exists, is renamed to a backup file */
  char *filename,
  /** Extension for backup file; NULL will set the default ".bak" extension. */
  char *backup_ext,
  /** Pointer to a string (allocated for at least 64 chars) where error message
      or other execution status will be written; enter NULL, if not needed */     
  char *status
) {
  char bakfile[FILENAME_MAX];
  int ret;

  // Check the input
  if(filename==NULL || strlen(filename)<1) {
    if(status!=NULL) sprintf(status, "invalid filename");
    return 1;
  }

  // Check if file exists; if not then no need to make any backup
  if(access(filename, 0) == -1) {
    if(status!=NULL) sprintf(status, "file does not pre-exist");
    return 0;
  }
  // Creat filename for the backup file
  strcpy(bakfile, filename);
  if(backup_ext==NULL) strcat(bakfile, ".bak");
  else strcat(bakfile, backup_ext);
  // If also backup file exists, then just delete it 
  if(access(bakfile, 0) != -1) {
    ret=remove(bakfile);
    if(ret!=0) {
      if(status!=NULL) sprintf(status, "cannot delete previous backup file");
      return 3;
    }
  }
  // Rename file
  ret=rename(filename, bakfile);
  if(ret!=0) {
    if(status!=NULL) sprintf(status, "cannot rename file as backup");
    return 5;
  }
  if(status!=NULL) sprintf(status, "file renamed as backup");
  return 0;
}  
/*****************************************************************************/

/*****************************************************************************/
/** Copy file contents to another file. Existing file will be overwritten,
 *  to prevent it call backupExistingFile() before calling this function.
\return Returns 0 if successfull, otherwise >0.
 */ 
int fileCopy(
  /** Name of file to be copied */
  char *filename1,
  /** Name of new file */
  char *filename2,
  /** Pointer to a string (allocated for at least 64 chars) where error message
      or other execution status will be written; enter NULL, if not needed */     
  char *status
) {
  FILE *from, *to;
  char c;

  // Check the input
  if(filename1==NULL || filename2==NULL) {
    if(status!=NULL) sprintf(status, "invalid filename");
    return 1;
  }
  // Open the file1 for reading
  if((from=fopen(filename1, "rb"))==NULL) {
    if(status!=NULL) sprintf(status, "cannot open file for read");
    return 2;
  }
  // Open file2 for writing
  if((to=fopen(filename2, "wb"))==NULL) {
    if(status!=NULL) sprintf(status, "cannot open file for write");
    fclose(from); return 3;
  }
  // Copy the file
  while(!feof(from)) {
    c=fgetc(from);
    if(ferror(from)) {
      if(status!=NULL) sprintf(status, "cannot read from file");
      fclose(from); fclose(to); (void)remove(filename2); return 4;
    }
    if(!feof(from)) fputc(c, to);
    if(ferror(to)) {
      if(status!=NULL) sprintf(status, "cannot write to file");
      fclose(from); fclose(to); (void)remove(filename2); return 6;
    }
  }
  // Close files
  if(fclose(from)==EOF) {
    if(status!=NULL) sprintf(status, "cannot close file");
    fclose(to); return 7;
  }
  if(fclose(to)==EOF) {
    if(status!=NULL) sprintf(status, "cannot close file");
    return 8;
  }
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/

