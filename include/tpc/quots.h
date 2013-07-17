/******************************************************************************
  Copyright (c) 2006,2013 by Turku PET Centre

  quots.h


  Modification history:
  2006-10-23 Vesa Oikonen
    First created.
  2013-06-17 VO
    Added function strnCopyClean().


******************************************************************************/
#ifndef _QUOTS_H
#define _QUOTS_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
extern char *strstr_noquotation(const char *str1, const char *str2);
extern int strnCopyClean(char *str1, const char *str2, int maxlen);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

