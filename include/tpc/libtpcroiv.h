/******************************************************************************
  Copyright (c) 2004-2013 by Turku PET Centre

  libtpcroiv.h
  
  Version:
  2004-11-07 Vesa Oikonen
  2004-01-20 Calle Laakkonen
  2005-04-26 Jarkko Johansson
  2013-02-12 VO

******************************************************************************/
#ifndef _LIBTPCROIV_H
#define _LIBTPCROIV_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
extern void libtpcroi_print_readme(FILE *fp);
extern void libtpcroi_print_history(FILE *fp);
extern void libtpcroi_print_dreadme(FILE *fp);
extern void libtpcroi_print_dhistory(FILE *fp);
extern void libtpcroi_print_build(FILE *fp);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

