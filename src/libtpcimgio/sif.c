/******************************************************************************

  File:        sif.c
  Description: Routines for Scan Information Files (SIF).

  Copyright (c) 2000-2013 Turku PET Centre

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details:
  http://www.gnu.org/copyleft/lesser.html

  You should have received a copy of the GNU Lesser General Public License
  along with this library/program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi

  Modification history:
  2000-09-04 Vesa Oikonen
  2000-09-08 VO
    malloc->calloc.
  2000-09-18 VO
    weightSIF() can calculate weights for decay corrected data.
  2000-12-13 VO
    Included function writeSIF().
  2002-07-30 VO
    memset() added to initSIF().
  2004-09-17 VO
    Doxygen style comments.
  2004-10-13 VO
    tm_isdst=-1 (unknown Daylight saving time).
  2005-01-12 VO
    Changed and additional comments not affecting compiled code.
  2005-01-15 VO
    SIF related stuff moved from libpet to new libsif.
    Function names changed, although old names can still be used.
  2005-01-16 VO
    studynr and isotope_name were added to SIF structure.
  2005-04-26 CL
    Merged libsif to libtpcimio

******************************************************************************/

/*****************************************************************************/
#include "sif.h"
/*****************************************************************************/

/*****************************************************************************/
char siferrmsg[128];
/*****************************************************************************/
int SIF_TEST = 0;
/*****************************************************************************/
/*!
 * Initiate SIF structure. This should be called once before first use.
 *
 * @param data pointer to sif data
 */
void sifInit(SIF *data) {
  if(SIF_TEST) printf("sifInit()\n");
  memset(data, 0, sizeof(SIF));
  data->frameNr=data->colNr=0;
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Free memory allocated for SIF. All contents are destroyed.
 *
 * @param data ponter to sif data
 */
void sifEmpty(SIF *data) {
  if(SIF_TEST) printf("sifEmpty()\n");
  if(data->frameNr>0) {
    free((char*)(data->x1)); free((char*)(data->x2));
    free((char*)(data->prompts)); free((char*)(data->randoms));
    free((char*)(data->trues)); free((char*)(data->weights));
    data->frameNr=data->colNr=0;
  }
  data->scantime=(time_t)0; data->version=0;
  strcpy(data->studynr, ""); strcpy(data->isotope_name, "");
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Allocates memory for SIF data. Old data is destroyed.
 *
 * @param data pointer to SIF data allocated here
 * @param frameNr number of PET time frames
 * @return 0 if ok, 1 failed memory allocation
 */
int sifSetmem(SIF *data, int frameNr) {
  if(SIF_TEST) printf("sifSetmem()\n");
  /* Clear previous data, if necessary */
  if(data->frameNr>0) sifEmpty(data);
  if(frameNr<1) return(0);
  
  /* Allocate memory */
  data->x1=(double*)calloc(frameNr, sizeof(double));
  data->x2=(double*)calloc(frameNr, sizeof(double));
  data->prompts=(double*)calloc(frameNr, sizeof(double));
  data->randoms=(double*)calloc(frameNr, sizeof(double));
  data->trues=(double*)calloc(frameNr, sizeof(double));
  data->weights=(double*)calloc(frameNr, sizeof(double));
  if(data->x1==NULL || data->x2==NULL || data->prompts==NULL ||
     data->randoms==NULL || data->trues==NULL || data->weights==NULL) {
    strcpy(siferrmsg, "out of memory"); return(1);}
  data->frameNr=frameNr;

  return(0);
}
/*****************************************************************************/

/*****************************************************************************/

