/******************************************************************************

  Copyright (c) 2002-2007,2012 by Turku PET Centre

  halflife.h
  
  References:

  Table of Isotopes, Sixth edition, edited by C.M. Lederer,
  J.M. Hollander, I. Perlman. WILEY, 1967.

  Monographie BIPM-5, Table of Radionuclides, Vol. 6, 2011,
  http://www.bipm.org/en/publications/monographies-ri.html

  Date:
  2002-01-30 Vesa Oikonen
    First created.
  2002-08-08 VO
    Ga68 changed from 67.8 to 68.0 as requested by Tuula Tolvanen.
  2002-08-28 VO
    Included unofficial isotopes Br76, Rb82 and Cu62.
  2003-07-07 VO
    Included function definitions for halflife.c.
  2004-02-02 VO
    Included unofficial half-lives for Br-75, Cu-64, Fe-52, O-14, Na-22 and
    Zn-62.
  2005-01-21 VO
    Included hlCorrectIsotopeCode().
  2007-02-01 VO
    See halflife.c.
  2007-03-30 Kaisa Liukko
    Added isotope I-124
  2012-01-31 VO
    Half-life of Cu-64 is corrected to follow BIPM-5 reference.
  2012-02-23 VO
    Isotope code enums moved in here from halflife.c.
    

******************************************************************************/
#ifndef _HALFLIFE_H
#define _HALFLIFE_H
/*****************************************************************************/
/** Isotope halflife in minutes */
#define HL_O15 2.05 /* 123 s */
#define HL_N13 10.0
#define HL_C11 20.4
#define HL_F18 109.8
#define HL_Ge68 396000.0 /* 275 d */
#define HL_Ga68 68.0
/*****************************************************************************/
/* The following halflifes have not been checked from the reference;         */
/* they are thus meant to be used only during program development period     */
#define HL_Br75 98.0
#define HL_Br76 978.33 /* 58700 s */
#define HL_Cu62 9.7    /* 582 s */
#define HL_Cu64 762.018 /* 12.7003 h */
#define HL_Fe52 4980.0
#define HL_Na22 1368000.0
#define HL_O14 1.1818
#define HL_Rb82 1.25   /* 75 s */
#define HL_Zn62 558.0
#define HL_I124 6013.44 /* 4.176 */
/*****************************************************************************/
/** isotope_code */
enum {
  TPCISOT_BR_75, TPCISOT_BR_76, TPCISOT_CU_62, TPCISOT_CU_64, 
  TPCISOT_FE_52, TPCISOT_GA_68, TPCISOT_GE_68, TPCISOT_NA_22,
  TPCISOT_RB_82, TPCISOT_ZN_62, TPCISOT_F_18, TPCISOT_C_11,
  TPCISOT_N_13, TPCISOT_O_15, TPCISOT_O_14, TPCISOT_I_124,
  TPCISOT_UNKNOWN
};
/*****************************************************************************/
/* Add ln(2) if it is not defined */
#ifndef M_LN2
#define M_LN2       0.69314718055994530942
#endif
/*****************************************************************************/
extern char *hlIsotopeCode(int isotope);
extern double hlFromIsotope(char *isocode);
extern double hl2lambda(double halflife);
extern double hlLambda2factor(double lambda, double frametime, double framedur);
extern float hlLambda2factor_float(float lambda, float frametime, float framedur);
extern char *hlCorrectIsotopeCode(char *isocode);
extern int hlIsotopeFromHalflife(double halflife);
/*****************************************************************************/
/* Deprecated function names. Please don't use these anymore */
#define lambda2factor hlLambda2factor
#define lambda2factor_float hlLambda2factor_float
/*****************************************************************************/
#endif
