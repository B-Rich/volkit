/******************************************************************************

  Copyright (c) 2004-2013 by Turku PET Centre

  File:         libtpcroiv.c
  Purpose:      functions for printing the information on the library. 
  Version:      See the LIB_VERSION and history below.
  
******************************************************************************/

/** Library name. */
#define LIB_NAME "libtpcroi"

/** Library version. */
#define LIB_VERSION "1.2.2"

/** Copyright. */
#define COPYRIGHT "(c) 1996-2013 by Turku PET Centre, University of Turku"

/*****************************************************************************/
#include <stdio.h>
#include "libtpcroiv.h"
/*****************************************************************************/

/*****************************************************************************/
/**
 * Print the library History to specified FILE pointer,
 * use stdout for screen.
 * 
 * @param fp file pointer
 */
void libtpcroi_print_history(FILE *fp)
{
  fputs(
  "\n"
  "2004-11-07 Vesa Oikonen\n"
  "  First created, based on roi.c and roi.h.\n"
  "2005-04-26 Jarkko Johansson\n"
  "  Functions libtpcroi_print_readme(), libtpcroi_print_history(),\n"
  "  libtpcroi_print_dreadme() and libtpcroi_print_dhistory()\n"
  "  are used for getting the Readme and History messages for the libtpcroi,\n"
  "  with and without Doxygen style tags. Remember to update the\n"
  "  functions to get correct Readme and History messages!\n"
  "  Function libtpcroi_print_build(), is used for getting the build\n"
  "  information.\n"
  "2005-05-13 Harri Merisaari\n"
  "  Functions for processing Imadeus ROIs added (file roi_Imadeus_IO.[hc]).\n"
  "2005-06-03 Calle Laakkonen\n"
  "  Rewrote roi_read(). fscanf didn't work properly on Windows because of\n"
  "  different coding for newlines.\n"
  "2005-06-06 Calle Laakkonen\n"
  "  Image filename can now have spaces escaped with backslashes or quotes.\n"
  "  Fixed reading of files in windows format broken in previous version.\n"
  "2006-10-13 1.2.1 Kaisa Sederholm \n"
  "  Functions roi_mplane and roi_mframe were recovered.\n"
  "2013-02-11 1.2.2 VO\n"
  "  Updated to get this compiled and built.\n"
  "  Recovered old function APIs for compatibility with old SW.\n"
  "  Removed support for Imadeus ROIs.\n"
  "  Fixed Doxygen comments.\n"
  "\n",fp);
  fflush(fp);  
}

void libtpcroi_print_readme(FILE *fp){
  fputs(
  "\n"
  LIB_NAME" "LIB_VERSION" "COPYRIGHT"\n"
  "\n"
  "Build "__DATE__" "__TIME__"\n"
  "\n"
  "\n"
  "PET ROI library\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "Library is distributed under the GNU Lesser General Public Licence\n"
  "http://www.turkupetcentre.net/software/lgpl-license.html\n"
  "More information on open source licences: http://www.opensource.org\n"
  "\n"
  "Description\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "The library contains methods for using PET image ROI definitions.\n"
  "\n",fp);
  fputs("List of files\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "Doxyfile         - Doxygen configuration file.\n"
  "include/         - Includes all the header files for the .c files\n"
  "Makefile         - The makefile\n"
  "** .c files **\n"
  "roi.c            - IO functions for CTI ECAT 6.3 ROI files.\n"
  "oldroi.c         - Old API for IO functions for CTI ECAT 6.3 ROI files.\n"
  "libtpcroiv.c     - Functions for printing the library information.\n"
  "libtpcroi.c      - Program for printing the library information,\n"
  "                   and for testing the library functions.\n"
  "\n"
  "Using the library\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "The program that uses the library should include the .h file it needs.\n"
  "Example:\n"
  "    \\#include <roi.h>\n"
  "\n"
  "libtpcroi depends on libtpcmisc and libtpcimgio.\n"
  "Example:\n"
  "\n"
  "	gcc myprog.c -ltpcroi -ltpcimgio -ltpcmisc\n"
  "\n",fp);
  fflush(fp);
}

/** 
    Print the library Readme to specified FILE pointer with Doxygen tags, 
    use stdout for screen.
*/
void libtpcroi_print_dreadme(FILE *fp)
{
  fprintf(fp,"/**\n\\mainpage %s %s %s\n\\section readme Readme\n<PRE>\n",
          LIB_NAME,LIB_VERSION,COPYRIGHT);
  libtpcroi_print_readme(fp);
  fprintf(fp,"\n</PRE> \n */\n");
}

/** 
    Print the library History to specified FILE pointer with Doxygen tags, 
    use stdout for screen.
*/
void libtpcroi_print_dhistory(FILE *fp)
{
  fprintf(fp,"/**\n\\page History\n\\section history History\n<PRE>\n");
  libtpcroi_print_history(fp);
  fprintf(fp,"\n</PRE> \n */\n");
}

/** 
    Print the build info to specified FILE pointer, use stdout for screen.
*/
void libtpcroi_print_build(FILE *fp)
{
  fprintf(fp,"\n %s %s %s\n", LIB_NAME, LIB_VERSION, COPYRIGHT);
  fprintf(fp,"\n Build %s %s\n", __DATE__, __TIME__);
#ifdef __STRICT_ANSI__
  fprintf(fp," (ANSI compatible build)\n");
#endif
#if defined(__FILE__) && defined(__TIMESTAMP__)
  printf(" File %s last edited on %s\n", __FILE__, __TIMESTAMP__);
#endif
#if defined(__STDC_VERSION__)
  printf(" Version of C: %ld\n", __STDC_VERSION__);
#endif
#if defined(__STDC_ISO_10646__)
  printf(" Compiler ISO/IEC 10646 specification: %ld\n", __STDC_ISO_10646__);
#endif
#if defined(__GNUC__) && defined(__VERSION__)
  printf(" GNU C version: %s\n", __VERSION__);
#endif
#if defined(__x86_64__) || defined(__LP64__) || defined(__ppc64__)
  printf(" Architecture: 64-bit\n");
#else
  printf(" Architecture: 32-bit\n");
#endif
  printf(" Size of long double: %d bits\n", 8*(int)sizeof(long double));
#if defined(FLT_EVAL_METHOD)
  printf(" FLT_EVAL_METHOD: %d\n", FLT_EVAL_METHOD);
#endif
}
/*****************************************************************************/

/*****************************************************************************/
