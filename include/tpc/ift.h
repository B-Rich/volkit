/******************************************************************************

  Copyright (c) 2004-2008,2012 by Turku PET Centre

  ift.h

  Modification history:
  2004-08-15 VO
  2004-10-14 VO
    iftFindNthValue() function name corrected.
  2004-10-15 VO
    Included iftFindNthKey().
  2004-11-23 VO
    Included iftReplaceNthValue().
  2005-01-05 VO
    ift_status moved in here.
  2008-07-11 VO
    Added the externs.
  2012-02-14 VO
    Changed comment.
  2012-02-16 VO
    Added functions iftGetFrom(), iftGetFullmatchFrom(), iftGetFloatValue(),
    iftGetIntValue(), and defRead().
  2012-02-17 VO
    Added function iftdup().
    Added enums for ift_status messages.
  2012-03-02 VO
    Added comments.
  2013-04-17 VO
    Included stdio.h and stdlib.h.


******************************************************************************/
#ifndef _IFT_H
#define _IFT_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************/

/*****************************************************************************/
/** Definitions for ift status message */
enum {IFT_OK, IFT_FAULT, IFT_NOMEMORY, IFT_CANNOTREAD, IFT_CANNOTWRITE,
      IFT_UNKNOWNFORMAT, IFT_KEYNOTFOUND, IFT_NODATA, IFT_VALUENOTFOUND};
/*****************************************************************************/

/*****************************************************************************/
typedef struct {
  /** Key (comment) type character: space, #, ; */
  char type;
  /** Undefined short integer for the use of programmer */
  short int sw;
  /** Pointer to the NULL terminated key string; NULL if not allocated */
  char *key;
  /** Pointer to the NULL terminated key value string; NULL if not allocated */
  char *value;
} IFT_KEY_AND_VALUE;
typedef struct {
  /** Number of allocated places for keys and values */
  int _memNr;
  /** Number of stored keys and their values */
  int keyNr;
  /** Type of the parameter file:
      0=unknown, 1=interfile ':=' , 2=setup '=', 3=result ':', 4=space ' ' 
      5=tab, 6=',', 7=';'
  */
  int type;
  /** Pointer to a status message */
  const char *status;
  /** List of key-value -pairs */
  IFT_KEY_AND_VALUE *item;
  /** Size of binary data (in bytes); not yet supported */
  size_t datasize;
  /** Pointer to binary data; not yet supported */
  unsigned char *data;
} IFT;
/*****************************************************************************/
extern int IFT_TEST;
/*****************************************************************************/
extern void libift_printdate(FILE *fp);
extern void iftSetStatus(IFT *ift, int status);
extern void iftInit(IFT *ift);
extern void iftEmpty(IFT *ift);
extern int iftPut(IFT *ift, char *key, char *value, char *cmt_type);
extern int iftRead(IFT *ift, char *filename, int is_key_required);
extern int iftWriteItem(IFT *ift, int item, FILE *fp);
extern int iftWrite(IFT *ift, char *filename);
extern int defRead(IFT *ift, char *filename);
extern int iftGet(IFT *ift, char *key);
extern int iftGetNth(IFT *ift, char *key, int n);
extern int iftFindNthKey(IFT *ift, char *str, int n);
extern int iftFindNthValue(IFT *ift, char *str, int n);
extern int iftGetFrom(IFT *ift, int si, const char *key);
extern int iftGetFullmatchFrom(
  IFT *ift, int si, const char *key, const char *value);
extern int iftGetFloatValue(IFT *ift, int si, const char *key, float *value);
extern int iftGetIntValue(IFT *ift, int si, const char *key, int *value);
extern int iftDeleteItem(IFT *ift, int item);
extern int iftReplaceNthValue(IFT *ift, int item, char *value);
extern int iftdup(IFT *ift1, IFT *ift2);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

