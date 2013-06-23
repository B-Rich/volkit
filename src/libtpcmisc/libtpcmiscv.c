/******************************************************************************

  libtpcmiscv.c  (c) 2004-2013 Turku PET Centre

  This file contains function for printing the date and time when this library
  is compiled.
  Function libtpcmisc_print_build(), is used for getting the build
  information.


  Version:
  2004-11-07 Vesa Oikonen
  First created.
  2005-01-20 Calle Laakkonen
  2005-04-26 1.1.0 Jarkko Johansson
  Functions libtpcmisc_print_readme(), libtpcmisc_print_history(),
  libtpcmisc_print_dreadme() and libtpcmisc_print_dhistory()
  are used for getting the Readme and History messages for the libtpcmisc,
  with and without Doxygen style tags. Remember to update the
  functions to get correct Readme and History messages!
  2006-03-26 1.1.1 VO
  Bug correction in iftfile.c.
  2006-06-03 1.1.2 VO
  Added decpoint.c and decpoint.h.
  2006-07-21 1.1.3 VO
  Change in iftfile.c, not affecting default behavior.
  2006-07-26 1.1.4 VO
  New functions in studynr.c, defined in studynr.h.
  2006-10-02 1.1.5 VO
  Bug correction version.
  Compiles independently without other TPC libraries.
  2006-10-12 1.1.6 VO
  Bug correction version.
  2006-10-23 1.1.7 VO
  Added files quots.c and quots.h.
  See iftread.c.
  2007-02-01 1.2.0 VO
  Halflife.c and .h and related functions are moved here from libtpcimgio 1.1.6
  because they are needed also in software not directly related to PET images.
  2007-03-30 1.2.1 Kaisa Liukko
  Added memory_handler.c which is used by lse and lsi (libtpcmodel).
  Added isotope I-124 into halflife.c and halflife.h.
  2007-07-17 1.3.0 Harri Merisaari
  Added substitutions for ANSI compatibility option 
  (not using configure scripts yet).
  2008-07-11 1.3.1 VO
  Small changes in header files etc, helping in compiling other sw.
  2008-07-14 1.3.2 VO
  Added backup.c and backup.h.
  2008-10-10 1.3.3 VO
  Additional function in decpoint.c and decpoint.h.

  In future, changes are listed below in history.

*/

/*****************************************************************************/

/** Library name. */
#define LIB_NAME "libtpcmisc"

/** Library version. */
#define LIB_VERSION "2.1.4"

/** Copyright. */
#define COPYRIGHT "(c) 2004-2013 by Turku PET Centre, University of Turku"

/*****************************************************************************/

#include <stdio.h>
#include "libtpcmisc.h"
#include "libtpcmiscv.h"

/*****************************************************************************/

/*!
 * Print the library readme to specified FILE pointer, use stdout for screen.
 *
 * @param fp target file pointer
 */
void libtpcmisc_print_readme(FILE *fp){

 fputs("\n"
  LIB_NAME" "LIB_VERSION" "COPYRIGHT"\n"
#ifdef __STRICT_ANSI__
  "(ANSI compatible build)\n"
#endif
  "\n"
  "Build "__DATE__" "__TIME__"\n"
  "\n"
  "\n"
  "PET Miscellaneous library\n"
  "-------------------------------------------------------------------------------\n"
  "\n"
  "Contents\n"
  "-------------------------------------------------------------------------------\n"
  "\n"
  "include/     - Includes all the header files for the .c files\n"
  "Makefile     - The makefile\n"
  "README       - This file.\n"
  "\n"
  "** .c files **\n"
  "\n",fp);
 fputs(
  "backup.c     -  File backup and copying.\n"
  "branch.c     -  Functions related to branching factor correction.\n"
  "datetime.c   -  Functions for processing date and time.\n"
  "decpoint.c   -  Reading floating point value from string with decimal comma.\n"
  "halflife.c   -  Procedures for isotopes and correction of physical decay.\n"
  "idcrypt.c    -  String encryption/decryption.\n"
  "intex.c      -  Expanding string expressions of integers to an array of ints.\n"
  "ift.c        -  Functions for basic processing of IFT data structure.\n"
  "iftfile.c    -  Functions for basic processing of IFT data structure.\n"
  "iftsrch.c    -  Search functions for IFT contents.\n"
  "libtpcmisc.c -  Program for printing the Readme, History and build information,\n"
  "                and for testing the library functions.\n"
  "libtpcmiscv.c-  Information for this library.\n"
  "memory_handler.c - Memory handling functions.\n"
  "petc99.c     -  ISO C99 functions that are not yet widely available in\n"
  "                common compilers.\n"
  "petunits.c   -  Functions for working with PET data units.\n"
  "rname.c      -  Functions for processing region names.\n"
  "quots.c      -  Functions for processing strings with quotation marks.\n"
  "studynr.c    -  Functions for processing PET study number.\n"
  "substitutions.c - Functions substituting others for ANSI compatibility.\n"
  "swap.c       -  Byte swapping between little and big endian platforms.\n"
  "\n"
  "Using the library\n"
  "-------------------------------------------------------------------------------\n"
  "\n"
  "The program that uses the library should include the .h file it needs.\n"
  "Example:\n"
  "    \\#include <libtpcmisc.h>\n"
  "\n"
  "    gcc myprog.c -ltpcmisc -lm\n"
  "\n"
  "\n", fp);
  fflush(fp);
}

/*!
 * Print the library History to specified FILE pointer,
 * use stdout for screen.
 *
 * @param fp target file pointer
 */
void libtpcmisc_print_history(FILE *fp) {
  fputs("\n"
  "2005-01-04 1.0.0 Vesa Oikonen, Calle Laakkonen, Riku Klén, Jarkko Johansson\n"
  "  Former libpet, the common PET C library, has been divided up in smaller\n"
  "  sublibraries that each handle a specific task.\n"
  "  This library includes miscellaneous functions utilised in PET data processing.\n"
  "2005-04-21 1.0.1 JJ\n"
  "  Changes in files libtpcmiscv.c and .h:\n"
  "  Functions libtpcmisc_print_readme(), libtpcmisc_print_history(),\n"
  "  libtpcmisc_print_dreadme() and libtpcmisc_print_dhistory()\n"
  "  are used for getting the Readme and History messages for the libtpcmisc,\n"
  "  with and without Doxygen style tags. Remember to update the\n"
  "  functions to get correct Readme and History messages!\n"
  "  Function libtpcmisc_print_build(), is used for getting the build\n"
  "  information.\n"
  "2005-04-26 1.1.0 JJ\n"
  "  Compiled with MinGW.\n"
  "  Merged with libift.\n"
  "2006-03-16 1.1.1 VO\n"
  "  Correction in iftread().\n"
  "2006-06-03 1.1.2 VO\n"
  "  Added decpoint.c and decpoint.h.\n"
  "2006-07-21 1.1.3 VO\n"
  "  Added global variables in iftfile.c.\n"
  ,fp);
  fputs("2006-07-26 1.1.4 VO\n"
  "  studynr.c and studynr.h:\n"
  "  Included functions studynr_match(), studynr_validity_check(),\n"
  "  studynr_rm_zeroes(), and studynr_to_lowercase().\n"
  "  Function studynr_from_fname() rewritten because of new definitions for\n"
  "  study numbers.\n"
  "2006-10-02 1.1.5 VO\n"
  "  Corrected a bug in studynr_validity_check().\n"
  "2006-10-12 1.1.6 VO\n"
  "  Corrected a bug in studynr_validity_check().\n"
  "2006-10-23 1.1.7 VO\n"
  "  iftRead() does not search for equation sign inside quotation marks.\n"
  ,fp);
  fputs("2007-02-02 1.2.0 VO\n"
  "  Decay correction and isotope code related functions are located in\n"
  "  this library instead of libtpcimgio.\n"
  "  New functions for identifying isotopes are added.\n"
  "2007-03-30 1.2.1 Kaisa Liukko\n"
  "  Added memory_handler.c which is used by lse and lsi (libtpcmodel).\n"
  "  Added isotope I-124 into halflife.c and halflife.h.\n"
  "2007-07-17 1.3.0 Harri Merisaari\n"
  "  Added substitutions for ANSI compatibility option\n"
  "  configure scripts yet).\n"
  "2008-07-11 1.3.1 VO\n"
  "  Small updates in header files to help in compiling other software.\n"
  "2008-07-14 1.3.2 VO\n"
  "  Added function backupExistingFile().\n"
  "2008-10-10 1.3.3 VO\n"
  "  Added function dec_nr().\n"
  "2009-06-09 1.3.4 VO\n"
  "  Added function atof_with_check().\n"
  ,fp);
  fputs("2010-04-12 1.4.0 VO\n"
  "  petunits.c and petunits.h moved here from libtpccurveio.\n"
  "2010-04-26 1.4.1 VO\n"
  "  Added functions studynr_from_fname2(), studynr_validity_check2(),\n"
  "  and studynr_in_fname() in studynr.c.\n"
  "2010-05-18 1.4.2 VO\n"
  "  Change in iftRead().\n"
  "  Added datetime.c and datetime.h.\n"
  "2010-05-19 1.4.3 VO\n"
  "  Added functions isdate2(), isdate3() and get_datetime().\n"
  "  Additional parameter in isdatetime().\n"
  "2010-05-19 1.4.4 VO\n"
  "  Change in iftRead().\n"
  "2010-07-01 1.4.5 VO\n"
  "  Added strcasestr() in substitutions.c and .h.\n"
  "  Added cunitFromFilename() in petunits.c and .h.\n"
  "2010-08-19 1.4.6 VO\n"
  "  Added function time_to_tm() and isdate4() in datetime.c and .h.\n"
  "2010-10-27 1.4.7 VO\n"
  "  Added function fileCopy() in backup.c and .h.\n"
  "2010-12-14 1.4.8 VO\n"
  "  Added function get_time() and bug fix in get_datetime().\n"
  "2011-07-01 1.4.9 VO\n"
  "  Additional units in petunits.c and .h.\n"
  "2011-09-13 2.0.0 VO\n"
  "  Changes to compile, install and distribute in 64-bit systems.\n"
  ,fp);
  fputs("2012-01-31 2.0.1 VO\n"
  "  branch.h moved from libtpcmodel to this libtpcmisc.\n"
  "  Added branching factor for Cu-64 in branch.h.\n"
  "  Corrected halflife for Cu-64.\n"
  "2012-02-16 2.0.2 VO\n"
  "  iftWriteItem() supports IFT type 4 (space separator).\n"
  "  New functions added in iftsrch.c and iftfile.c.\n"
  "2012-02-17 2.0.3 VO\n"
  "  Added function iftdup().\n"
  "  Other small changes in IFT processing and ift.h.\n"
  "2012-02-23 2.0.4 VO\n"
  "  Added branch.c with branchingFraction().\n"
  "2012-02-24 2.0.5 VO\n"
  "  Additional units in petunits.c and petunits.h.\n"
  "2012-03-05 2.0.6 VO\n"
  "  iftWriteItem() supports IFT types 5,6, and 7 (tab, comma, semicolon).\n"
  "  Additions and changes in decpoint.c and .h.\n"
  "2012-03-17 2.0.7 VO\n"
  "  Bug fix in studynr.c.\n"
  "2012-06-08 2.0.8 VO\n"
  "  Added function rnameRmDots() in studynr.c.\n"
  "2012-09-28 2.1.0 VO\n"
  "  Created rname.c and rname.h, into which region name related contents\n"
  "  were moved from studynr.c and studynr.h.\n"
  "  Added function rnameCatenate() in rname.c.\n"
  "  Added defined value for region name subfield lengths in rname.h.\n"
  "2013-04-16 2.1.3 VO\n"
  "  Changes in includes.\n"
  "2013-07-17 2.1.4 VO\n"
  "  Added function strnCopyClean() to quots.c.\n"
  "  Additional units in petunits.c and petunits.h.\n"
  "\n",fp);

  fflush(fp);
}

/*!
 * Print the library Readme to specified FILE pointer with Doxygen tags,
 * use stdout for screen.
 *
 * @param fp target file pointer
 */
void libtpcmisc_print_dreadme(FILE *fp) {
  fprintf(fp,"/**\n\\mainpage %s %s %s\n\\section readme Readme\n<PRE>\n",
    LIB_NAME,LIB_VERSION,COPYRIGHT);
  libtpcmisc_print_readme(fp);
  fprintf(fp,"\n</PRE> \n */\n");
}

/*!
 * Print the library History to specified FILE pointer with Doxygen tags,
 * use stdout for screen.
 *
 * @param fp target file pointer
 */
void libtpcmisc_print_dhistory(FILE *fp) {
  fprintf(fp,"/**\n\\page History\n\\section history History\n<PRE>\n");
  libtpcmisc_print_history(fp);
  fprintf(fp,"\n</PRE> \n */\n");
}

/*!
 * Print the build info to specified FILE pointer, use stdout for screen.
 *
 * @param fp target file pointer
 */
void libtpcmisc_print_build(FILE *fp) 
{
  fprintf(fp,"\n %s %s %s\n",LIB_NAME,LIB_VERSION,COPYRIGHT);
  fprintf(fp,"\n Build %s %s\n",__DATE__,__TIME__);
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
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Print the compilation date and time to specified FILE pointer
 *
 * @param fp target file pointer
 */
void libtpcmisc_printdate(FILE *fp) {
  libtpcmisc_print_build(fp);
}
/*****************************************************************************/

/*****************************************************************************/
