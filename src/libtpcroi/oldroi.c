/******************************************************************************
  Library file:  oldroi.c
  Description:   Deprecated ROI functions; may need to be updated before use.

  Copyright (c) Turku PET Centre 1996-2005

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

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
  2005-01-20 CL
    Split the roi.c file to roi.c and oldroi.c
  2013-02-12 VO
    Deleted an unused variable.
    

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
/*****************************************************************************/
#include "libtpcmisc.h"
/*****************************************************************************/
#include "oldroi.h"
/*****************************************************************************/

/*****************************************************************************/
/** If ind=0, all ROIs in list are deleted keeping memory allocated,
 *  else only one ROI is deleted (ind=1, first ROI) keeping memory allocated.
 */
void roiDel(ROI_list *rl, int ind)
{
  int  j,r;
  
  if(ind==0){
    for(r=0; r<rl->nr; r++) {
      if(rl->roi[r].point_nr>0) {
        free(rl->roi[r].x);
        free(rl->roi[r].y);
        rl->roi[r].point_nr=0;
      }
    }
    rl->nr=0;
  }
  else {
     if(ind>rl->nr) return;
     /* free x and y for given ROI */
     if(rl->roi[ind-1].point_nr>0) {
        free(rl->roi[ind-1].x);
        free(rl->roi[ind-1].y);
        rl->roi[ind-1].point_nr=0;
     }
     /* copy memory */
     for(j=ind; j<rl->nr; j++)
	 memcpy(&rl->roi[j-1], &rl->roi[j], sizeof(ROI));
     rl->nr--;
  }
                     
}

/*****************************************************************************/
/** Adds ROI file contents (all ROIs) to specified data structure.
\return Returns 0, if OK.
 */
int roiRead(const char *fname, ROI_list *rl)
{
  int    i, j, n, ch;
  FILE  *fp;
  char   imgfile[FILENAME_MAX], roiname[FILENAME_MAX];
  float  zoom, recon_zoom;
  int    matrix, type, status, pos_x, pos_y, w, h, t, roi, npts, x, y;

  /* Open ROI file */
  if((fp=fopen(fname, "r")) == NULL) {
    strcpy(roierrmsg, "cannot open file");
    return(1);
  }

  n=0;
  while(1) {
    /* Read first '*' */
    ch=fgetc(fp); if(ch==EOF || ch!='*') break;

    /* Read image filename */
    if(fscanf(fp, "%s ", imgfile)==EOF) {
      strcpy(roierrmsg, "cannot read image filename");
      fclose(fp); return(3);
    }

    /* Read zooms */
    if(fscanf(fp, "%f %f ", &zoom, &recon_zoom)==EOF) {
      strcpy(roierrmsg, "cannot read zoom values");
      fclose(fp); return(4);}
    if(recon_zoom==0.0) recon_zoom=1.0;

    /* Read matrix -> t */
    if(fscanf(fp, "%d %d %d %d %d %d %d %d ", &matrix, &type, &status,
         &pos_x, &pos_y, &w, &h, &t)==EOF) {
      strcpy(roierrmsg, "cannot read ROI definitions");
      fclose(fp); return(5);
    }

    /* Read ROI nr and name */
    if(fscanf(fp, "%d ", &roi)==EOF) {
      strcpy(roierrmsg, "cannot read ROI number");
      fclose(fp); return(6);
    }
    for(i=j=0; j<3; i++) {
      roiname[i]=(char)getc(fp);
      if(roiname[i]=='/') j++; else j=0;
    }
    j=i-3; roiname[j]=(char)0;
    /* delete spaces from ROI name */
    j=strlen(roiname);
    for(i=j-1; i>=0; i--)
      if(isspace((int)roiname[i])) roiname[i]=(char)0; else break;
    j=i; for(i=0; i<j; i++)
      if(!isspace((int)roiname[i])) {if(i>0) strcpy(roiname, roiname+i); break;}

    /* Read the number of points */
    (void)getc(fp); (void)getc(fp); /* skip 0 and space */
    if(fscanf(fp, "%d ", &npts)==EOF) {
      strcpy(roierrmsg, "cannot read nr of ROI border points");
      fclose(fp); return(8);
    }

    /* Allocate memory for ROI */
    if(rl->_allocNr==0){ rl->roi=malloc(sizeof(ROI)); rl->_allocNr++;}
    else if(rl->_allocNr<=rl->nr){
       rl->roi=(ROI*)realloc((char*)rl->roi, (rl->nr+1)*sizeof(ROI));
       rl->_allocNr++;
    }
    if(rl->roi==NULL) {
      strcpy(roierrmsg, "out of memory");
      fclose(fp); rl->nr=0; return(11);
    } 
    

    /* Copy data */ 
    strcpy(rl->roi[rl->nr].imgfile, imgfile);
    rl->roi[rl->nr].zoom = zoom;
    rl->roi[rl->nr].recon_zoom = recon_zoom;
    rl->roi[rl->nr].matnum = matrix;
    rl->roi[rl->nr].type = type;
    rl->roi[rl->nr].status = status;
    rl->roi[rl->nr].pos_x = pos_x;
    rl->roi[rl->nr].pos_y = pos_y;
    rl->roi[rl->nr].w = w;
    rl->roi[rl->nr].h = h;
    rl->roi[rl->nr].t = t;
    rl->roi[rl->nr].roi = roi;
    strcpy(rl->roi[rl->nr].roiname, roiname);
    rl->roi[rl->nr].point_nr = npts; 
    rl->roi[rl->nr].userdata=NULL;
    rl->nr++;
    n++;

    /* If TRACE ROI, read points */
    if(type==ROI_TRACE) {
      if((rl->roi[rl->nr-1].x=malloc((npts+1)*sizeof(int)))==NULL) {
        strcpy(roierrmsg, "cannot read ROI border definition");
        fclose(fp); rl->nr--; return(12);
      }
      if((rl->roi[rl->nr-1].y=malloc((npts+1)*sizeof(int)))==NULL) {
        strcpy(roierrmsg, "cannot read ROI border definition");
        fclose(fp); rl->nr--; free((char*)rl->roi[rl->nr-1].x); return(13);
      }
      for(i=0; i<npts; i++)
        if(fscanf(fp, "%d %d", &x, &y) == EOF) {
          strcpy(roierrmsg, "cannot read ROI border definition");
          free((char*)rl->roi[rl->nr-1].x); free((char*)rl->roi[rl->nr-1].y);
          fclose(fp); rl->nr--; return(9);
        } else {
          rl->roi[rl->nr-1].x[i]=x; rl->roi[rl->nr-1].y[i]=y;
        }
      do {ch=getc(fp);} while(ch!=EOF && ch!='\n');
      /* make sure that ROI start and end are connected (extra memory was allocated previously) */
      if(rl->roi[rl->nr-1].x[npts-1]!=0 || rl->roi[rl->nr-1].y[npts-1]!=0) {
        rl->roi[rl->nr-1].x[npts]=0;
        rl->roi[rl->nr-1].y[npts]=0;
        npts++; rl->roi[rl->nr-1].point_nr++;
      }
    }
  }
  fclose(fp);
  if(n==0) {
    strcpy(roierrmsg, "error in ROI file");
    return(2);
  }
  /* Make points for others than trace ROIs */
  for(i=rl->nr-n; i<rl->nr; i++) {
    switch(rl->roi[i].type) {
      case ROI_RECTANGULAR:	j=roiComputeRect(&rl->roi[i]); break;
      case ROI_CIRCULAR:	j=roiComputeCirc(&rl->roi[i]); break;
      case ROI_ELLIPSE:		j=roiComputeElli(&rl->roi[i]); break;
      case ROI_TRACE:		j=0; break;
      default:			j=-1;
    }
    if(j) {
      strcpy(roierrmsg, "cannot read ROI border definition");
      rl->nr-=n; return(21);
    }
  }
  return(0);
}
/*****************************************************************************/

int roiComputeCirc(ROI *roi)
{
  int i, d, x, y, n, r, s;

  r=roi->w;
  if((roi->x=malloc(sizeof(int)))==NULL) return(-1);
  if((roi->y=malloc(sizeof(int)))==NULL) {free((char*)roi->x); return(-1);}
  roi->x[0]=0; roi->y[0]=r;
  if(r==0) n=1; else {
    for(x=1, y=r, d=3-2*r, n=1; x<=y; x++) {
      if(d<0) d=d+4*x+6; else {d=d+4*(x-y)+10; y-=1;}
      if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
        return(-1);
      if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
        free((char*)roi->x); free((char*)roi->y); return(-1);}
      roi->x[n]=x; roi->y[n]=y;
      n++;
    }
  }
  for(i=n-1, s=i; i>0; i--) {
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return(-1);
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return(-1);}
    roi->x[n]=roi->y[--s]; roi->y[n++]=roi->x[s];
  }
  for(i=n-1, s=i; i>0; i--) {
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return(-1);
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return(-1);}
    roi->x[n]=roi->x[--s]; roi->y[n++]=-roi->y[s];
  }
  for(i=n-1, s=i; i>0; i--) {
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return(-1);
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return(-1);}
    roi->x[n]=-roi->x[--s]; roi->y[n++]=roi->y[s];
  }
  roi->point_nr=n;

  return(0);
}

int roiComputeElli(ROI *roi)
{
  int i, ix, iy, /*ox,*/ oy, n, s, w, h;

  w=roi->w; h=roi->h;
  if((roi->x=malloc(sizeof(int)))==NULL) return(-1);
  if((roi->y=malloc(sizeof(int)))==NULL) {free((char*)roi->x); return(-1);}
  roi->x[0]=0; roi->y[0]=h;
  n=1; oy=h;
  for(ix=1; ix<w; ix++) {
    iy=(jsqrt(h+h*h*(w-ix)*(w+ix)))/w; if(oy-iy>1) break;
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return -1;
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return -1;}
    roi->x[n]=ix; roi->y[n++]=iy;
    oy=iy;
  }
  /*ox=ix;*/
  for(iy=oy-1; iy>=0; iy--) {
    ix=(jsqrt(w+w*w*(h-iy)*(h+iy)))/h;
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return -1;
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return -1;}
    roi->x[n]=ix; roi->y[n++]=iy;
    /*ox=ix;*/
  }
  for(i=n-1, s=i; i>0; i--) {
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return -1;
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return -1;}
    roi->x[n]=roi->x[--s]; roi->y[n++]=-roi->y[s];
  }
  for(i=n-1, s=i; i>0; i--) {
    if((roi->x=(int*)realloc((char*)roi->x, (n+1)*sizeof(int)))==NULL)
      return -1;
    if((roi->y=(int*)realloc((char*)roi->y, (n+1)*sizeof(int)))==NULL) {
      free((char*)roi->x); free((char*)roi->y); return -1;}
    roi->x[n]=-roi->x[--s]; roi->y[n++]=roi->y[s];
  }
  roi->point_nr=n;

  return(0);
}

/*****************************************************************************/
/** Fills the gaps between ROI points; join them with new points.
 *  Allocates memory for new point data in fx and fy.
\return Returns the number of new points or =< 0, if error.
 */
int roiFillGaps(int *x, int *y, int nr, int **ffx, int **ffy)
{
  int i, j, x1, x2=0, y1, y2=0, dx, dy, nx, ny, *fx, *fy, n;
  float k;

  /* Check data */
  if(nr<1) return(0);

  /* Allocate memory for new data */
  if((fx=malloc(nr*sizeof(int)))==NULL) return -1;
  if((fy=malloc(nr*sizeof(int)))==NULL) {free((char*)fx); return -1;}

  /* Process from first to last (last->first is done already) */
  for(i=1, n=j=0; i<nr; i++) {
    x1=x[i-1]; x2=x[i]; y1=y[i-1]; y2=y[i]; dx=x2-x1; dy=y2-y1;
    fx[j]=x1; fy[j++]=y1;
    if(abs(dx)<2 && abs(dy)<2) continue;
    if(abs(dx)>=abs(dy)) {
      k=(float)dy/(float)dx;
      if(x2>x1)
        for(nx=x1+1; nx<x2; nx++) {
          ny=y1+(int)temp_roundf(k*(nx-x1));
          fx[j]=nx; fy[j++]=ny; n++;
          if((fx=(int*)realloc((char*)fx, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fy); return -1;}
          if((fy=(int*)realloc((char*)fy, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fx); return -1;}
        }
      else
        for(nx=x1-1; nx>x2; nx--) {
          ny=y1+(int)temp_roundf(k*(nx-x1));
          fx[j]=nx; fy[j++]=ny; n++;
          if((fx=(int*)realloc((char*)fx, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fy); return -1;}
          if((fy=(int*)realloc((char*)fy, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fx); return -1;}
        }
    } else {
      k=(float)dx/(float)dy;
      if(y2>y1)
        for(ny=y1+1; ny<y2; ny++) {
          nx=x1+(int)temp_roundf(k*(ny-y1));
          fx[j]=nx; fy[j++]=ny; n++;
          if((fx=(int*)realloc((char*)fx, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fy); return -1;}
          if((fy=(int*)realloc((char*)fy, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fx); return -1;}
        }
      else
        for(ny=y1-1; ny>y2; ny--) {
          nx=x1+(int)temp_roundf(k*(ny-y1));
          fx[j]=nx; fy[j++]=ny; n++;
          if((fx=(int*)realloc((char*)fx, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fy); return -1;}
          if((fy=(int*)realloc((char*)fy, (nr+n)*sizeof(int)))==NULL) {
            free((char*)fx); return -1;}
        }
    }
  }
  fx[j]=x2; fy[j++]=y2; n=j;
  *ffx=fx; *ffy=fy;
  return(n);
}

/*****************************************************************************/
/** The matrix is filled with 0's (outside of ROI), 1's (on the ROI border),
 *  and 2's (inside the ROI border).
 *  If ROI extends outside image borders, those points are ignored.
 *  Matrix coordinates are up-to-bottom and left-to-right.
\return Function returns 0, if everything is OK.
 */
int roiOnOff(
  /** Definitions for one region-of-interest */
  ROI *roi,
  /** Image matrix x dimension */
  int dimx,
  /** Image matrix y dimension */
  int dimy,
  /** Image reconstruction zoom */
  float zoom,
  /** Allocated memory for output matrix[dimx][dimy] */
  char **m
) {
  int i, j, n, *x, *y, pos_x, pos_y, *fx, *fy;
  float f;


  /* Check the parameters */
  if(roi==NULL || dimx<2 || dimy<2 || zoom<1.0e-6 || m==NULL) {
    strcpy(roierrmsg, "invalid arguments for roiOnOff()");
    return(1);
  }

  /* Make scaled ROI points */
  strcpy(roierrmsg, "out of memory");
  if((x=malloc(roi->point_nr*sizeof(int)))==NULL) return(2);
  if((y=malloc(roi->point_nr*sizeof(int)))==NULL) {
    free((char*)x); return(2);}
  f=roi->zoom; if(f<=0.0) f=1.0;
  pos_x=(int)temp_roundf(((float)roi->pos_x)/f);
  pos_y=(int)temp_roundf(((float)roi->pos_y)/f);
  f=roi->zoom*(roi->recon_zoom/zoom); if(f<=0.0) f=1.0;
  for(i=0; i<roi->point_nr; i++) {
    x[i]=pos_x+(int)temp_roundf((float)roi->x[i]/f);
    y[i]=pos_y+(int)temp_roundf((float)roi->y[i]/f);
  }

  /* Fill the gaps between points */
  n=roiFillGaps(x, y, roi->point_nr, &fx, &fy);
  if(n<1) {
    strcpy(roierrmsg, "cannot fill the gaps in ROI border");
    free((char*)x); free((char*)y); return(3);
  }

  /* Set matrix */
  for(i=0; i<dimy; i++) for(j=0; j<dimx; j++) m[j][i]=(char)0;
  for(i=0; i<n; i++) if(fx[i]>=0 && fx[i]<dimx && fy[i]>=0 && fy[i]<dimy)
    m[fx[i]][fy[i]]=(char)1;

  /* Fill the inside of ROI */
  for(i=0; i<dimy; i++) if(m[0][i]==0) m[0][i]=2; else break;
  for(i=dimy-1; i>=0; i--) if(m[0][i]==0) m[0][i]=2; else break;
  for(i=0; i<dimy; i++) if(m[dimx-1][i]==0) m[dimx-1][i]=2; else break;
  for(i=dimy-1; i>=0; i--) if(m[dimx-1][i]==0) m[dimx-1][i]=2; else break;
  for(i=0; i<dimx; i++) if(m[i][0]==0) m[i][0]=2; else break;
  for(i=dimx-1; i>=0; i--) if(m[i][0]==0) m[i][0]=2; else break;
  for(i=0; i<dimx; i++) if(m[i][dimy-1]==0) m[i][dimy-1]=2; else break;
  for(i=dimx-1; i>=0; i--) if(m[i][dimy-1]==0) m[i][dimy-1]=2; else break;
  for(i=0; i<dimy; i++) for(j=0; j<dimx; j++) {
    if(m[j][i]!=2) continue;
    if(i>0 && m[j][i-1]==0) {m[j][i-1]=2; j=-1; i-=2; continue;}
    if(j>0 && m[j-1][i]==0) {m[j-1][i]=2; j-=2; continue;}
    if(j+1<dimx) {if(m[j+1][i]!=1) m[j+1][i]=2;}
    if(i+1<dimy) {if(m[j][i+1]!=1) m[j][i+1]=2;}
    m[j][i]=3;
  }
  for(i=0; i<dimy; i++) for(j=0; j<dimx; j++)
    if(m[j][i]==0) m[j][i]=2; else if(m[j][i]>1) m[j][i]=0;
    //if(m[j][i]<2) m[j][i]=1; else m[j][i]=0;

  /* Free memory */
  free((char*)x); free((char*)y);
  free((char*)fx); free((char*)fy);

  return(0);
}

/*****************************************************************************/
/*
 *  int roiSave(char *fname, ROI_list *roilist)
 */
int roiSave(const char *fname, ROI_list *rl)
{
  int i, j;
  FILE *fp;

  /* Check arguments */
  if(!fname[0] || rl==NULL || rl->nr<1) {
    strcpy(roierrmsg, "invalid arguments for roiSave()");
    return(1);
  }

  /* Create file */
  if((fp=fopen(fname, "w"))==NULL) {
    strcpy(roierrmsg, "cannot open file");
    return(2);
  }

  for(i=0; i<rl->nr; i++) {
    /* Write first line */
    if(fprintf(fp, "*%s %f %f %d %d %d %d %d %d %d %d %d %s///%d %d\n",
      rl->roi[i].imgfile, rl->roi[i].zoom, rl->roi[i].recon_zoom,
      rl->roi[i].matnum, rl->roi[i].type, rl->roi[i].status,
      rl->roi[i].pos_x, rl->roi[i].pos_y, rl->roi[i].w, rl->roi[i].h,
      rl->roi[i].t, rl->roi[i].roi, rl->roi[i].roiname,
      0, rl->roi[i].point_nr ) < 10 ) {
        strcpy(roierrmsg, "cannot write data");
        fclose(fp); return(3);
    }
    /* Write second line, if trace ROI */
    if(rl->roi[i].type!=ROI_TRACE) continue;
    for(j=0; j<rl->roi[i].point_nr; j++)
      fprintf(fp, "%d %d ", rl->roi[i].x[j], rl->roi[i].y[j]);
    fprintf(fp, "\n");
  }

  /* Close file */
  fclose(fp);

  return(0);
}

/*****************************************************************************/
/*
 *  int roiAppend(char *fname, ROI_list *roilist)
 */
int roiAppend(char *fname, ROI_list *rl, int ind)
{
  int i, j;
  FILE *fp;

  /* Check arguments */
  if(!fname[0] || rl==NULL || rl->nr<1) {
    strcpy(roierrmsg, "invalid arguments for roiAppend()");
    return(1);
  }

  /* Create file */
  if((fp=fopen(fname, "a"))==NULL) {
    strcpy(roierrmsg, "cannot open file");
    return(2);
  }

  /* Check ind, which roi to append to file */
  if(ind==0){
        for(i=0; i<rl->nr; i++) {
           /* Write first line */
           if(fprintf(fp, "*%s %f %f %d %d %d %d %d %d %d %d %d %s///%d %d\n",
            rl->roi[i].imgfile, rl->roi[i].zoom, rl->roi[i].recon_zoom,
            rl->roi[i].matnum, rl->roi[i].type, rl->roi[i].status,
            rl->roi[i].pos_x, rl->roi[i].pos_y, rl->roi[i].w, rl->roi[i].h,
            rl->roi[i].t, rl->roi[i].roi, rl->roi[i].roiname,
            0, rl->roi[i].point_nr ) < 10 ) {
              strcpy(roierrmsg, "cannot write data");
              fclose(fp); return(3);
          }
          /* Write second line, if trace ROI */
          if(rl->roi[i].type!=ROI_TRACE) continue;
          for(j=0; j<rl->roi[i].point_nr; j++)
            fprintf(fp, "%d %d ", rl->roi[i].x[j], rl->roi[i].y[j]);
          fprintf(fp, "\n");
        }

    }
  else if(ind>0){
         i=ind-1;
         /* Write first line */
         if(fprintf(fp, "*%s %f %f %d %d %d %d %d %d %d %d %d %s///%d %d\n",
           rl->roi[i].imgfile, rl->roi[i].zoom, rl->roi[i].recon_zoom,
           rl->roi[i].matnum, rl->roi[i].type, rl->roi[i].status,
           rl->roi[i].pos_x, rl->roi[i].pos_y, rl->roi[i].w, rl->roi[i].h,
           rl->roi[i].t, rl->roi[i].roi, rl->roi[i].roiname,
           0, rl->roi[i].point_nr ) < 10 ) {
             strcpy(roierrmsg, "cannot write data");
             fclose(fp); return(3);
         }
         /* Write second line, if trace ROI */
         //if(rl->roi[i].type!=ROI_TRACE) continue;
         for(j=0; j<rl->roi[i].point_nr; j++)
            fprintf(fp, "%d %d ", rl->roi[i].x[j], rl->roi[i].y[j]);
            fprintf(fp, "\n");
           
    }
  


    /* Close file */
    fclose(fp);
    return(0);
}
/*****************************************************************************/

