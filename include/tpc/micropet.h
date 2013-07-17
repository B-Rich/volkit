/******************************************************************************

  Copyright (c) 2009,2012 by Turku PET Centre

  micropet.h

  2009-02-16 VO
  2009-02-25 VO
  2012-02-04 VO
  2012-02-19 VO


******************************************************************************/
#ifndef _MICROPET_H
#define _MICROPET_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
#ifndef MAX_MICROPET_LINE_LEN
#define MAX_MICROPET_LINE_LEN 1024
#endif 
/*****************************************************************************/
extern int MICROPET_TEST;
/*****************************************************************************/
extern int upetHeaderReadParameter(FILE *fp, char *parameter, char *value);
extern int upetIsHeader(char *hdrfile);
extern int upetExists(const char *upetname, char *hdrfile, char *imgfile);
extern int upetGetImageDimensions(FILE *fp, int *z, int *x, int *y, int *f);
extern int upetScanStart(FILE *fp, time_t *scant);
extern int upetReadImagedata(FILE *fp, IFT *ift, int frame, float *data);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

