/******************************************************************************

  Copyright (c) 2005-2013 by Turku PET Centre

  File:        libtpcimgiov.c
  Description: This file contains function for printing the date and time when
               this library is compiled.
  
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
  
  Modification history: see below.

******************************************************************************/
#include <stdio.h>
#include "libtpcimgiov.h"
/*****************************************************************************/
/** Library name. */
#define LIB_NAME "libtpcimgio"

/** Library version. */
#define LIB_VERSION "2.1.6"

/** Copyright. */
#define COPYRIGHT "(c) 2005-2013 by Turku PET Centre, University of Turku"

/*****************************************************************************/
/*!
 * Print the library readme to specified FILE pointer, use stdout for screen.
 *
 * @param fp file pointer
 */
void libtpcimgio_print_readme(FILE *fp){
  fputs("\n"
  LIB_NAME" "LIB_VERSION" "COPYRIGHT"\n"
  "\n"
  "Build "__DATE__" "__TIME__"\n"
  "\n"
  "\n"
  "PET Image IO library\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "Turku PET Centre\n"
  "\n"
  "Libtpcimgio is distributed under the GNU Lesser General Public Licence\n"
  "http://www.gnu.org/licenses/lgpl.html\n"
  "More information on open source licences: http://www.opensource.org/\n"
  "\n"
  "Description\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "The libtpcimgio library is a collection of commonly used C files in\n"
  "Turku PET Centre for image file input and output procedures.\n"
  "Libtpcimgio library supports Ecat 6.x, Ecat 7.x and partly Analyze 7.5,\n"
  "NIfTI-1, microPET, and Interfile formats.\n"
  "\n",fp);
  fputs("List of files\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "include/      - Includes all the header files for the .c files\n"
  "Makefile      - The makefile\n"
  "** .c files **\n"
  "analyze.c     - Reading and writing Analyze 7.5 image files.\n"
  "ecat63ml.c    - Procedures for ECAT 6.3 matrix list.\n"
  "ecat63p.c     - Printing ECAT 6.3 headers.\n"
  "ecat63r.c     - Reading ECAT 6.3 files.\n"
  "ecat63w.c     - Writing ECAT 6.3 files.\n"
  "ecat7ml.c     - Procedures for ECAT 7.x matrix list.\n"
  "ecat7p.c      - Printing ECAT 7.x headers.\n"
  "ecat7h.c      - Edit ECAT 7.x headers.\n"
  "ecat7r.c      - Reading ECAT 7.x files.\n"
  "ecat7w.c      - Writing ECAT 7.x files.\n"
  "img.c         - Procedures for 4D image data, common to all image formats.\n"
  "img_ana.c     - Procedures for reading/writing Analyze 7.5 to/from IMG.\n"
  "img_e63.c     - Procedures for reading/writing ECAT 6.3 to/from IMG.\n"
  "img_e7.c      - Procedures for reading/writing ECAT 7 to/from IMG.\n"
  "imgmatch.c    - Procedures for comparing IMG contents.\n"
  "img_nii.c     - Procedures for reading/writing NIfTI-1 to/from IMG.\n"
  "img_sif.c     - Copying information between IMG and SIF structs.\n"
  "img_upet.c    - Procedures for reading/writing microPET to/from IMG.\n"
  "imgdecay.c    - Decay correction, isotope codes, and half-lives.\n"
  "imgfile.c     - Main procedures for IMG read/write.\n"
  "imgmax.c      - Procedures for searching IMG min and max value.\n"
  "imgunit.c     - Procedures for working with IMG calibration unit.\n"
  "interfile.c   - Reading Interfile headers.\n"
  "micropet.c    - Reading Inveon microPET image files.\n"
  "nifti.c       - Reading and writing NIfTI-1 PET images.\n"
  "sif.c         - Routines for Scan Information Files (SIF).\n"
  "sifio.c       - Functions for reading and writing SIF format files.\n"
  "vol.c         - Procedures for 3D image volume data (no time information).\n"
  "weight.c      - Functions for setting weight factors based on SIF.\n"
  "libtpcimgio.c - Program for printing Readme and History information, and\n"
  "                for testing the library functions.\n"
  "libtpcimgiov.c - Version information for this library.\n"
  "\n"
  "Using the library\n"
  "--------------------------------------------------------------------------\n"
  "\n"
  "The program that uses the library should include the .h file it needs.\n"
  "Example to include them all:\n"
  "    \\#include <imgio.h>\n"
  "\n"
  "libtpcimgio depends on libtpcmisc.\n"
  "Example:\n"
  "  gcc myprog.c -ltpcimgio -ltpcmisc\n"
  "\n",fp);
  fflush(fp);
}

/*!
 * Print the library History to specified FILE pointer,
 * use stdout for screen.
 * 
 * @param fp file pointer
 */
void libtpcimgio_print_history(FILE *fp) {
  fputs("2005-01-04 1.0.0 V Oikonen, C Laakkonen, R Klén, J Johansson\n"
  "  Former libpet, the common PET C library, has been divided up in smaller\n"
  "  sublibraries that each handle a specific task, including this library,\n"
  "  libtpcimgio.\n"
  "2005-03-03 1.0.0 Jarkko Johansson \n"
  "  imgproc.[c/h] moved to libtpcimgp library\n"
  "2005-04-05 1.0.1 (krs) Roman Krais\n"
  "  interfile.c added (for reading interfile headers)\n"
  "2005-04-25 1.1.0 Jarkko Johansson\n"
  "  History and Readme files included in new print functions:\n"
  "  libtpcimgio_print_readme(FILE *fp);\n"
  "  libtpcimgio_print_history(FILE *fp);\n"
  "  libtpcimgio_print_dreadme(FILE *fp);\n"
  "  libtpcimgio_print_dhistory(FILE *fp);\n"
  "  libtpcimgio_print_build(FILE *fp);\n"
  "  in file libtpcimgio.c. Update the the Readme and History\n"
  "  there.\n"
  "  Compiled in Windows with MinGW.\n"
  "2005-05-04 1.1.0 JJ\n"
  "  Minor changes in file sifio.c.\n"
  "2005-08-01 1.1.1 CL/JJ\n"
  "  Changed image loading/writing functions fname to const,\n"
  "  and extern int VOL_TEST to int VOL_TEST.\n"
  "2005-10-10 1.1.2 CL\n"
  "  imgWriteAnalyze() now writes only 1 frame at a time,\n"
  "  conserving memory.\n"
  "2005-11-21 1.1.3 VO\n"
  "  lambda2factor() now calculates correctly when removing\n"
  "  decay correction considering frame lengths.\n"
  "2005-12-12 1.1.4 VO\n"
  "  imgReadAnalyze() sets img.isotopeHalflife, if isotope is found in SIF.\n"
  ,fp);
  fputs("2006-10-31 1.1.5 VO\n"
  "  Added function imgUnitId().\n"
  "  Fixed a timezone bug with ECAT7 files.\n"
  "2006-12-28 1.1.6 VO\n"
  "  Changes in identification of isotope codes.\n"
  "2007-02-02 1.2.0 VO\n"
  "  Additions in IMG structure, and subsequent changes in i/o functions.\n"
  "  Isotope and halflife related functions are relocated into libtpcmisc.\n"
  "  This library is now independent of libtpcimgp.\n"
  "  IMG min and max search functions are relocated here.\n"
  "  User_process_code is read/written with ECAT7 and ECAT6.3 files.\n"
  "  Patient_id is read/written with ECAT7 and ECAT6.3 files.\n"
  "  Study_description is read/written with ECAT7 and ECAT6.3 files.\n"
  "  X, y and z resolution is read/written with ECAT7 images.\n"
  "  Prompts and randoms (delayed) are read/written with ECAT7 and\n"
  "  ECAT6.3 sinograms and read from SIF with Analyze 7.5.\n"
  "  Changes in identifying and processing calibration units.\n"
  "2007-02-11 1.2.1 VO\n"
  "  Addition in IMG structure.\n"
  "  Deleted imgmsg.h. Set IMG.statmsg using imgSetStatus().\n"
  "2007-02-27 1.2.2 VO\n"
  "  Bug corrections.\n"
  "2007-03-13 1.2.3 VO\n"
  "  Added ECAT matrix list functions and error messages.\n"
  "  ECAT 6.3 header print format changed.\n"
  ,fp);
  fputs("2007-03-21 1.2.4 VO\n"
  "  Changes to make easier to write ECAT7 2D files.\n"
  "2007-03-25 1.2.5 VO\n"
  "  Addition of functions for reading and writing ECAT and Analyze\n"
  "  format one frame at a time.\n"
  "2007-04-03 1.3.0 VO\n"
  "  Added support for ECAT7 polar maps.\n"
  "  Information for polar map added to IMG struct.\n"
  "2007-07-17 1.3.1 Harri Merisaari\n"
  "  Modified for optional ANSI compatibility.\n"
  "2007-09-10 1.3.2 VO\n"
  "  Bug correction in setting error messages.\n"
  "  Bug correction in reading some too long header text fields.\n"
  "2007-09-10 1.3.3 VO\n"
  "  Return value of localtime() is always checked.\n"
  ,fp);
  fputs("2007-09-11 1.3.4 VO\n"
  "  Corrected a bug affecting versions 1.3.1 -1.3.3:\n"
  "  Analyze and ECAT files could have been mixed if in same folder.\n"
  "2007-09-12 1.3.5 VO\n"
  "  Corrected a bug affecting frame-by-frame reading of\n"
  "  ECAT7 2D image files.\n"
  "2008-07-07 1.3.6 VO\n"
  "  Image decay correction functions return error if frame times\n"
  "  are missing.\n"
  "  Assumes that Analyze 7.5 images are corrected for decay when\n"
  "  that information is not found in header file.\n"
  "2008-07-11 1.3.7 VO\n"
  "  Added function sifModerate().\n"
  "2008-07-14 1.3.8 VO\n"
  "  Added function imgAllocateWithHeader().\n"
  "2008-07-24 1.3.9 VO\n"
  "  Added functions ecat7EditMHeader(), ecat7EditSHeader(),\n"
  "  ecat7EditVHeader(), imgSetUnit(), and ecat7PrintSubheader().\n"
  "2008-09-02 1.4.0 VO\n"
  "  Default ECAT backup extension changed to .bak.\n"
  "2008-10-03 1.4.1 VO\n"
  "  Accepts certain Analyze formats with 32 bit pixel values.\n"
  "2008-10-09 1.4.2 VO\n"
  "  Accepts more Analyze formats with 32 bit pixel values (PVElab).\n"
  ,fp);
  fputs("2008-11-06 1.4.3 VO\n"
  "  Bug correction in reading ECAT 6.3 files.\n"
  "2009-02-17 1.5.0 VO\n"
  "  Added definitions for Siemens Inveon in img.h.\n"
  "  Added micropet.c and .h for reading Siemens Inveon images.\n"
  "2009-02-25 1.5.1 VO\n"
  "  Changes in micropet.c and .h to allow reading CT images.\n"
  "2009-04-06 1.5.2 VO\n"
  "  Changes in sif.c to allow SIF without prompts and randoms.\n"
  "2009-09-28 1.5.3 VO\n"
  "  Change in micropet.c reading pixel z size.\n"
  "2009-12-01 1.5.4 VO\n"
  "  Added function imgSmoothMax().\n"
  "2009-12-10 1.5.5 VO\n"
  "  Tiny update in img_ana.c.\n"
  "2010-02-12 1.5.6 VO\n"
  "  Time zone is corrected in imgSetEcat7MHeader().\n"
  "2010-05-11 1.5.7 VO\n"
  "  imgUnitFromEcat7() tries more to identify units.\n"
  "2010-08-19 1.5.8 VO\n"
  "  ECAT7 main header field patient_birth_date is read correctly\n"
  "  whether is is in documented YYYYMMDD format or as seconds from\n"
  "  start of years 1970.\n" 
  "2011-01-11 1.5.9 VO\n"
  "  Bug fixes in reading micropet images.\n"
  "2011-02-04 1.5.10 VO\n"
  "  Label unitless was previously not written in main header\n"
  "  when IMG data is written in ECAT7 format.\n"
  "2011-08-12 1.5.11 VO\n"
  "  Supports reading MicroPET images with header version 001.701\n"
  "  (previously only 001.830).\n"
  ,fp);
  fputs("2011-08-13 1.5.12 VO\n"
  "  MicroPET frame times (sec) are read as floats.\n"
  "  Frame times are saved in SIF with one decimal when necessary.\n"
  "2011-08-17 1.5.13 VO\n"
  "  Changed exp_date format in Analyze header to prevent overflow.\n"
  "2011-09-13 2.0.0 VO\n"
  "  Changes to compile, install and distribute in 64-bit systems.\n"
  "2012-02-04 2.0.1 VO\n"
  "  Branching fraction is included in IMG struct.\n"
  "2012-02-04 2.0.2 VO\n"
  "  anaExistsNew() added to analyze.c.\n"
  "  Some of micropet.c contents moved into new file img_upet.c.\n"
  "2012-02-17 2.0.3 VO\n"
  "  Calibration factor and optional IFT struct is included in IMG struct.\n"
  "  imgInfo() prints more fields.\n"
  "2012-02-24 2.0.4 VO\n"
  "  Partial support for reading microPET image.\n"
  "  Function imgFormatDetermine() added to imgfile.c.\n"
  "  Modality included in IMG struct.\n"
  "  Functions imgGetMicropetHeader(), imgReadMicropetHeader(),\n"
  "  imgReadMicropetFrame(), imgReadMicropetFirstFrame(), and\n"
  "  imgReadMicropet() added to img_upet.c.\n"
  "  Function upetReadImagedata() added to micropet.c.\n"
  "  interfileIsHeader() and interfileExists() added to interfile.c.\n"
  "  imgBranchingCorrection() added to imgdecay.c.\n"
  "2012-02-24 2.0.5 VO\n"
  "  Bugs fixed in analyze.c and imgfile.c; update in imgReadAnalyze().\n"
  "2012-05-09 2.0.6 VO\n"
  "  More unit representations identified in imgunit.c.\n"
  "  Removed unused variables in several source files.\n"
  "2012-08-20 2.0.7 VO\n"
  "  Bugs fixed in vol.c.\n"
  "2012-09-21 2.0.8 VO\n"
  "  Defined IMG_DC_UNKNOWN, IMG_DC_CORRECTED, and IMG_DC_NONCORRECTED\n"
  "  in img.h, and applied these in all functions in this library."
  "  Fixed setting of whether data are corrected for physical decay or not\n"
  "  in reading single frame of ECAT images.\n"
  "2012-09-24 2.0.9 VO\n"
  "  Bug fix in reading 3D sinogram frame times.\n"
  "2012-09-24 2.0.10 VO\n"
  "  Bug fixed in imgfile.c.\n"
  "2012-10-03 2.0.11 VO\n"
  "  Added functions img2sif() and sif2img() in img_ana.c and imgfile.h.\n"
  "  imgWriteAnalyze() tries to save SIF.\n"
  "2012-10-13 2.0.12 VO\n"
  "  Changes in imgInfo().\n"
  "2012-10-19 2.0.13 VO\n"
  "  Added functions in img.c, vol.c, and imgmax.c.\n"
  "2012-10-23 2.0.14 VO\n"
  "  Changed interface of volAvg() and volMax().\n"
  "2012-10-27 2.0.15 VO\n"
  "  Changed interface of imgSmoothMax().\n"
  "2012-12-04 2.0.16 VO\n"
  "  Added functions in imgmax.c.\n"
  "2013-03-07 2.0.17 VO\n"
  "  Added function imgDup() in img.c and .h.\n"
  ,fp);
  fputs("2013-03-18 2.1.0 VO\n"
  "  Added nifti.c, nifti.h and img_nii.c with preliminary and simplistic\n"
  "  NIfTI-1 IO.\n"
  "  Added new fields to IMG struct.\n"
  "  License updated to GNU LGPL 3.\n"
  "2013-03-22 2.1.1 VO\n"
  "  Tiny changes in analyze.c and addition in nifti.h.\n"
  "  Added niftiWriteHeader() in nifti.c.\n"
  "2013-03-26 2.1.2 VO\n"
  "  Added niftiCreateFNames() and niftiRemove() in nifti.c.\n"
  "  Bug fixes in niftiExists() and niftiWriteHeader().\n"
  "  Added imgSetNiftiHeader() and imgWriteNiftiFrame() in img_nii.c.\n"
  "  imgReadNiftiFrame() does not flip the image.\n"
  "  imgGetNiftiHeader() sets _fileFormat, not in imgReadNiftiHeader().\n"
  "2013-04-02 2.1.3 VO\n"
  "  Added imgWriteNifti().\n"
  "  Added more orientation and transformation fields to IMG struct.\n"
  "  imgRead(), imgWrite(), imgReadFrame(), imgReadHeader(),\n"
  "  imgFormatDetermine(), and imgFormatFromFName() support NIfTI-1.\n"
  "  imgReadNiftiFrame() sets IMG plane numbers.\n"
  "  Moved imgmatch.c from libtpcimgp to this library.\n"
  "2013-04-03 2.1.4 VO\n"
  "  Fixed a serious bug in reading NIfTI.\n"
  "2013-04-04 2.1.5 VO\n"
  "  Added two functions in img.c.\n"
  "  Updated img2sif() and sif2img().\n"
  "  imgWriteNifti() and imgWriteAnalyze() create count data in a new SIF.\n"
  "  Added sifExistentCounts() in weight.c\n"
  "2013-06-20 2.1.6 VO\n"
  "  imgWriteFrame() writes NIfTI-1, but still not Analyze.\n"
  "\n",fp);
  fflush(fp);  
}

/*!
 * Print the library Readme to specified FILE pointer with Doxygen tags,
 * use stdout for screen.
 *
 * @param fp file pointer where to print
 */
void libtpcimgio_print_dreadme(FILE *fp) {
  fprintf(fp,"/**\n\\mainpage %s %s %s\n\\section readme Readme\n<pre>\n",
    LIB_NAME,LIB_VERSION,COPYRIGHT);
  libtpcimgio_print_readme(fp);
  fprintf(fp,"\n</pre> \n */\n");
}

/*!
 * Print the library History to specified FILE pointer with Doxygen tags,
 * use stdout for screen.
 *
 * @param fp file pointer where to print
 */
void libtpcimgio_print_dhistory(FILE *fp) {
  fprintf(fp,"/**\n\\page History\n\\section history History\n<pre>\n");
  libtpcimgio_print_history(fp);
  fprintf(fp,"\n</pre> \n */\n");
}

/*!
 * Print the build info to specified FILE pointer, use stdout for screen.
 *
 * @param fp file pointer where to print
 */
void libtpcimgio_print_build(FILE *fp) 
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
}

/*!
 * Print the compilation date and time to specified FILE pointer
 *
 * @param fp file pointer where to print
 */
void libtpcimgio_printdate(FILE *fp) {
  libtpcimgio_print_build(fp);
}
/*****************************************************************************/

/*****************************************************************************/
