/******************************************************************************
  Copyright (c) 1993-2004 by Turku PET Centre

  intex.c
  
  Procedure for expansion of positive integers specified in a string.
  
  Version:
  1993-10-29 Vesa Oikonen
  1994-07-13 VO
    Edited to be compiled as a subprogram.
    Lowest int is changed from 1 to 0. Value 0 is not changed to 1.
  1996-09-18 VO
    ANSI C version.
  2002-02-18 VO
    Specific memory structure is created.
    Required memory is allocated here.
    The parameter of max int is removed, but set to 65536.
  2002-07-29 VO
    isdigit(*t) changed to isdigit((int)*t) to prevent compiler warnings.
  2003-08-17 Calle Laakkonen
    added intMerge()
  2004-09-17 VO
    Doxygen style comments.

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*****************************************************************************/
#include "intex.h"
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Existing list is freed and all data is cleared.
 * Expanded integers are listed in list.i[] in increasing order.
 *
 * @param text Integer expressions to be expanded, e.g. 0-8,12,34-28
 * @param list Pointer for int list data
 * @return 0 if ok and at least one integer is listed.
 */
int intExpand(char *text, INT_list *list) {
  int j;
  char *p, *t;
  int first, last, swap, intMax=65536;

  /* Check the arguments */
  if(strlen(text)<1) return(1);
  if(list->nr>0 && list->i!=NULL) free((char*)list->i); list->nr=0;

  /* Expand */
  p=strtok(text, " ,;.&\t\n\r\0");
  while(p!=NULL) {
    t=p; first=last=-1;
    while((*t!='-') && (!isdigit((int)*t)) && (*t)) t++;
    if(*t=='-') {
      while((!isdigit((int)*t)) && (*t)) t++;
      if(isdigit((int)*t)) {first=0; last=atoi(t);}
    } else if(isdigit((int)*t)) {
      first=atoi(t); /*if (first==0) first=1;*/
      while((isdigit((int)*t)) && (*t)) t++;
      if(*t == '-') {
        t++; while((!isdigit((int)*t)) && (*t)) t++;
        if(isdigit((int)*t)) last=atoi(t); else last=intMax;
      }
    }
    if((first>=0) && (last>=0)) {
      if(first>last) {swap=first; first=last; last=swap;}
      if(last>intMax) {if(first<=intMax) last=intMax; else last=0;}
      for(j=first; j<=last && list->nr<intMax; j++)
        if(_intexadd(list, j)<0) return(2);
    } else if(first>=0) {
      if(first<=intMax) if(_intexadd(list, first)<0) return(3);
    } else if(last>=0) {
      if(last>=intMax) if(_intexadd(list, last)<0) return(4);
    }
    p=strtok(NULL, " ,;.&\t\n\r\0");
  }
  if(list->nr<1) return(1);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * int _intexadd(int *list, int a) ; local function
 *
 * @param list
 * @param a
 */
int _intexadd(INT_list *list, int a) {
  int i, j, n;

  /* Check if list is yet empty */
  if(list->nr==0) {
    list->i=(int*)malloc(sizeof(int)); if(list->i==NULL) return(-1);
    /* Put the first integer to list and return */
    list->nr=1; list->i[0]=a; return(1);
  }
  n=list->nr;
  /* Check through the existing list */
  for(i=0; i<n; i++) {
    /* if it already is listed, just return */
    if(list->i[i]==a) return(0);
    /* make room for this integer */
    list->i=(int*)realloc(list->i, (n+1)*sizeof(int)); if(list==NULL) return(-1);
    if(list->i[i]>a) {for(j=n-1; j>=i; j--) list->i[j+1]=list->i[j]; break;}
  }
  list->i[i]=a; list->nr=n+1;
  return(1);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Merges two lists and returns the result. (the originals are not touched)
 * Duplicate entries are removed.
 * 
 * @param list1 The first list
 * @param list2 The second list
 * @return pointer to the new combined list.
 */
INT_list intMerge(INT_list *list1, INT_list *list2) {
  int r,count=0,l1=0,l2=0;
  INT_list newlist;
  int *tmplist;
  int found;
  tmplist=(int*)malloc(sizeof(int)*(list1->nr+list2->nr));
  while(l1<list1->nr || l2<list2->nr) {
    if(l1<list1->nr) {
      tmplist[count]=list1->i[l1];
      count++;
      l1++;
    }
    found=0;
    if(l2<list2->nr) {
      for(r=0;r<count;r++) {
        if(tmplist[r]==list2->i[l2]) found++;
      }
      if(found<1) {
        tmplist[count]=list2->i[l2]; count++;
      }
      l2++;
    }
  }
  newlist.i=(int*)malloc(sizeof(int)*count);
  memcpy(newlist.i,tmplist,count*sizeof(int));
  newlist.nr=count;
  free(tmplist);
  return newlist;
}
/*****************************************************************************/

