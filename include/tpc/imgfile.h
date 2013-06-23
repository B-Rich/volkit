/******************************************************************************
  Copyright (c)  2003-2013 by Turku PET Centre

  imgfile.h - definitions for imgfile.c

  Version:
  2003-07-17 Vesa Oikonen
  2003-07-21 VO
  2003-07-25 VO
  2003-07-26 VO
  2003-09-04 VO
  2003-10-04 VO
  2004-09-24 VO
  2006-10-30 VO
  2007-01-29 VO
  2007-03-25 VO
  2012-02-04 VO
  2012-02-19 VO
  2012-10-03 VO
  2013-03-17 VO
  2013-03-26 VO
  2013-04-01 VO
  2013-04-04 VO


******************************************************************************/
#ifndef _IMGFILE_H
#define _IMGFILE_H
/*****************************************************************************/
#include "img.h"
#include "ecat7.h"
#include "ecat63.h"
#include "analyze.h"
#include "micropet.h"
#include "nifti.h"
#include "imgunit.h"
#include "sif.h"
/*****************************************************************************/
extern int imgRead(const char *fname, IMG *img);
extern int imgReadMainHeader(const char *fname, IMG *img);
extern int imgWrite(const char *fname, IMG *img);
extern int imgReadHeader(const char *fname, IMG *img);
extern int imgReadNextFrame(char *fname, IMG *img);
extern int imgReadFrame(
  const char *fname, int frame_to_read, IMG *img, int frame_index);
extern int imgWriteFrame(
  const char *fname, int frame_to_write, IMG *img, int frame_index);
extern void imgFormatFromFName(IMG *img, const char *fname);
extern int imgFormatDetermine(const char *fname, char *basename, char *hdrfile,
  char *imgfile, char *siffile, int *file_format, int *scanner, int *type,
  int *modality, int verbose
);
/*****************************************************************************/
extern int ecat63ReadAllToImg(const char *fname, IMG *img);
extern int ecat63WriteAllImg(const char *fname, IMG *img);
extern int ecat63ReadPlaneToImg(const char *fname, IMG *img);
extern int ecat63AddImg(const char *fname, IMG *img);
extern void imgGetEcat63MHeader(IMG *img, ECAT63_mainheader *h);
extern void imgSetEcat63MHeader(IMG *img, ECAT63_mainheader *h);
extern int imgEcat63Supported(ECAT63_mainheader *h);
extern int imgGetEcat63Fileformat(ECAT63_mainheader *h);
extern int imgReadEcat63Header(const char *fname, IMG *img);
extern int imgReadEcat63FirstFrame(const char *fname, IMG *img);
extern int imgReadEcat63Frame(
  const char *fname, int frame_to_read, IMG *img, int frame_index);
extern int imgWriteEcat63Frame(
  const char *fname, int frame_to_write, IMG *img, int frame_index);
extern void imgSetEcat63SHeader(IMG *img, void *h);
/*****************************************************************************/
extern int imgReadEcat7(const char *fname, IMG *img);
extern int imgWriteEcat7(const char *fname, IMG *img);
extern int imgWrite2DEcat7(const char *fname, IMG *img);
extern int imgWritePolarmap(const char *fname, IMG *img);
/*****************************************************************************/
extern void imgGetEcat7MHeader(IMG *img, ECAT7_mainheader *h);
extern void imgSetEcat7MHeader(IMG *img, ECAT7_mainheader *h);
extern int imgReadEcat7Header(const char *fname, IMG *img);
extern int imgEcat7Supported(ECAT7_mainheader *h);
extern int imgReadEcat7Frame(
  const char *fname, int frame_to_read, IMG *img, int frame_index);
extern int imgReadEcat7FirstFrame(const char *fname, IMG *img);
extern int imgGetEcat7Fileformat(ECAT7_mainheader *h);
extern int imgWriteEcat7Frame(
  const char *fname, int frame_to_write, IMG *img, int frame_index);
extern void imgSetEcat7SHeader(IMG *img, void *h);
/*****************************************************************************/
extern int imgReadAnalyze(const char *dbname, IMG *img);
extern int imgWriteAnalyze(const char *dbname, IMG *img);
extern int imgReadAnalyzeHeader(const char *dbname, IMG *img);
extern int imgGetAnalyzeHeader(IMG *img, ANALYZE_DSR *h);
extern int imgSetAnalyzeHeader(
  IMG *img, const char *dbname, ANALYZE_DSR *h, float fmin, float fmax);
extern int imgReadAnalyzeFrame(
  const char *dbname, int frame_to_read, IMG *img, int frame_index);
extern int imgReadAnalyzeFirstFrame(const char *fname, IMG *img);
extern int imgWriteAnalyzeFrame(
  const char *fname, int frame_to_write, IMG *img, int frame_index,
  float fmin, float fmax
);
/*****************************************************************************/
extern int imgMicropetToEcat7(char *upetname, char *ecatfile, int verbose);
extern int imgMicropetPETToEcat7(
  FILE *fph, FILE *fpi, char *ecatfile, int verbose);
extern int imgMicropetCTToEcat7(
  FILE *fph, FILE *fpi, char *ecatfile, int verbose);
extern int imgGetMicropetMainHeader(
  FILE *fp, IMG *img, float *calibration_factor);
extern int imgGetMicropetFrameHeader(FILE *fp, IMG *img, int frame_index);
extern int imgGetMicropetSIF(FILE *fp, SIF *sif);
extern int imgGetMicropetHeader(IMG *img);
extern int imgReadMicropetHeader(const char *dbname, IMG *img);
extern int imgReadMicropetFrame(const char *fname, int frame_to_read,
  IMG *img, int frame_index);
extern int imgReadMicropetFirstFrame(const char *fname, IMG *img);
extern int imgReadMicropet(const char *fname, IMG *img);
/*****************************************************************************/
extern int imgReadNifti(const char *filename, IMG *img, int verbose);
extern int imgReadNiftiFirstFrame(const char *filename, IMG *img, int verbose);
extern int imgReadNiftiHeader(const char *filename, IMG *img, int verbose);
extern int imgGetNiftiHeader(IMG *img, NIFTI_DSR *h, int verbose);
extern int imgReadNiftiFrame(
  const char *filename, int frame_to_read, IMG *img, int frame_index,
  int verbose
);
extern int imgSetNiftiHeader(
  IMG *img, const char *dbname, NIFTI_DSR *dsr, float fmin, float fmax,
  int verbose
);
extern int imgWriteNiftiFrame(
  const char *dbname, int frame_to_write, IMG *img, int frame_index,
  float fmin, float fmax, int verbose
);
extern int imgWriteNifti(
  const char *dbname, IMG *img, int save_sif, int verbose
);
/*****************************************************************************/
#endif /* _IMGFILE_H */

