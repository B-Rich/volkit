/******************************************************************************
  Copyright (c)  1993-2008 by Turku PET Centre

  intex.h - definitions for intex.c

  Version:
  1996-09-18 Vesa Oikonen
  2002-02-17 VO
  2003-07-17 Calle Laakkonen
  2008-07-11 VO
    Added the externs.

******************************************************************************/
#ifndef _INTEX_H
#define _INTEX_H
/*****************************************************************************/
/* Integer list */
typedef struct {
  int nr;
  int *i;
} INT_list;
/*****************************************************************************/
extern int intExpand(char *text, INT_list *list);
extern INT_list intMerge(INT_list *list1,INT_list *list2);
extern int _intexadd(INT_list *list, int a);
/*****************************************************************************/
#endif

