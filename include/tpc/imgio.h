/*
  Copyright (c)  2007-2013 by Turku PET Centre

  imgio.h - combined definitions for imgio library

  Version:
  2007-02-02 Vesa Oikonen
  2009-02-16 VO
  2013-03-17 VO
  2013-04-02 VO
  2013-04-04 VO

*/
#ifndef _IMGIO_H
#define _IMGIO_H

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

/*===========================================================================*/
#include "img.h"
#include "analyze.h"
#include "imgdecay.h"
#include "sif.h"
#include "ecat63.h"
#include "img_sif.h"
#include "imgfile.h"
#include "imgunit.h"
#include "vol.h"
#include "ecat7.h"
#include "interfile.h"
#include "imgmax.h"
#include "micropet.h"
#include "nifti.h"
#include "imgmatch.h"
#include "libtpcimgiov.h"
/*===========================================================================*/
/* Backup file extension */
#ifndef BACKUP_EXTENSION
#define BACKUP_EXTENSION ".bak"
#endif 
/*===========================================================================*/

/*===========================================================================*/
#ifdef __cplusplus
}
#endif

#endif /* _IMGIO_H */

