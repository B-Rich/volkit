/******************************************************************************

  Copyright (c) 2005-2013 Turku PET Centre

  File:        weight.c
  Description: Functions for setting weight factors based on SIF.

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
    Weights are scaled so that average is 1.0.
  2005-04-26 CL
    Merged libsif to libtpcimgio.
  2008-07-11 VO
    Added sifModerate().
  2013-04-04 VO
    Added sifExistentCounts().

******************************************************************************/

/*****************************************************************************/
#include "sif.h"
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Calculate weights for frames in SIF data, normalized between 0-1.
 *   Weights are calculated from formula
 *     weight=(frame duration)^2 / (trues in a frame).
 *   Before calling this routine, trues must be calculated as total counts -
 *   randoms.
 *   Counts in SIF are not corrected for physical decay. Therefore, isotope
 *   halflife must be known, if weights are to be calculated for decay
 *   corrected TACs. Isotope halflife must be set to 0, if weights are used
 *   for TACs that are not corrected for decay.
 *
 * @see Mazoyer BM, Huesman RH, Budinger TF, Knittel BL. Dynamic PET data analysis.
 * J Comput Assist Tomogr 1986; 10:645-653.
 * @param data sif data
 * @param halflife halflife (sec) of isotope label;
 * If halflife is 0, the weights are calculated for non-decay corrected data.
 * If halflife (sec) is >0, the weights are calculated using decay corrected
 * trues, but trues data is not changed.
 */
void sifWeight(SIF *data, double halflife) {
  int i;
  double f, d;

  if(SIF_TEST) printf("sifWeight(*sif, %g)\n", halflife);
  /* Calculate weights */
  for(i=0; i<data->frameNr; i++) {
    if(data->trues[i]<1.0) data->trues[i]=1.0;
    f=data->x2[i]-data->x1[i]; if(f<=0.0) f=1.0;
    if(halflife<=1.0E-8) 
      d=1.0;
    else
      d=exp( ((data->x1[i]+data->x2[i])/2.0)*0.693147/halflife );
    data->weights[i]=(f*f)/(d*data->trues[i]);
    /*printf("%3d %g %g\n", i, data->trues[i], data->weights[i]);*/
  }

#if(0)
  /* Scale weights between [0,1] */
  f=data->weights[0];
  for(i=1; i<data->frameNr; i++) if(data->weights[i]>f) f=data->weights[i];
  for(i=0; i<data->frameNr; i++) data->weights[i]/=f;
#else
  /* Scale weights so that average weight is 1.0 */
  for(i=0, f=0.0; i<data->frameNr; i++) f+=data->weights[i];
  f/=(double)data->frameNr;
  for(i=0; i<data->frameNr; i++) data->weights[i]/=f;
#endif

  return;
}
/*****************************************************************************/

/*****************************************************************************/
/** Moderate the trues in SIF. True values in SIF are used to calculate
 *  weight factors for time frames. If trues are very low in certain frames,
 *  the weight factors in other frames may become very low.
 *  This function finds the maximum trues, and adds max/limit to each trues
 *  value, if min trues < max trues / limit.
 *  Negative trues are always eliminated.
 */
void sifModerate(
  /** Pointer to SIF in which the trues are moderated */
  SIF *sif,
  /** Max trues / limit is added to all trues values; 100.0 might be good */ 
  double limit
) {
  int fi;
  double w, f;

  if(sif==NULL || sif->frameNr<2) return;
  if(limit<=1.0) return;
  for(w=f=sif->trues[0], fi=1; fi<sif->frameNr; fi++) {
    if(sif->trues[fi]>w) w=sif->trues[fi];
    else if(sif->trues[fi]<f) f=sif->trues[fi];
  }
  if(f*limit<w) {
    for(w/=limit, fi=0; fi<sif->frameNr; fi++)
      if(sif->trues[fi]>0.0) sif->trues[fi]+=w; else sif->trues[fi]=w;
  } else {
    for(fi=0; fi<sif->frameNr; fi++)
      if(sif->trues[fi]<0.0) sif->trues[fi]=0.0;
  }
}
/*****************************************************************************/

/*****************************************************************************/
/** Verify that SIF contains prompts and randoms.
\return Returns 0 if neither prompts or randoms can be found, 1 or 2 if prompts
    or randoms can be found, and 3 if both prompts and randoms are there.
 */
int sifExistentCounts(
  /** Pointer to SIF struct */
  SIF *sif
) {
  int fi, p=0, r=0;
  double v1, v2;
  if(sif==NULL || sif->frameNr<1) return 0;
  /* If just one frame, then value > 0 is fine */
  if(sif->frameNr==1) {
    if(sif->prompts[0]>0.00000001) p=1;
    if(sif->randoms[0]>0.00000001) r=2;
    return(p+r);
  }
  /* Otherwise, check also that frames have different count level */
  for(fi=1; fi<sif->frameNr; fi++) {
    v1=sif->prompts[fi]-sif->prompts[fi-1]; if(fabs(v1)>0.001) p=1;
    v2=sif->randoms[fi]-sif->randoms[fi-1]; if(fabs(v2)>0.001) r=2;
    if((p+r)>2) break;
  }
  return(p+r);
}
/*****************************************************************************/

/*****************************************************************************/

