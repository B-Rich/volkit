/*******************************************************************************

  Copyright (c) 2012 by Turku PET Centre

  File:        branch.c
  Description: Contains functions for branching ratio correction.

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
  2012-02-23 Vesa Oikonen
    First created, including function branchingFraction().


*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/******************************************************************************/
#include "branch.h"
/******************************************************************************/
#include "halflife.h"
/******************************************************************************/

/******************************************************************************/
/** Branching fraction for specified isotope.
 *
\return Returns the branching factor, or 0 in case branching fraction is unknown.
 */
float branchingFraction(
  /** Isotope code; see hlIsotopeFromHalflife() */
  int isotope
) {
  float bf=0.0;
  switch(isotope) {
    case TPCISOT_CU_64: bf=BRANCHING_Cu64; break;
    case TPCISOT_GA_68: bf=BRANCHING_Ga; break;
    case TPCISOT_GE_68: bf=BRANCHING_Ge; break;
    case TPCISOT_RB_82: bf=BRANCHING_Rb; break;
    case TPCISOT_F_18:  bf=BRANCHING_F; break;
    case TPCISOT_C_11:  bf=BRANCHING_C; break;
    case TPCISOT_N_13:  bf=BRANCHING_N; break;
    case TPCISOT_O_15:  bf=BRANCHING_O; break;
    case TPCISOT_BR_75:
    case TPCISOT_BR_76:
    case TPCISOT_CU_62:
    case TPCISOT_FE_52:
    case TPCISOT_NA_22:
    case TPCISOT_O_14:
    case TPCISOT_I_124:
    case TPCISOT_ZN_62:
    case TPCISOT_UNKNOWN:
    default:            bf=0.0;
  }
  return(bf);
}
/******************************************************************************/

/******************************************************************************/
