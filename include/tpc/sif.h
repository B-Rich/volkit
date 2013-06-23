/******************************************************************************

  Copyright (c) 2000-2013 Turku PET Centre

  sif.h

  Modification history:
  2000-12-13 Vesa Oikonen
  2004-08-23 VO
    Doxygen style comments.
    Functions "renamed" with defines.
  2005-01-12 VO
    Changed and additional comments not affecting compiled code.
  2005-01-15 VO
    SIF related stuff moved from libpet to new libsif.
  2005-01-16 VO
    studynr and isotope_name were added to SIF structure.
  2013-04-04 VO
    Added sifExistentCounts().


******************************************************************************/
#ifndef _SIF_H
#define _SIF_H
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
/*****************************************************************************/
extern char siferrmsg[];
/*****************************************************************************/
extern int SIF_TEST;
/*****************************************************************************/
/** Scan Information File Data structure */
typedef struct {
  /** Scan time */
  time_t scantime;
  /** Number of frames */
  int frameNr;
  /** Number of columns (usually 4) */
  int colNr;
  /** SIF version */
  int version;
  /** Studynumber */
  char studynr[11];
  /** String representation of the isotope */
  char isotope_name[8];
  /** Frame start time (sec) */
  double *x1;
  /** Frame end time (sec) */
  double *x2;
  /** Prompts */
  double *prompts;
  /** Randoms */
  double *randoms;
  /** Trues = Prompts-randoms, but at least 1 */
  double *trues;
  /** Weights = (Frame duration)^2 / trues */
  double *weights;
} SIF;
/*****************************************************************************/
extern void libsif_printdate(FILE *fp);
extern int sifRead(char *filename, SIF *data);
extern int sifWrite(SIF *data, char *filename);
extern void sifPrint(SIF *data);
extern void sifEmpty(SIF *data);
extern void sifInit(SIF *data);
extern int sifSetmem(SIF *data, int frameNr);
extern void sifWeight(SIF *data, double halflife);
extern void sifModerate(SIF *sif, double limit);
extern int sifExistentCounts(SIF *sif);
/*****************************************************************************/
#define readSIF sifRead
#define writeSIF sifWrite
#define printSIF sifPrint
#define emptySIF sifEmpty
#define weightSIF sifWeight
#define initSIF sifInit
#define setmemSIF sifSetmem
/*****************************************************************************/
#endif

