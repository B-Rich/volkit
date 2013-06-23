/******************************************************************************

  Copyright (c) 2005-2013 Turku PET Centre

  File:        sifio.c
  Description: Functions for reading and writing SIF format files.

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
  2005-01-15 Vesa Oikonen
    First created. Functions from sif.c.
  2005-01-16 VO
    studynr and isotope_name were added to SIF structure, and if available,
    can be read from/written in SIF file.
    sifRead() mostly rewritten.
    sifRead() does not calculate SIF weights.
  2005-04-26 CL
    Merged libsif to libtpcimgio
  2005-05-04 Jarkko Johansson
    Compatibility fix in function strftime().
  2006-10-31 VO
    Return value of mktime() is checked.
  2007-17-07 Harri Merisaari
    Modified for optional ANSi compatibility
  2007-09-10 VO
    Return value of localtime() is checked.
  2009-04-06 VO
    sifRead(): SIF file with only two columns is accepted, if that is correctly
    specified in title line. Comment about weights is corrected.
    sifWrite(): only frame times are saved if SIF column number tells that
    prompts and randoms are not available.
  2011-08-13 VO
    sifWrite() writes frame times with one decimal if rounding would lead to
    equal times in adjacent frames.
    sifPrint() prints frame times always with one decimal.


******************************************************************************/

/*****************************************************************************/
#include "sif.h"
/*****************************************************************************/

/*****************************************************************************/
/** Reads SIF file contents to the specified data structure.
    Weights are set to 1.
\return Returns 0 if ok, 1 invalid input, 2 failed to open file,
    3 failed to allocate memory, 4 header parsing error,
    5 wrong file type, 6 failed to allocate memory,
    8 parse error, 9 wrong data format
 */
int sifRead(
  /** SIF filename to be read */
  char *filename,
  /** Pointer to initiated SIF struct; any existing contents will be deleted */
  SIF *data
) {
  FILE *fp;
  int i, c, n, frameNr, yy, mm, dd, h, m, s, longest=0;
  struct tm *st;
  time_t timet;
  char *line, *cptr;


  if(SIF_TEST) printf("sifRead(%s, *sif)\n", filename);
  if(filename==NULL || data==NULL) return(1);
  /* Empty data */
  sifEmpty(data);

  /* Open file */
  fp=fopen(filename, "r");
  if(fp==NULL) {strcpy(siferrmsg, "cannot open file"); return(2);}

  /* Get the length of the longest line */
  i=0; while((c=fgetc(fp))!=EOF) {
    if(c==10) {if(i>longest) longest=i; i=0;} else i++;
  }
  if(i>longest) longest=i;
  rewind(fp); longest+=1;
  /* and allocate memory for string of that length */
  line=(char*)malloc((longest+1)*sizeof(char));
  if(line==NULL) {strcpy(siferrmsg, "out of memory"); fclose(fp); return(3);}

  /* Read the title line */
  do {
    if(fgets(line, longest, fp)==NULL) {
      strcpy(siferrmsg, "wrong format"); fclose(fp); free(line); return(4);
    }
    cptr=strpbrk(line, "\n\r"); if(cptr!=NULL) *cptr='\0';
    cptr=line+strspn(line, " \t");
  } while(*cptr=='#' || strlen(cptr)==0);
  n=sscanf(line, "%d/%d/%d %d:%d:%d %d %d %d %10s %7s",
      &dd, &mm, &yy, &h, &m, &s, &frameNr, &data->colNr, &data->version,
      data->studynr, data->isotope_name);
  if(n<9 || frameNr<1 || data->colNr<2 || data->version!=1) {
    strcpy(siferrmsg, "wrong filetype"); fclose(fp); free(line); return(5);}
  timet=time(NULL); st=localtime(&timet);
  if(st!=NULL) {
    st->tm_mday=dd; st->tm_mon=mm-1; st->tm_year=yy-1900;
    st->tm_hour=h; st->tm_min=m; st->tm_sec=s; st->tm_isdst=-1;
    data->scantime=mktime(st); if(data->scantime==-1) data->scantime=0;
  } else {
    data->scantime=0;
  }

  /* Allocate memory for data */
  if(sifSetmem(data, frameNr)) {
    fclose(fp); free(line); return(6);
  }

  /* Read data lines */
  i=0;
  while(i<data->frameNr) {
    do {
      if(fgets(line, longest, fp)==NULL) {
        strcpy(siferrmsg, "wrong format"); sifEmpty(data);
        fclose(fp); free(line); return(8);
      }
      cptr=strpbrk(line, "\n\r"); if(cptr!=NULL) *cptr='\0';
      cptr=line+strspn(line, " \t");
    } while(*cptr=='#' || strlen(cptr)==0);
    n=sscanf(line, "%lf %lf %lf %lf", &data->x1[i], &data->x2[i],
        &data->prompts[i], &data->randoms[i]);
    if(n<data->colNr || data->x2[i]<data->x1[i]) { 
      strcpy(siferrmsg, "wrong data format"); sifEmpty(data);
      fclose(fp); free(line); return(9);
    }
    i++;
  }

  /* Close file */
  fclose(fp); free(line);

  /* Calculate trues */
  if(data->colNr>=4) for(i=0; i<data->frameNr; i++)
    data->trues[i]=data->prompts[i]-data->randoms[i];
  /* Set weights to 1.0 */
  for(i=0; i<data->frameNr; i++) data->weights[i]=1.0;

  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/** Write SIF data to a standard SIF file, emptying files old contents.
\return Returns 0 if successful, 1 invalid input, 2 failed to open file, 
    3 failed to write into file.
 */
int sifWrite(
  /** Pointer to SIF struct containing data to be written in file */
  SIF *data,
  /** Filename for SIF; file is overwritten without backup */
  char *filename
) {
  FILE *fp;
  int i, n, req_decimals=0;
  char buf[1024];
  struct tm *st;


  if(SIF_TEST) printf("sifWrite(*sif, %s)\n", filename);
  /* Check data */
  if(data->frameNr<1) {strcpy(siferrmsg, "no data to save"); return 1;}

  /* Open file */
  fp=fopen(filename, "w");
  if(fp==NULL) {strcpy(siferrmsg, "cannot open file"); return 2;}

  /* Write title line */
  st=localtime(&data->scantime);
  if(st!=NULL) strftime(buf, 1024, "%d/%m/%Y %H:%M:%S", st);
  else strcpy(buf, "1/1/1900 00:00:00");
  n=fprintf(fp, "%s %d %d %d", buf, data->frameNr, data->colNr, data->version);
  if(SIF_TEST)
    printf("%s %d %d %d\n", buf, data->frameNr, data->colNr, data->version);
  if(n<7) {strcpy(siferrmsg, "cannot write file"); fclose(fp); return 2;}
  if(strlen(data->studynr)!=0 || strlen(data->isotope_name)!=0) {
    /* Write also study number and isotope */
    if(strlen(data->studynr)==0) fprintf(fp, " .");
    else fprintf(fp, " %s", data->studynr);
    if(strlen(data->isotope_name)>0) fprintf(fp, " %s", data->isotope_name);
  }
  fprintf(fp, "\n");

  /* Check if frame times need to printed with decimals */
  for(i=1, req_decimals=0; i<data->frameNr; i++) {
    if(round(data->x1[i])==round(data->x1[i-1])) {req_decimals=1; break;}
    if(round(data->x2[i])==round(data->x2[i-1])) {req_decimals=1; break;}
  }

  /* Write data lines */
  for(i=0; i<data->frameNr; i++) {
    if(req_decimals==0) n=fprintf(fp, "%.0f %.0f", data->x1[i], data->x2[i]);
    else n=fprintf(fp, "%.1f %.1f", data->x1[i], data->x2[i]);
    if(n<3) {strcpy(siferrmsg, "cannot write file"); fclose(fp); return 3;}
    if(data->colNr<=2) n=fprintf(fp, "\n");
    else n=fprintf(fp, " %.0f %.0f\n", data->prompts[i], data->randoms[i]);
    if(n<1) {strcpy(siferrmsg, "cannot write file"); fclose(fp); return 3;}
  }

  /* Close file */
  fclose(fp);

  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/** Prints to stdout the contents of SIF data structure.
 */
void sifPrint(
  /** Pointer to SIF struct */
  SIF *data
) {
  int i;
  char buf[512];
  struct tm *st;
  
  st=localtime(&data->scantime);
  if(st!=NULL) strftime(buf, 512, "%Y-%m-%d %H:%M:%S", st);
  else strcpy(buf, "1900-01-01 00:00:00");
  printf("Scan time: %s\n", buf);
  printf("Isotope: %s\n", data->isotope_name);
  printf("Frame start   end      Prompts    Randoms      Trues   Weight\n");
  for(i=0; i<data->frameNr; i++) {
    printf(" %03d %6.1f %6.1f  %10.0f %10.0f %10.0f %8.6f\n", i+1,
      data->x1[i], data->x2[i], data->prompts[i], data->randoms[i],
      data->trues[i], data->weights[i]);
  }
  return;
}
/*****************************************************************************/

/*****************************************************************************/

