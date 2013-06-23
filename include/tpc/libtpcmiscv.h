/******************************************************************************
  Copyright (c) 2004-2013 by Turku PET Centre

  libtpcmiscv.h
  
  Version:
  2004-11-07 Vesa Oikonen
  2004-01-20 Calle Laakkonen
  2005-04-25 Jarkko Johansson
  2013-04-17 VO
    Included stdio.h and stdlib.h.

******************************************************************************/
#ifndef _LIBTPCMISCV_H
#define _LIBTPCMISCV_H
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************/

/*****************************************************************************/
extern void libtpcmisc_print_readme(FILE *fp);
extern void libtpcmisc_print_history(FILE *fp);
extern void libtpcmisc_print_dreadme(FILE *fp);
extern void libtpcmisc_print_dhistory(FILE *fp);
extern void libtpcmisc_print_build(FILE *fp);
/*****************************************************************************/
extern void libtpcmisc_printdate(FILE *fp);
/*****************************************************************************/
#endif

