/******************************************************************************

  Copyright (c) 2003-2013 Turku PET Centre

  Library:     imgfile.c
  Description: I/O routines for IMG data.
               Currently supported file formats:
               ECAT 6.3 images and sinograms
               ECAT 7.x 2D and 3D images (volumes) and sinograms
               Analyze 7.5 images
               NIfTI-1 images
               microPET images (only reading)

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

  Turku PET Centre, Turku, Finland, http://www.turkupetcentre.fi/

  Modification history:
  2003-07-27 Vesa Oikonen
    First created.
  2003-08-11 VO
    Corrected a bug in test print in imgRead().
  2003-09-08 VO
    Can now read&write 3D sinograms.
    sampleDistance is now read/written from/to bin_size in ECAT7 mainheader.
    imgWriteEcat7() renamed to imgWrite2DEcat7().
    "New" function imgWriteEcat7() makes 3D format instead of 2D as before.
    Applies the new IMG field _fileFormat in read/write.
  2003-10-05 VO
    Added support for Analyze 7.5 image format.
  2003-10-07 VO
    Setting Analyze db_name is corrected.
  2003-10-08 VO
    Only 7 first letters of image magic number is checked.
  2003-10-21 VO
    imgRead() and imgReadAnalyze() may work even if filename is given
    with extension.
  2003-11-04 VO
    If ECAT 6.3 image mainheader and subheader contain the same calibration
    factor, it is used only once.
    Works with unit nCi/mL.
  2003-11-10 VO
    ECAT 7 3D scan subheader field x_resolution is used as sample distance.
  2003-11-12 VO
    Reading ECAT 6.3 image: pixels x size is read from subheader slice_width
    if it is not found in main header plane_separation field.
  2003-11-30 VO
    For now, calls temp_roundf() instead of roundf().
  2003-12-05 VO
    Analyze files may be read&write without flipping in x,y,z-directions.
    Function anaFlipping() is used to determine whether to flip or not.
  2003-12-10 VO
    Changes in 2003-11-10 and 2003-11-12 had been accidentally deleted
    and are now returned.
  2003-12-14 VO
    Patient orientation is read&written in ECAT7 format.
    Scanner (system_type) is read&written in ECAT formats.
  2004-02-05 VO
    anaFlipping() determines whether image is flipped in z-direction;
    image is always flipped in x,y-directions.
  2004-02-08 VO
    Changes in imgSetEcat7MHeader():
    -sets different magic number for sinograms and image volumes
    -sets sw_version=72.
    When writing ECAT 6.3 format: sets sw_version=2.
  2004-02-22 VO
    Analyze format: zoom factor is not written into/read from funused2,
    because that field is used by SPM2 to another purpose.
  2004-05-23 VO
    ECAT7_3DSCANFIT is now supported as ECAT7_3DSCAN.
    imgReadEcat7() reports that axial compression is not supported.
    ECAT unit 1 is assumed to represent MBq/mL (IMG unit 13).
  2004-05-24 VO
    Pixel sizes are correctly converted from mm to cm when writing ECAT7.
    Changes in unit conversions between ECAT and IMG.
  2004-06-27 VO
    Additional error message.
    ecat63ReadAllToImg() and ecat63ReadPlaneToImg() do not even try to read
    extra frames.
    ECAT63_TEST changed to IMG_TEST.
    Patient name is copied with strncpy in ecat63ReadPlaneToImg() and
    ecat63ReadAllToImg().
  2004-07-10 VO
    Not so many test prints in reading ECAT 6.3 files.
  2004-08-23 VO
    MAX_STUDYNR_LEN applied where appropriate.
  2004-09-20 VO
    Doxygen style comments are corrected.
  2004-09-24 VO
    Better handling of ECAT7 calibration units.
    E.g. imgUnitToEcat7() divided into imgUnitToEcat6() and imgUnitToEcat7().
  2004-10-10 VO
    imgSetEcat7MHeader() sets ECAT7 mainheader plane number to dimz, not 1
    as before.
  2004-10-13 VO
    tm_isdst=-1 (unknown Daylight saving time) when appropriate.
  2004-11-07
    imgReadEcat7() reads files with axial compression, each slice as one plane.
  2004-12-22 VO
    Correction in imgGetEcat7MHeader(): calls imgUnitFromEcat7() instead of
    imgUnitFromEcat().
  2004-12-27 VO
    imgUnitFromEcat(): IMG unit is set to unknown, not to MBq/mL, when ECAT
    unit is unknown.
  2005-03-03 Jarkko Johansson
    initSIF changed to sifInit and readSIF to sifRead
  2005-05-12 Calle Laakkonen
    made image loading/saving functions filename argument const
  2005-06-06 Calle Laakkonen
    imgmsg is now shared by all functions.
  2005-06-30 Harri Merisaari
    fixed imgWrite(char* fname, IMG* img) to write also image structures 
    with '_fileFormat' field set as 'IMG_ANA_L' 
  2005-10-10 Calle Laakkonen
    imgWriteAnalyze() now conserves memory by writing only 1 frame at a time.
  2005-12-12 VO
    imgReadAnalyze() sets img.isotopeHalflife, if isotope is found in SIF.
    Corrected a setting of error message in imgRead().
  2006-10-31 VO
    Calibration unit functions moved to imgunit.c.
    Return value of mktime() is checked.
    Added "if(timezone == -7200) img.scanStart += 10800;"
    into imgGetEcat7MHeader() as suggested by HM.
  2007-01-31 VO
    Corrected a bug in imgReadEcat7() with ECAT7 3D sinograms: all planes, not
    only the first, are multiplied with dead-time correction factor.
    Analyze 7.5 I/O functions separated into imgana.c.
    ECAT 6.3 I/O functions separated into img_e63.c.
    IMG status messages separated into imgmsg.h.
    If valid study number is found in ECAT7 user_process_code, 
    then take it from there.  
    Patient_id and study_description are read and written in ECAT7.
    Image x,y,z-resolutions are read/written in ECAT7. 
    Prompts and randoms (delayed) are read and written in ECAT7.
  2007-02-27 VO
    ECAT 7 I/O functions separated into img_e7.c.
  2007-03-25 VO
    Added functions imgFormatFromFName(), imgReadHeader(), imgReadNextFrame(),
    imgReadFrame(), and imgWriteFrame().
  2007-03-27 VO
    imgFormatFromFName() identifies extension .polmap as polar map.
    imgWrite() calls imgFormatFromFName() when necessary.
    Read and write functions accept polar maps.
  2007-07-17 HM
    Changed void imgFormatFromFName(IMG *img, const char *fname) for ANSI.
  2012-02-04 VO
    imgRead() and imgReadHeader() identify microPET file, but do not support
    those any further.
    imgRead() uses anaExistsNew() instead of anaExists().
  2012-02-23 VO
    Added function imgFormatDetermine().
    imgFormatFromFName() identifies .dcm extension as DICOM and .i.hdr as
    Interfile.
    Functions imgRead(), imgReadFrame() and imgReadHeader() support microPET
    format, although support is not yet full in the functions that they call.
  2012-02-28 VO
    Fixed a recent bug in imgRead() which caused error if Analyze file was
    not accompanied by SIF.
  2012-09-24 VO
    Bug fix in imgFormatFromFName().
    Added a few test prints.
  2013-04-02 VO
    imgFormatFromFName() identifies extension .nii.
    imgFormatDetermine() identifies single and dual file format NIfTI.
    imgRead() and imgWrite() supports NIfTI format.
  2013-06-20 VO
    imgWriteFrame() writes NIfTI-1, without global min and max.



******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>
/*****************************************************************************/
#include "petc99.h"
#include "swap.h"
#include "halflife.h"
#include "substitutions.h"
/*****************************************************************************/
#include "img.h"
#include "sif.h"
#include "ecat63.h"
#include "ecat7.h"
#include "analyze.h"
#include "micropet.h"
#include "interfile.h"
#include "imgmax.h"
#include "imgdecay.h"
#include "imgfile.h"
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Read an image or sinogram file in ECAT 6.3 or ECAT 7.x format,
 * or image in NIfTI-1, Analyze 7.5, or microPET format.
 *
 * @param fname input filename
 * @param img pointer to initialized IMG structure
 * @return 0 if ok, 1 invalid input, 2 image status is not 'initialized', 
 * 4 unrecognized format, 5 unsupported Ecat7 type,
 * sets IMG->statmsg in case of error
 */
int imgRead(const char *fname, IMG *img) {
  FILE *fp;
  int ret;
  ECAT7_mainheader ecat7_main_header;
  ECAT63_mainheader ecat63_main_header;
  char temp[FILENAME_MAX];
  //char *cptr;

  if(IMG_TEST) printf("imgRead(%s, *img)\n", fname);
  /* Check the arguments */
  if(fname==NULL) {img->statmsg=imgStatus(STATUS_FAULT); return(1);}
  if(img==NULL || img->status!=IMG_STATUS_INITIALIZED) {
    img->statmsg=imgStatus(STATUS_FAULT); return(2);}

  /* Check if we have NIfTI file, which may be in single file format,
     or dual file format which has similar names as microPET and Analyze */
  if(niftiExists(fname, NULL, NULL, NULL, NULL, IMG_TEST-3, NULL)>0) {
    /* Read NIfTI image */
    ret=imgReadNifti(fname, img, IMG_TEST);
    if(IMG_TEST) {printf("imgReadNifti() := %d\n", ret); fflush(stdout);}
    if(ret==STATUS_OK) {
      if(IMG_TEST) printf("%s identified as supported NIfTI.\n", fname);
      img->statmsg=imgStatus(STATUS_OK);
      return(STATUS_OK);
    }
    img->statmsg=imgStatus(ret); return(4);
  }

  /* Check if we have microPET or Analyze file, which consist of separate header
     and data files, and have similar names */
  if(upetExists(fname, NULL, NULL)!=0) {
    /* Read microPET image */
    ret=imgReadMicropet(fname, img); if(ret!=STATUS_OK) return(3);
    if(IMG_TEST) printf("%s identified as microPET format.\n", fname);
    return(0);
  }
  if(anaExistsNew(fname, temp, NULL, NULL)!=0) {
    anaRemoveFNameExtension(temp);
    /* Read Analyze image */
    ret=imgReadAnalyze(temp, img);
    if(IMG_TEST) {printf("imgReadAnalyze() := %d\n", ret); fflush(stdout);}
    if(ret==STATUS_OK) {
      if(IMG_TEST) printf("%s identified as supported Analyze 7.5 format.\n",
        fname);
      img->statmsg=imgStatus(STATUS_OK);
      return(0);
    }
    if(ret==STATUS_NOSIFDATA || ret==STATUS_WRONGSIFDATA) {
      img->statmsg=imgStatus(ret); return(0);}
    img->statmsg=imgStatus(ret); return(4);
  }

  /* Check if we have an ECAT file */
  /* Open file for read */
  if((fp=fopen(fname, "rb")) == NULL) {
    img->statmsg=imgStatus(STATUS_NOFILE); return(4);
  }
  /* Try to read ECAT 7.x main header */
  ret=ecat7ReadMainheader(fp, &ecat7_main_header);
  if(ret) {fclose(fp); img->statmsg=imgStatus(STATUS_UNKNOWNFORMAT); return(4);}
  /* If header could be read, check for magic number */
  if(strncmp(ecat7_main_header.magic_number, ECAT7V_MAGICNR, 7)==0) {
    /* This is ECAT 7.x file */
    /* Check if file type is supported */
    if(imgEcat7Supported(&ecat7_main_header)==0) {
      fclose(fp); img->statmsg=imgStatus(STATUS_UNSUPPORTED); return(5);
    }
    fclose(fp);
    /* Read file */
    if(IMG_TEST) printf("%s identified as supported ECAT 7.x %s format\n",
      fname, ecat7filetype(ecat7_main_header.file_type));
    ret=imgReadEcat7(fname, img);
    if(ret) {if(IMG_TEST) printf("imgReadEcat7()=%d\n", ret); return(6);}
  } else {
    /* Check if file is in ECAT 6.3 format */
    ret=ecat63ReadMainheader(fp, &ecat63_main_header);
    fclose(fp);
    if(ret==0) {
      /* It seems to be ECAT 6.3, so read it */
      if(IMG_TEST) printf("%s identified as supported ECAT 6.3 %s format\n",
        fname, ecat7filetype(ecat63_main_header.file_type));
      ret=ecat63ReadAllToImg(fname, img);
      if(ret) {
        if(IMG_TEST) fprintf(stderr, "ecat63ReaddAllToImg: %s\n", ecat63errmsg);
        if(ret==6) img->statmsg=imgStatus(STATUS_MISSINGMATRIX);
	else img->statmsg=imgStatus(STATUS_UNSUPPORTED);
        return(6);
      }
    } else {img->statmsg=imgStatus(STATUS_UNKNOWNFORMAT); return(4);}
  }
  img->statmsg=imgStatus(STATUS_OK);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Write an image or sinogram file.
 * Format depends on _fileFormat or filename extension.
 *
 * @param fname output filename
 * @param img pointer to IMG data
 * @return 0 if ok, 1 invalid input, 2 invalid image type or status, 
 * 5 failed to write file, sets IMG->statmsg in case of error
 */
int imgWrite(const char *fname, IMG *img) {
  int ret;

  if(IMG_TEST) printf("imgWrite(%s, *img)\n", fname);
  /* Check the arguments */
  if(fname==NULL) return(1);
  if(img==NULL || img->status!=IMG_STATUS_OCCUPIED) {
    imgSetStatus(img, STATUS_FAULT); return(2);}
  if(img->type!=IMG_TYPE_RAW &&
     img->type!=IMG_TYPE_IMAGE &&
     img->type!=IMG_TYPE_POLARMAP) {
    imgSetStatus(img, STATUS_FAULT); return(2);}

  /* If _fileFormat is not defined, then determine it from the filename */
  if(img->_fileFormat==IMG_UNKNOWN) {
    if(IMG_TEST>1) printf("  file format determined based on filename\n");
    imgFormatFromFName(img, fname);
    if(IMG_TEST>1) printf("  _fileFormat := %d\n", img->_fileFormat);
  }

  /* Write */
  if(img->_fileFormat==IMG_E63) {
    ret=ecat63WriteAllImg(fname, img);
    switch(ret) {
      case 0: break;
      case 4: imgSetStatus(img, STATUS_NOMEMORY); break;
      case 3: imgSetStatus(img, STATUS_NOWRITEPERM); break;
      case 9: imgSetStatus(img, STATUS_DISKFULL); break;
      default: imgSetStatus(img, STATUS_FAULT);
    }
    if(ret) return(7);
  } else if(img->_fileFormat==IMG_ANA || img->_fileFormat==IMG_ANA_L) {
    ret=imgWriteAnalyze(fname, img); if(ret) return(5);
  } else if(img->_fileFormat==IMG_NIFTI_1S || img->_fileFormat==IMG_NIFTI_1D) {
    ret=imgWriteNifti(fname, img, 1, IMG_TEST-1); if(ret) return(5);
  } else if(img->_fileFormat==IMG_E7_2D) {
    ret=imgWrite2DEcat7(fname, img); if(ret) return(5);
  } else if(img->_fileFormat==IMG_POLARMAP) {
    ret=imgWritePolarmap(fname, img); if(ret) return(5);
  } else {
    ret=imgWriteEcat7(fname, img); if(ret) return(5);
  }
  imgSetStatus(img, STATUS_OK);
  return(0);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Fill IMG struct header information from an image or sinogram file
 *  in ECAT 6.3, ECAT 7.x or Analyze 7.5 format.
 *  Information concerning separate frames or planes is not filled.
 *
 * @param fname image or sinogram filename
 * @param img pointer to initialized but not allocated IMG structure
 * @return errstatus, which is STATUS_OK (0) when call was successful,
 * and >0 in case of an error.
 */
int imgReadHeader(
  const char *fname, IMG *img
) {
  int ret;
  char temp[FILENAME_MAX];

  if(IMG_TEST) printf("\nimgReadHeader(%s, *img)\n", fname);

  /* Check the arguments */
  if(fname==NULL) return STATUS_FAULT;
  if(img==NULL) return STATUS_FAULT;
  if(img->status!=IMG_STATUS_INITIALIZED) return STATUS_FAULT;

  /* Check if we have NIfTI file, which may be in single file format,
     or dual file format which has similar names as microPET and Analyze */
  if(niftiExists(fname, NULL, NULL, NULL, NULL, IMG_TEST-3, NULL)>0) {
    /* Read NIfTI image header information */
    ret=imgReadNiftiHeader(fname, img, IMG_TEST);
    imgSetStatus(img, ret);
    return(ret);
  }

  /* Check if we have got microPET data */
  if(upetExists(fname, temp, NULL)!=0) {
    /* Read microPET header information */
    ret=imgReadMicropetHeader(temp, img);
    imgSetStatus(img, ret);
    return(ret);
  }

  /* Check if we have got Analyze data */
  if(anaDatabaseExists(fname, temp, NULL, NULL)>0) { // yes we have Analyze 7.5
    /* Read Analyze header information */
    ret=imgReadAnalyzeHeader(temp, img);
    imgSetStatus(img, ret);
    return(ret);
  }

  /* Is this an ECAT7 file */
  ret=imgReadEcat7Header(fname, img);
  /* If main header was read but format was not identified as Ecat7, 
     it might be in Ecat6 format */
  if(ret==STATUS_UNKNOWNFORMAT) {
    /* Is this an ECAT6 file; check this as the last option, because
       ECAT6 files don't contain any magic number */
    ret=imgReadEcat63Header(fname, img);
    if(ret) return STATUS_UNKNOWNFORMAT; /* don't know what this is */
    /* Fine, it is Ecat 6.3 (or close enough) */
    imgSetStatus(img, STATUS_OK);
    return STATUS_OK;
  }

  /* Check if we have got Interfile data */
  if(interfileExists(fname, temp, NULL, IMG_TEST-2)!=0) {
    imgSetStatus(img, STATUS_UNSUPPORTED);
    return(STATUS_UNSUPPORTED);
  }
  
  imgSetStatus(img, ret);
  return ret;
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Read one time frame from a supported PET image or sinogram file into
 *  IMG data structure. This functions can be called repeatedly to read all
 *  the frames one at a time to conserve memory.
 *
 * @param fname name of file from which IMG contents will be read.
 *  Currently supported file formats are ECAT 6.3 images and sinograms,
 *  ECAT 7.x 2D and 3D images and sinograms, and NIfTI-1, Analyze 7.5, and
 *  microPET 3D and 4D images.
 * @param frame_to_read frame which will be read [1..frameNr]
 * @param img pointer to initiated or occupied IMG data.
 *  If occupied, then new frame is tested to match the previous file type,
 *  dimensions, and other fundamental information contained in the IMG.
 *  If not occupied, then memory is allocated here.
 * @param frame_index IMG frame index (0..dimt-1) where data will be placed.
 *  If index is >0, then the memory for that frame must be allocated before
 *  calling this function.
 * @return errstatus, which is STATUS_OK (0) when call was successful,
 * and >0 in case of an error.  Specifically, return value STATUS_NOMATRIX
 * signals that frame does not exist, i.e. all frames have been read.
 * IMG.statmsg can be set using ERROR_STATUS.
 */
int imgReadFrame(
  const char *fname, int frame_to_read, IMG *img, int frame_index
) {
  IMG test_img;
  int ret=0;

  if(IMG_TEST) printf("\nimgReadFrame(%s, %d, *img, %d)\n",
    fname, frame_to_read, frame_index);
    
  /*
   *  Check the input 
   */
  if(fname==NULL) return STATUS_FAULT;
  if(img==NULL) return STATUS_FAULT;
  if(img->status!=IMG_STATUS_INITIALIZED && img->status!=IMG_STATUS_OCCUPIED)
    return STATUS_FAULT;
  if(frame_to_read<1) return STATUS_FAULT;
  if(frame_index<0) return STATUS_FAULT;
  /* if frame_index>0, then there must be sufficient memory allocated for it */
  if(frame_index>0) {
    if(img->status!=IMG_STATUS_OCCUPIED) return STATUS_FAULT;
    if(frame_index>img->dimt-1) return STATUS_FAULT;
  }

  /*
   *  If IMG is preallocated, check that fundamental header information
   *  is compatible with old and new contents.
   *  If not allocated, then read the header contents, and allocate it
   */
  imgInit(&test_img);
  if(img->status==IMG_STATUS_OCCUPIED) {
    ret=imgReadHeader(fname, &test_img); imgSetStatus(&test_img, ret);
    if(IMG_TEST>1)
      printf("imgReadHeader() return message := %s\n", test_img.statmsg);
    if(ret) return(ret);
    if(IMG_TEST>3) imgInfo(&test_img);
    /* Test that file format and type are the same */
    ret=0;
    if(img->type!=test_img.type) ret++;
    if(img->_fileFormat!=test_img._fileFormat) ret++;
    /* Test that x, y, and z dimensions are the same */
    if(img->dimx!=test_img.dimx) ret++;
    if(img->dimy!=test_img.dimy) ret++;
    if(img->dimz!=test_img.dimz) ret++; 
    imgEmpty(&test_img); if(ret>0) return STATUS_INVALIDHEADER;
  } else {
    ret=imgReadHeader(fname, img); imgSetStatus(img, ret);
    if(IMG_TEST>1)
      printf("imgReadHeader() return message := %s\n", img->statmsg);
    if(ret) return(ret);
    if(IMG_TEST>3) imgInfo(img);
    /* Allocate memory for one frame */
    img->dimt=1;
    ret=imgAllocate(img, img->dimz, img->dimy, img->dimx, img->dimt);
    if(ret) return STATUS_NOMEMORY;
  }

  /*
   *  Read the frame data and corresponding information like frame time
   *  if available   
   */
  switch(img->_fileFormat) {
    case IMG_E7:
    case IMG_E7_2D:
    case IMG_POLARMAP:
      ret=imgReadEcat7Frame(fname, frame_to_read, img, frame_index);
      if(IMG_TEST>1) printf("imgReadEcat7Frame() return value := %d\n", ret);
      break;
    case IMG_E63:
      ret=imgReadEcat63Frame(fname, frame_to_read, img, frame_index);
      if(IMG_TEST>1) printf("imgReadEcat63Frame() return value := %d\n", ret);
      break;
    case IMG_ANA:
    case IMG_ANA_L:
      ret=imgReadAnalyzeFrame(fname, frame_to_read, img, frame_index);
      if(IMG_TEST>1) printf("imgReadAnalyzeFrame() return value := %d\n", ret);
      break;
    case IMG_NIFTI_1S:
    case IMG_NIFTI_1D:
      ret=imgReadNiftiFrame(fname, frame_to_read, img, frame_index, 0);
      if(IMG_TEST>1) printf("imgReadNiftiFrame() return value := %d\n", ret);
      break;
    case IMG_MICROPET:
      ret=imgReadMicropetFrame(fname, frame_to_read, img, frame_index);
      if(IMG_TEST>1) printf("imgReadAnalyzeFrame() return value := %d\n", ret);
      break;
    default:
      ret=STATUS_UNSUPPORTED;
  }
  imgSetStatus(img, ret);
  return ret;
}
/*****************************************************************************/

/*****************************************************************************/
/** Write one PET frame from IMG data struct into a supported PET image or
 *  sinogram file. This function can be called repeatedly to write all
 *  frames one at a time to conserve memory.
\return Returns errstatus, which is STATUS_OK (0) when call was successful,
    and >0 in case of an error.
 */
int imgWriteFrame(
  /** Name of file where IMG contents will be written.
      Currently supported file formats are ECAT 6.3 images and sinograms, and
      ECAT 7.x 2D and 3D images and sinograms, and NIfTI-1 images.
      Analyze 7.5 images are NOT supported (because global min and max would
      be needed).
     If file exists, data is either overwritten or catenated as a new frame,
     depending on the following arguments.
     If file does not exist, it is created. */
  const char *fname,
  /** PET frame number (1..frameNr) which will be written:
      If set to 0, frame data will be written to an existing or new PET file as
      a new frame, never overwriting existing data.
      If >0, then frame data is written as specified frame number, overwriting
      any data existing with the same frame number */
  int frame_to_write,
  /** Pointer to the IMG data struct */
  IMG *img,
  /** IMG frame index (0..dimt-1) which will be written */
  int frame_index
) {
  int ret=0;

  if(IMG_TEST) printf("\nimgWriteFrame(%s, %d, *img, %d)\n",
    fname, frame_to_write, frame_index);
    
  /*
   *  Check the input 
   */
  if(fname==NULL) return STATUS_FAULT;
  if(img==NULL) return STATUS_FAULT;
  if(img->status!=IMG_STATUS_OCCUPIED) return STATUS_FAULT;
  if(frame_to_write<0) return STATUS_FAULT;
  if(frame_index<0 || frame_index>=img->dimt) return STATUS_FAULT;


  /*
   *  Call separate function for each supported file format
   */
  imgFormatFromFName(img, fname);
  switch(img->_fileFormat) {
    case IMG_E7:
    case IMG_E7_2D:
    case IMG_POLARMAP:
      ret=imgWriteEcat7Frame(fname, frame_to_write, img, frame_index);
      break;
    case IMG_E63:
      ret=imgWriteEcat63Frame(fname, frame_to_write, img, frame_index);
      break;
    case IMG_ANA:
    case IMG_ANA_L:
      ret=STATUS_UNSUPPORTED;
      /* Not supported because would require global min&max values
       * if saved in short ints which is now the only possibility
       * ret=imgWriteAnaFrame(fname, frame_to_write, img, frame_index);
       */
      break;
    case IMG_NIFTI_1D:
    case IMG_NIFTI_1S:
#if(0)
      /* Not supported because would require global min&max values
       * if saved in short ints which is now the only possibility
       */
      ret=STATUS_UNSUPPORTED;
#endif
      /* Nifti is currently always written as floats, therefore
         global min and max pixel values are not yet needed */
      ret=imgWriteNiftiFrame(fname, frame_to_write, img, frame_index,
                             0, 0, IMG_TEST-2);
      break;
    default:
      ret=STATUS_UNSUPPORTED;
  }
  imgSetStatus(img, ret);
  return ret;
}
/*****************************************************************************/

/*****************************************************************************/
/**
 * Determine IMG _fileFormat from filename extension, if not already defined.
 * Default is ECAT 7 image volume, if nothing else can be guessed.
 * Note that NIfTI dual file format, Analyze, and microPET files can not be
 * separated by file naming, thus all of these formats will be identified as
 * Analyze by this function (default may be changed to NIfTI in future).
 *
 * @param img target image struture where fileformat is saved,
 * should have IMG_UNKNOWN as file type
 * @param fname name of file that is detemined
 */
void imgFormatFromFName(
  IMG *img, const char *fname
) {
  char *cptr=NULL, *cptr2=NULL, temp[FILENAME_MAX];

  if(IMG_TEST>2) printf("imgFormatFromFName(img, %s)\n", fname);
  if(img->_fileFormat!=IMG_UNKNOWN && img->_fileFormat>0) {
    if(IMG_TEST>3)
      printf("  _fileFormat := %d, not changed\n", img->_fileFormat);
    return;
  }
  img->_fileFormat=IMG_E7; /* default */
  /* get extensions */
  strcpy(temp, fname); cptr=strrchr(temp, '.');
  if(cptr!=NULL) {
    *cptr=(char)0; cptr++;
    cptr2=strrchr(temp, '.'); if(cptr2!=NULL) {*cptr2=(char)0; cptr2++;}
  }
  if(cptr2!=NULL) {
    if(strcasecmp(cptr2, "i.hdr")==0) { img->_fileFormat=IMG_INTERFILE; return;}
    if(strcasecmp(cptr2, "i.img")==0) { img->_fileFormat=IMG_INTERFILE; return;}
  }
  if(cptr!=NULL) {
    if(strcasecmp(cptr, "hdr")==0) { img->_fileFormat=IMG_ANA; return;}
    if(strcasecmp(cptr, "polmap")==0) { img->_fileFormat=IMG_POLARMAP; return;}
    if(strcasecmp(cptr, "img")==0 ||
       strcasecmp(cptr, "scn")==0 ||
       strcasecmp(cptr, "nrm")==0 ||
       strcasecmp(cptr, "atn")==0) {
      img->_fileFormat=IMG_E63; return;
    }
    if(strcasecmp(cptr, "dcm")==0) { img->_fileFormat=IMG_DICOM; return;}
    if(strcasecmp(cptr, "i")==0) { img->_fileFormat=IMG_INTERFILE; return;}
    if(strcasecmp(cptr, "nii")==0) { img->_fileFormat=IMG_NIFTI_1S; return;}
  } else { /* no extension at all */
    img->_fileFormat=IMG_ANA;
  }
}
/*****************************************************************************/

/*****************************************************************************/
/** Determine the file format and type of an existing PET image data.
 *  Note that this function only identifies those formats and types that were
 *  needed in TPC at the time of writing this function.
 *  Note also that this function does not care whether the format is fully or
 *  not at all supported by other library functions.
 *  
\return Returns 0 when no errors occurred, which does not mean that format
    was identified.
 */
int imgFormatDetermine(
  /** Full name of image file. In case of Analyze or microPET image, hdr or img
   *  filename, or basename without extension is accepted. Pathname is not
   *  accepted. */
  const char *fname,
  /** Pointer to allocated string where image filename without extensions
   *  is written (only for Analyze and microPET); enter NULL, if not needed. */ 
  char *basename,
  /** Pointer to allocated string where header filename without extensions
   *  is written (only for Analyze and microPET); enter NULL, if not needed. */ 
  char *hdrfile,
  /** Pointer to allocated string where image data filename without extensions
   *  is written (only for Analyze and microPET); enter NULL, if not needed. */ 
  char *imgfile,
  /** Pointer to allocated string where SIF filename without extensions
   *  is written (only for Analyze), if available; enter NULL, if not needed. */ 
  char *siffile,
  /** Pointer to int where image format ID is written; 0=unknown, for other
   *  formats see definitions in img.h */
  int *file_format,
  /** Pointer to int where scanner ID is written; 0=unknown, for other
   *  formats see definitions in img.h */
  int *scanner,
  /** Pointer to int where image type is written; 0=unknown, 1=image,
   *  2=sinogram, 3=polarmap, please check definitions in img.h */
  int *type,
  /** Pointer to int where modality is written; 0=unknown, 1=PET,
   *  2=MRI, 3=CT, ..., please check definitions in img.h */
  int *modality,
  /** Verbose level; if zero, then nothing is printed into stdout or stderr */
  int verbose
) {
  char *cptr, temp[FILENAME_MAX];
  int ret, fformat=IMG_UNKNOWN;
  NIFTI_DSR nifti_dsr;
  IMG img;

  if(verbose>0) printf("imgFormatDetermine(\"%s\", ...)\n", fname);
  /* Initiate results */
  if(basename!=NULL) strcpy(basename, "");
  if(hdrfile!=NULL) strcpy(hdrfile, "");
  if(imgfile!=NULL) strcpy(imgfile, "");
  if(siffile!=NULL) strcpy(siffile, "");
  if(file_format!=NULL) *file_format=IMG_UNKNOWN;
  if(scanner!=NULL) *scanner=SCANNER_UNKNOWN;
  if(type!=NULL) *type=IMG_TYPE_UNKNOWN;
  if(modality!=NULL) *modality=IMG_MODALITY_UNKNOWN;
  if(fname==NULL) return STATUS_FAULT;
  if(strlen(fname)<1) return STATUS_NOFILE;

  /* Check the image data exists and is accessible */
  strcpy(temp, fname);
  if(access(temp, 0) == -1) {
    if(verbose>1) printf("  file is not directly accessible.\n");
    /* Try to add .nii to the name */
    sprintf(temp, "%s.nii", fname);
    if(access(temp, 0) == -1) {
      if(verbose>1)
        printf("  file is not accessible with .nii extension.\n");
      /* Try to add .img to the name */
      sprintf(temp, "%s.img", fname);
      if(access(temp, 0) == -1) {
        if(verbose>1)
          printf("  file is not accessible with .img extension.\n");
        /* Try to add .hdr to the name */
        sprintf(temp, "%s.hdr", fname);
        if(access(temp, 0) == -1) sprintf(temp, "%s.i.hdr", fname);
        if(access(temp, 0) == -1) sprintf(temp, "%s.img.hdr", fname);
        if(access(temp, 0) == -1) {
          if(verbose>1)
            printf("  file is not accessible with .hdr extension.\n");
          /* Try to add .dcm to the name */
          sprintf(temp, "%s.dcm", fname);
          if(access(temp, 0) == -1) {
            if(verbose>1)
              printf("  file is not accessible with .dcm extension.\n");
            return STATUS_NOFILE;
          }
        }
      }
    }
  }
  if(verbose>1) printf("'%s' is accessible.\n", temp);

  /* DICOM is identified from the filename extension, and not processed any
     further */
  cptr=strrchr(temp, '.');
  if(cptr!=NULL && strcasecmp(cptr, ".DCM")==0) {
    fformat=IMG_DICOM; if(file_format!=NULL) *file_format=fformat;
    if(verbose>1) printf("file was identified to be in DICOM format.\n");
    return STATUS_OK;
  }

  /* Try to read it as ECAT file first, because images which consist of 
     more than one file may reside in the same folder with
     other formats */
  imgInit(&img);
  /* Is this an ECAT7 file */
  ret=imgReadEcat7Header(fname, &img);
  if(ret==STATUS_OK) {
    fformat=img._fileFormat; if(file_format!=NULL) *file_format=fformat;
    if(verbose>1) printf("file was identified to be in ECAT7 format.\n");
  } else if(ret==STATUS_UNKNOWNFORMAT || ret==STATUS_NOFILE) {
    /* If main header was read but format was not identified as Ecat7, 
       it might be in Ecat6 format */
    ret=imgReadEcat63Header(fname, &img);
    /* if necessary, try also with .img added */
    if(ret!=STATUS_OK) {
      sprintf(temp, "%s.img", fname); ret=imgReadEcat63Header(temp, &img);}
    if(ret==STATUS_OK) {
      /* Is this an ECAT6 file; however this is rather uncertain step, because
         ECAT6 files don't contain any magic number */
      fformat=img._fileFormat; if(file_format!=NULL) *file_format=fformat;
      if(verbose>1) printf("file was identified to be in ECAT6 format.\n");
    }
  }

  /* If format was not yet identified, then try to read it as NIfTI;
     this must be done before trying Analyze, because dual format NIfTI
     is compatible with Analyze format */
  if(fformat==IMG_UNKNOWN &&
     niftiExists(fname, hdrfile, imgfile, siffile, &nifti_dsr,
                 verbose-2, NULL)>0) {
    if(verbose>1) printf("file was identified to be in NIfTI format.\n");
    /* Read NIfTI header information into IMG struct */
    ret=imgGetNiftiHeader(&img, &nifti_dsr, verbose-2);
    if(ret==STATUS_OK) {
      fformat=img._fileFormat; if(file_format!=NULL) *file_format=fformat;
    }
  }

  /* If format was not yet identified, then try to read it as microPET */
  if(fformat==IMG_UNKNOWN && upetExists(fname, hdrfile, imgfile)!=0) {
    fformat=IMG_MICROPET; if(file_format!=NULL) *file_format=fformat;
    if(verbose>1) printf("file was identified to be in microPET format\n.");
    /* Read header information from file */
    ret=imgReadMicropetHeader(fname, &img);
    if(ret==STATUS_OK) {
      fformat=img._fileFormat; if(file_format!=NULL) *file_format=fformat;
    }
  }

  /* If format was not yet identified, then try to read it as Analyze */
  if(fformat==IMG_UNKNOWN &&
     anaDatabaseExists(fname, hdrfile, imgfile, siffile)>0) {
    if(verbose>1) printf("file was identified to be in Analyze format.\n");
    fformat=IMG_ANA; if(file_format!=NULL) *file_format=fformat;
    /* Read Analyze header information */
    ret=imgReadAnalyzeHeader(hdrfile, &img);
    if(ret==STATUS_OK) {
      fformat=img._fileFormat; if(file_format!=NULL) *file_format=fformat;
    }
  }

  /* If format was not yet identified, check if it is Interfile */
  if(fformat==IMG_UNKNOWN &&
     interfileExists(fname, hdrfile, imgfile, verbose-1)!=0) {
    fformat=IMG_INTERFILE; if(file_format!=NULL) *file_format=fformat;
    if(verbose>1) printf("file was identified to be in Interfile format\n.");
  }

  /* Fill other information */
  if(scanner!=NULL) *scanner=img.scanner;
  if(type!=NULL) *type=img.type;
  if(modality!=NULL) *modality=img.modality;

  /* Quit */
  imgEmpty(&img);
  if(verbose>1) printf("fformat := %d\n", fformat);
  return STATUS_OK;
}
/*****************************************************************************/

/*****************************************************************************/
