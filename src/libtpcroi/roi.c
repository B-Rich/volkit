/******************************************************************************

  Copyright (c) 1996-2005,2013 by Turku PET Centre

  File:          roi.c
  Description:   Procedures for reading, writing and processing 2D ROIs
                 of CTI ECAT format.

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
  1996-10-08 Vesa Oikonen
    First version.
  2000-11-24 VO
    nint() -> (int)rint().
  2001-10-03 VO
    Added some parenthesis.
  2002-02-18 VO
    i386 compatible version.
    Function names changed and procedures reorganized in functions.
    Possibility to apply ROIs to sinogram data was removed.
    Included functions for initiating and emptying ROI data.
    Removed the option for scaling ROIs to image with other matrix size.
  2002-11-22 Anne Seppänen
    New function to append rois to file.
  2003-01-23 AS
    Function roiAppend changed to write only one roi or all rois 
    from roi_list depending the 3rd parameter.
  2003-03-07 AS
    Function roiDel deletes ROI(s). 
    If ind=0 all ROIs are deleted, else del. ROI, which index is ind.
    Memory handling: _allocNr was added to ROI_list struct in roi.h file. 
  2003-07-02 CL
    Created the new API.
    ROIs are now stored in a linked list rather than a array.
    Rewrote RoiOnOff
    Rewrote Roi -> trace roi conversion functions
  2003-11-30 VO
    For now, calls temp_roundf() instead of roundf().
  2004-11-18 VO
    When reading trace ROIs with roiRead(), start and end points are connected
    if they are not so in the ROI file.
  2004-11-25 CL
    improved roi_onoff <-- kesken
    userdata is guaranteed to be NULL when ROI is loaded
    added roi module specific version information
    Merged following changes from 2003-10-21 version:
    Fixed ellipse and circle ROI bugs (radius was used as diameter)
    delete_roi and delete_roi_n now return a pointer to the next ROI in list.
  2004-12-22 VO
    Changed some comments etc, not affecting actual code.
    Note that improved roi_onoff() is applied only with "new" roi functions,
    old roi functions work as before.
  2005-01-20 CL
    Split the roi.c file to roi.c and oldroi.c
    Removed functions roi_mplane and roi_mframe
  2006-10-13 Kaisa Sederholm
    Recovered functions roi_mplane and roi_mframe
  2013-02-11 VO
    Modified to be able to compile this again.
    Support for Imadeus ROIs is removed.


******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
/*****************************************************************************/
#include "libtpcmisc.h"
#include "imgio.h"
/*****************************************************************************/
#include "libtpcroiv.h"
#include "roi.h"
/*****************************************************************************/
/** The ROI library error message */
char roierrmsg[128];
/*****************************************************************************/

#define SUPPORT_LEGACY


/*****************************************************************************/

/** Allocated memory is freed, all data is cleared. */
void roi_empty(ROI_list *rl)
{
  int r;
  RoiList *next;
#ifdef SUPPORT_LEGACY 
  if(rl->roi) {
    for(r=0; r<rl->nr; r++) {
      if(rl->roi[r].point_nr>0) {
        free(rl->roi[r].x);
        free(rl->roi[r].y);
        rl->roi[r].point_nr=0;
      }
    }
  }
  if(rl->_allocNr>0) free(rl->roi);
  rl->_allocNr=0;
#endif
  rl->nr=0;
  while(rl->rois) {
    next=rl->rois->next;
    free(rl->rois);
    rl->rois=next;
  }
}
/*****************************************************************************/

/*****************************************************************************/
/** Before any use, this should be called, but not later.
 */
void roi_init(ROI_list *rl)
{
  rl->roi=(ROI*)NULL;
  rl->nr=0;
#ifdef SUPPORT_LEGACY
  rl->_allocNr=0;
  rl->rois=NULL;
#endif
}

/*****************************************************************************/
/** Deletes the index:th ROI */
void roi_delete_n(ROI_list *rl, int ind) {
  RoiList *rois=rl->rois;
  int pos=0;
  while(pos<ind) {
    rois=rois->next;
    if(!rois) return;
    pos++;
  }
  if(rois->prev) rois->prev->next=rois->next; else rl->rois=rois->next;
  if(rois->next) rois->next->prev=rois->prev;
  if(rois->roi->x) free(rois->roi->x);
  if(rois->roi->y) free(rois->roi->y);
  free(rois->roi);
  free(rois);
  rl->nr--;
}

/*****************************************************************************/
/** Deletes ROI from roilist */
void roi_delete(ROI_list *rl, ROI *roi) {
  RoiList *rois=rl->rois;
  while(rois->roi!=roi) {
    rois=rois->next;
    if(!rois) return;
  }
  if(rois->prev) rois->prev->next=rois->next; else rl->rois=rois->next;
  if(rois->next) rois->next->prev=rois->prev;
  if(rois->roi->x) free(rois->roi->x);
  if(rois->roi->y) free(rois->roi->y);
  free(rois->roi);
  free(rois);
  rl->nr--;
}

/*****************************************************************************/
/** Adds ROI file contents (all ROIs) to specified data structure.
\return Returns 0, if OK.
 */
int roi_read(const char *fname, ROI_list *rl) {
  RoiList *newroientry;
  RoiList *roilist=rl->rois;
  ROI *newroi;
  int    i, j, n, ch;
  FILE  *fp;

  if(roilist) while(roilist->next) roilist=roilist->next;

  /* Open ROI file */
  if((fp=fopen(fname, "r")) == NULL) {
    strcpy(roierrmsg, "cannot open file");
    return 1;
  }

  n=0;
  while(1) {
    /* Allocate space for the new ROI and initialize it*/
    newroi=malloc(sizeof(ROI));
    memset(newroi,0,sizeof(ROI));
    newroi->userdata=NULL;

    /* Read first '*' */
    ch=fgetc(fp); if(ch==EOF || ch!='*') break;

    /* Read image filename */
    if(fscanf(fp, "%s ", newroi->imgfile)==EOF) {
      strcpy(roierrmsg, "cannot read image filename");
      fclose(fp);
      return 3;
    }

    /* Read zooms */
    if(fscanf(fp, "%f %f ", &newroi->zoom, &newroi->recon_zoom)==EOF) {
      strcpy(roierrmsg, "cannot read zoom values");
      fclose(fp);
      return 4;
    }
    if(newroi->recon_zoom==0.0) newroi->recon_zoom=1.0;

    /* Read matrix -> t */
    if(fscanf(fp, "%d %d %d %d %d %d %d %d ", &newroi->matnum, &newroi->type, &newroi->status,
         &newroi->pos_x, &newroi->pos_y, &newroi->w, &newroi->h, &newroi->t)==EOF) {
      strcpy(roierrmsg, "cannot read ROI definitions");
      fclose(fp);
      return 5;
    }

    /* Read ROI nr and name */
    if(fscanf(fp, "%d ", &newroi->roi)==EOF) {
      strcpy(roierrmsg, "cannot read ROI number");
      fclose(fp);
      return 6;
    }
    for(i=j=0; j<3; i++) {
      newroi->roiname[i]=(char)getc(fp);
      if(newroi->roiname[i]=='/') j++; else j=0;
    }
    j=i-3; newroi->roiname[j]='\0';
    /* delete spaces from ROI name */
    j=strlen(newroi->roiname);
    for(i=j-1; i>=0; i--)
      if(isspace((int)newroi->roiname[i])) newroi->roiname[i]=(char)0; else break;
    j=i; for(i=0; i<j; i++)
      if(!isspace((int)newroi->roiname[i])) {if(i>0) strcpy(newroi->roiname, newroi->roiname+i); break;}

    /* Read the number of points */
    fseek(fp,2,SEEK_CUR); /* skip 0 and space */
    if(fscanf(fp, "%d ", &newroi->point_nr)==EOF) {
      strcpy(roierrmsg, "cannot read nr of ROI border points");
      fclose(fp);
      return 8;
    }

    /* If TRACE ROI, read points */
    if(newroi->type==ROI_TRACE) {
      newroi->x=malloc(newroi->point_nr*sizeof(int));
      newroi->y=malloc(newroi->point_nr*sizeof(int));
      for(i=0; i<newroi->point_nr; i++)
        if(fscanf(fp, "%d %d", &newroi->x[i], &newroi->y[i]) == EOF) {
          strcpy(roierrmsg, "cannot read ROI border definition");
          free(newroi->x);
	  free(newroi->y);
	  free(newroi);
          fclose(fp);
	  return 9;
        }
      do {ch=getc(fp);} while(ch!=EOF && ch!='\n');
    }
    /* Append ROI to list */
    newroientry=malloc(sizeof(RoiList));
    newroientry->roi=newroi;
    newroientry->prev=roilist;
    newroientry->next=NULL;
    if(roilist) {
      roilist->next=newroientry;
      roilist=newroientry;
    } else {
      roilist=newroientry;
      rl->rois=roilist;
    }
    rl->nr++;
    n++;
  }
  fclose(fp);
  if(n==0) {
    strcpy(roierrmsg, "error in ROI file");
    return 2;
  }
  return 0;
}

/*****************************************************************************/
/*
 *  int roi_compute_rect(ROI *roi);
 */ 
int roi_compute_rect(ROI *roi)
{
  roi->point_nr=4;
  if((roi->x=malloc(roi->point_nr*sizeof(int)))==NULL)
    return(-1);
  if((roi->y=malloc(roi->point_nr*sizeof(int)))==NULL) {
    free(roi->x); return(-1);}
  roi->x[0]=0; roi->y[0]=0;
  roi->x[1]=roi->w; roi->y[1]=0;
  roi->x[2]=roi->w; roi->y[2]=roi->h;
  roi->x[3]=0; roi->y[3]=roi->h;
  return(0);
}
#ifdef SUPPORT_LEGACY
int roiComputeRect(ROI *roi)
{
  roi->point_nr=5;
  if((roi->x=malloc(roi->point_nr*sizeof(int)))==NULL)
    return(-1);
  if((roi->y=malloc(roi->point_nr*sizeof(int)))==NULL) {
    free((char*)roi->x); return(-1);}
  roi->x[0]=0; roi->y[0]=0;
  roi->x[1]=roi->w; roi->y[1]=0;
  roi->x[2]=roi->w; roi->y[2]=roi->h;
  roi->x[3]=0; roi->y[3]=roi->h;
  roi->x[4]=0; roi->y[4]=0;
  return(0);
}
#endif
/*****************************************************************************/

/*****************************************************************************/
/*
 *  int roi_compute_circle(ROI *roi);
 */
int roi_compute_circle(ROI *roi) {
  int points,r;
  r=roi->w/2;
  roi->pos_x+=r;
  roi->pos_y+=roi->w;
  roi->point_nr=36; /* TODO: Some clever algorithm to decide how many points we need */
  roi->x=malloc(sizeof(int)*roi->point_nr);
  roi->y=malloc(sizeof(int)*roi->point_nr);
  for(points=0;points<roi->point_nr;points++) {
    float rad=(M_PI/180.0)*(360.0*(points/(float)roi->point_nr));
    roi->x[points]=sin(rad)*r;
    roi->y[points]=cos(rad)*r-r;
  }
  return 0;
}
/*****************************************************************************/

/*****************************************************************************/
/*
 *  int roi_compute_ellipse(ROI *roi)
 */ 
int roi_compute_ellipse(ROI *roi) {
  int points;
  roi->pos_x+=roi->w/2;
  roi->pos_y+=roi->h;
  roi->point_nr=40; /* TODO: Some clever algorithm to decide how many points we need */
  roi->x=malloc(sizeof(int)*roi->point_nr);
  roi->y=malloc(sizeof(int)*roi->point_nr);
  for(points=0;points<roi->point_nr;points++) {
    float rad=(M_PI/180.0)*(360.0*(points/(float)roi->point_nr));
    roi->x[points]=sin(rad)*(roi->w/2);
    roi->y[points]=cos(rad)*(roi->h/2)-roi->h/2;
  }
  return 0;

}
/*****************************************************************************/

/*****************************************************************************/
/** Prints the contents of (individual) roi data structure. */
void roi_print(ROI *roi)
{
  int i;

  printf("imgfile: %s\n", roi->imgfile);
  printf("zoom: %f  recon_zoom: %f\n", roi->zoom, roi->recon_zoom);
#if 0
  printf("matnum: %d ; plane: %d frame: %d\n",
    roi->matnum, roi_mplane(roi->matnum), roi_mframe(roi->matnum) );
#endif
  printf("type: %d status: %d\n", roi->type, roi->status);
  printf("pos_x=%d pos_y=%d w=%d h=%d t=%d\n",
    roi->pos_x, roi->pos_y, roi->w, roi->h, roi->t);
  printf("roi=%d _%s_\n", roi->roi, roi->roiname);
  printf("points=%d\n", roi->point_nr);
  if(roi->point_nr>0) {
    for(i=0; i<roi->point_nr; i++) printf("(%d,%d) ", roi->x[i], roi->y[i]);
    printf("\n");
  }
}
/*****************************************************************************/

/*** Fill the matrix with a Trace ROI ***/
/* A structure for polygon edges. Used in roi_onoff() */
typedef struct {
    /** Minimium and maximium Y coordinates */
    float minY,maxY;
    /** X coordinate of minY */
    float x;
    /** slope */
    float m;
} PolygonEdge;

static void fill_traceroi(char **m,int dimx,int dimy,float f,int pos_x,int pos_y,int *roix,int *roiy,int point_nr) {
    PolygonEdge *edges;
    int edge,p,y;
    int *px;

    edges=malloc(sizeof(PolygonEdge)*point_nr);
    edge=0;
    for(p=0;p<point_nr;p++) {
        float maxX;
        int n;
        if(p==point_nr-1) n=0; else n=p+1;
        if(roiy[p]>roiy[n]) {
            edges[edge].minY=roiy[n]*f+pos_y;
            edges[edge].x=roix[n]*f+pos_x;
            edges[edge].maxY=roiy[p]*f+pos_y;
            maxX=roix[p]*f+pos_x;
        } else if(roiy[p]<roiy[n]) {
            edges[edge].minY=roiy[p]*f+pos_y;
            edges[edge].x=roix[p]*f+pos_x;
            edges[edge].maxY=roiy[n]*f+pos_y;
            maxX=roix[n]*f+pos_x;
        } else continue;
        edges[edge].m=(maxX-edges[edge].x)/(edges[edge].maxY-edges[edge].minY);
        edge++;
    }
    px=malloc(sizeof(int)*point_nr);
    for(y=0;y<dimy;y++) {
        int pc,x,odd;
        pc=0;
        for(p=0;p<edge;p++) {
            if(temp_roundf(edges[p].minY)<=y && y<temp_roundf(edges[p].maxY)){
                px[pc]=temp_roundf(edges[p].x+edges[p].m*(y-temp_roundf(edges[p].minY)));
                if(px[pc]<0) px[pc]=0;
                pc++;
            }
        }
        odd=0;
        for(x=0;x<dimx;x++) {
            m[x][y]=odd;
            for(p=0;p<pc;p++) {
                if(x==px[p]) {
                    if(pc>1) odd=!odd;
                    m[x][y]=1;
                }
            }
        }
    }
    free(px);
    free(edges);
}

/** The matrix is filled with 0's (outside of ROI) and 1's (inside
 *  the ROI border).
 *  If ROI extends outside image borders, those points are ignored.
 *  Matrix coordinates are up-to-bottom and left-to-right.
\return Function returns 0, if everything is OK.
 */

int roi_onoff(
  /** Definitions for one region-of-interest */
  ROI *roi,
  /** Image matrix x dimension */
  int dimx,
  /** Image matrix y dimension */
  int dimy,
  /** Image zoom */
  float zoom,
  /** Allocated memory for output matrix[dimx][dimy] */
  char **m
) {
  int x,y,r;
  float f,pos_x,pos_y;

  /* Check the parameters */
  if(roi==NULL || dimx<2 || dimy<2 || zoom<1.0e-6 || m==NULL) {
    strcpy(roierrmsg, "invalid arguments for roiOnOff()");
    return 1;
  }
  /* Prepare */
  f=zoom/roi->zoom;
  pos_x=roi->pos_x*f;
  pos_y=roi->pos_y*f;
  for(y=0;y<dimy;y++)
    memset(m[y],0,dimx);

  /* Fill the matrix */
  switch(roi->type) {
    case ROI_TRACE:		    /* Trace ROI */
        fill_traceroi(m,dimx,dimy,f,pos_x,pos_y,roi->x,roi->y,roi->point_nr);
      break;
    case ROI_RECTANGULAR:	/* Rectangle ROI */
      for(x=pos_x;x<pos_x+roi->w*f;x++)
        for(y=pos_y;y<pos_y+roi->h*f;y++)
          m[x][y]=1;
      break;
    case ROI_CIRCULAR:{	    /* Circle ROI */
      int x1,x2;
      int y1,y2;
      r=roi->w*f;
      for(y=0;y<r;y++) {
        y1=pos_y-y;
	    y2=pos_y+y;
        x1=temp_roundf(sqrt(r*r-y*y));
        x2=pos_x+x1;
        x1=pos_x-x1;
        if(x1<0) x1=0;
        if(x2>dimx) x2=dimx;
        while(x1<x2) {
          if(y1>=0 && y1<dimy) m[x1][y1]=1;
          if(y2>=0 && y2<dimy) m[x1][y2]=1;
          x1++;
        }
      }
    } break;
    case ROI_ELLIPSE: {		/* Ellipse ROI */
      int x1,x2,a;
      int y1,y2,b;
      a=roi->w*f;
      b=roi->h*f;
      for(y=0;y<b;y++) {
        y1=pos_y-y;
        y2=pos_y+y;
        x1=sqrt(1-(y*y)/(float)(b*b))*a;
        x2=pos_x+x1;
        x1=pos_x-x1;
        if(x1<0) x1=0;
        if(x2>dimx) x2=dimx;
        while(x1<x2) {
          if(y1>=0 && y1<dimy) m[x1][y1]=1;
          if(y2>=0 && y2<dimy) m[x1][y2]=1;
          x1++;
        }
      }
    } break;
    default:
      strcpy(roierrmsg,"invalid roi type");
      return 2;
  }
  return 0;
}
/*****************************************************************************/
/*
 *  int roi_save(char *fname, ROI_list *roilist)
 */
int roi_save(const char *fname, ROI_list *rl) {
  RoiList *rois;
  FILE *fp;

  /* Check arguments */
  if(!fname[0] || rl==NULL || rl->nr<1) {
    strcpy(roierrmsg, "invalid arguments for roiSave()");
    return 1;
  }

  /* Create file */
  if((fp=fopen(fname, "w"))==NULL) {
    strcpy(roierrmsg, "cannot open file");
    return 2;
  }
  /* Loop and save the ROIs */
  rois=rl->rois;
  while(rois) {
    if(roi_append(fp,rois->roi)) return 3;
    rois=rois->next;
  }
  return 0;
}


/*****************************************************************************/
/*
 *  int roi_append(FILE *fp, ROI *roi)
 */
int roi_append(FILE *fp,ROI *roi) {
  /* Write first line */
  if(fprintf(fp, "*%s %f %f %d %d %d %d %d %d %d %d %d %s///%d %d\n",
    roi->imgfile, roi->zoom, roi->recon_zoom,
    roi->matnum, roi->type, roi->status,
    roi->pos_x, roi->pos_y, roi->w, roi->h,
    roi->t, roi->roi, roi->roiname,
    0, roi->point_nr ) < 10 ) {
      strcpy(roierrmsg, "cannot write data");
      fclose(fp); return 1;
   }
   /* Write second line, if trace ROI */
   if(roi->type==ROI_TRACE) {
     int j;
     for(j=0; j<roi->point_nr; j++)
       fprintf(fp, "%d %d ", roi->x[j], roi->y[j]);
     fprintf(fp, "\n");
   }
   return 0;
}

/*****************************************************************************/
/*
 *  int roi_append_n(FILE *fp, ROI_list *rl,int ind)
 */
int roi_append_n(FILE *fp,ROI_list *rl,int ind) {
  RoiList *rois=rl->rois;
  int pos=0;
  while(pos<ind) {
    rois=rois->next;
    pos++;
  }
  return roi_append(fp,rois->roi);
}


/*****************************************************************************/
int jsqrt(int n)
{
  int x, x2, q, r, t;

  if(n<2) return n;
  t=x=n; while(t>>=2) x>>=1; x++;
  while(1) {
    q=n/x; r=n%x; 
    if(x<=q) {x2=x+2; if(q<x2 || (q==x2 && r==0)) break;}
    x=(x+q)>>1;
  }
  return x;
}
/*****************************************************************************/

/*****************************************************************************/
/** Returns the plane number of specified matrix */
int roi_mplane(int matnum)
{
  Matval matval;

  mat_numdoc(matnum, &matval);
  return matval.plane;
}
/*****************************************************************************/

/*****************************************************************************/
/** Returns the frame number of specified matrix */
int roi_mframe(int matnum)
{
  Matval matval;

  mat_numdoc(matnum, &matval);
  return matval.frame;
}
/*****************************************************************************/

/*****************************************************************************/
