/******************************************************************************
  Copyright (c) 2007 Turku PET Centre

  substitutions.h

  Version history:
  2006-17-07 Harri Merisaari
    First introduced.

******************************************************************************/
#ifndef _SUBSTITUTIONS_H
#define _SUBSTITUTIONS_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
#ifdef __STRICT_ANSI__
char* strdup(const char* s);
int strcasecmp(char *s1, char *s2);
#endif
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

