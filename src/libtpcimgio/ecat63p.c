/******************************************************************************

  Copyright (c) 2003-2013 Turku PET Centre

  Library:     ecat63p.c
  Description: Procedures for printing ECAT 6.3 header contents.

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
  2003-07-21 Vesa Oikonen
    Contents separated from ecat63.c.
  2004-02-08 VO
    ecat63PrintMainheader() prints also sw_version.
  2004-09-20 VO
    Doxygen style comments.
  2007-03-12 VO
    Header print functions changed: file pointer is required as second
    argument, set to stdout to make it work as before;
    header contents are now printed one value per line.
  2008-07-24 VO
    Added ecat63PrintSubheader().


******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/*****************************************************************************/
#include "swap.h"
#include "ecat63.h"
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Print ECAT 6.3 mainheader contents to specified file pointer.
 *
 * @param h Ecat 6.3 main header
 * @param fp file pointer
 */
void ecat63PrintMainheader(ECAT63_mainheader *h, FILE *fp) {
  char tmp[32];

  if(ECAT63_TEST) fprintf(stdout, "ecat63PrintMainheader()\n");
  fprintf(fp, "original_file_name := %.20s\n", h->original_file_name);
  fprintf(fp, "data_type := %d (%s)\n", h->data_type, ecat63Datatype(h->data_type));
  fprintf(fp, "system_type := %d\n", h->system_type);
  if(h->file_type==1) strcpy(tmp, "sinogram");
  else if(h->file_type==2) strcpy(tmp, "image");
  else if(h->file_type==3) strcpy(tmp, "attenuation");
  else if(h->file_type==4) strcpy(tmp, "normalization");
  else strcpy(tmp, "unknown");
  fprintf(fp, "file_type := %d (%s)\n", h->file_type, tmp);
  fprintf(fp, "sw_version := %d\n", h->sw_version);
  fprintf(fp, "scan start := %02d.%02d.%04d %02d:%02d:%02d\n",
    h->scan_start_day, h->scan_start_month, h->scan_start_year,
    h->scan_start_hour, h->scan_start_minute, h->scan_start_second);
  fprintf(fp, "isotope_code := %.8s\n", h->isotope_code);
  fprintf(fp, "isotope_halflife := %E sec\n", h->isotope_halflife);
  fprintf(fp, "radiopharmaceutical := %.32s\n", h->radiopharmaceutical);
  fprintf(fp, "gantry_tilt := %g\n", h->gantry_tilt);
  fprintf(fp, "gantry_rotation := %g\n", h->gantry_rotation);
  fprintf(fp, "bed_elevation := %g\n", h->bed_elevation);
  fprintf(fp, "axial_fov := %g\n", h->axial_fov);
  fprintf(fp, "transaxial_fov := %g\n", h->transaxial_fov);
  fprintf(fp, "calibration_factor := %g\n", h->calibration_factor);
  fprintf(fp, "calibration_units := %d (%s)\n", h->calibration_units, ecat63Unit(h->calibration_units));
  fprintf(fp, "study_name := %.12s\n", h->study_name);
  fprintf(fp, "patient_id := %.32s\n", h->patient_id);
  fprintf(fp, "patient_name := %.32s\n", h->patient_name);
  fprintf(fp, "sex := %c\nage := %.10s\nheight := %.10s\nweigth := %.10s\ndexterity := %c\n",
    h->patient_sex, h->patient_age, h->patient_height, h->patient_weight,
    h->patient_dexterity);
  fprintf(fp, "physician_name := %.32s\noperator_name := %.32s\n",
    h->physician_name, h->operator_name);
  fprintf(fp, "study_description := %.32s\n", h->study_description);
  fprintf(fp, "num_planes := %d\n", h->num_planes);
  fprintf(fp, "num_frames := %d\n", h->num_frames);
  fprintf(fp, "num_gates := %d\n", h->num_gates);
  fprintf(fp, "num_bed_pos := %d\n", h->num_bed_pos);
  fprintf(fp, "init_bed_position := %g\n", h->init_bed_position);
  fprintf(fp, "plane_separation := %g cm\n", h->plane_separation);
  fprintf(fp, "user_process_code := %.10s\n", h->user_process_code);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Print ECAT 6.3 imageheader contents to specified file pointer
 *
 * @param h Ecat 6.3 image header
 * @param fp target file pointer
 */
void ecat63PrintImageheader(ECAT63_imageheader *h, FILE *fp) {
  int i;

  if(ECAT63_TEST) printf("ecat63PrintImageheader()\n");
  fprintf(fp, "data_type := %d (%s)\n", h->data_type, ecat63Datatype(h->data_type));
  fprintf(fp, "dimension_1 := %d\n", h->dimension_1);
  fprintf(fp, "dimension_2 := %d\n", h->dimension_2);
  fprintf(fp, "x_origin := %g\ny_origin := %g\nrecon_scale := %g\n",
    h->x_origin, h->y_origin, h->recon_scale);
  fprintf(fp, "quant_scale := %g\nimage_min := %d\nimage_max := %d\n",
    h->quant_scale, h->image_min, h->image_max);
  fprintf(fp, "slice_width := %g\npixel_size := %g\n", h->slice_width, h->pixel_size);
  fprintf(fp, "frame_start_time := %d\nframe_duration := %d\n",
    h->frame_start_time, h->frame_duration );
  fprintf(fp, "reconstruction_start := %02d.%02d.%04d %02d:%02d:%02d\n",
    h->recon_start_day, h->recon_start_month, h->recon_start_year,
    h->recon_start_hour, h->recon_start_min, h->recon_start_sec);
  fprintf(fp, "filter_code := %d\nimage_rotation := %g\nintrinsic_tilt := %g\n",
    h->filter_code, h->image_rotation, h->intrinsic_tilt);
  fprintf(fp, "filter_params :=");
  for(i=0; i<6; i++) fprintf(fp, " %g", h->filter_params[i]); fprintf(fp, "\n");
  fprintf(fp, "plane_eff_corr_fctr := %g\ndecay_corr_fctr := %g\nloss_corr_fctr := %g\n",
    h->plane_eff_corr_fctr, h->decay_corr_fctr, h->loss_corr_fctr);
  fprintf(fp, "quant_units := %d (%s)\n", h->quant_units, ecat63Unit(h->quant_units));
  fprintf(fp, "ecat_calibration_fctr := %g\nwell_counter_cal_fctr := %g\n",
    h->ecat_calibration_fctr, h->well_counter_cal_fctr);
  fprintf(fp, "annotation := %.40s\n", h->annotation);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Print ECAT 6.3 scanheader contents to specified file pointer
 *
 * @param h Ecat 6.3 scan header
 * @param fp target file pointer
 */
void ecat63PrintScanheader(ECAT63_scanheader *h, FILE *fp) {
  int i;

  if(ECAT63_TEST) printf("ecat63PrintScanheader()\n");
  fprintf(fp, "data_type := %d (%s)\n", h->data_type, ecat63Datatype(h->data_type));
  fprintf(fp, "dimension_1 := %d\n", h->dimension_1);
  fprintf(fp, "dimension_2 := %d\n", h->dimension_2);
  fprintf(fp, "sample_distance := %g cm\n", h->sample_distance);
  fprintf(fp, "isotope_halflife := %g sec\n", h->isotope_halflife);
  fprintf(fp, "gate_duration := %d\nr_wave_offset := %d\n",
    h->gate_duration, h->r_wave_offset);
  fprintf(fp, "scale_factor := %g\n", h->scale_factor);
  fprintf(fp, "scan_min := %d\nscan_max := %d\n", h->scan_min, h->scan_max);
  fprintf(fp, "prompts := %d\ndelayed := %d\nmultiples := %d\nnet_trues := %d\n",
    h->prompts, h->delayed, h->multiples, h->net_trues);
  fprintf(fp, "cor_singles :=");
  for(i=0; i<16; i++) fprintf(fp, " %8.0f", h->cor_singles[i]); printf("\n");
  fprintf(fp, "uncor_singles :=");
  for(i=0; i<16; i++) fprintf(fp, " %8.0f", h->uncor_singles[i]); printf("\n");
  fprintf(fp, "tot_avg_cor := %g\ntot_avg_uncor := %g\n", h->tot_avg_cor, h->tot_avg_uncor);
  fprintf(fp, "total_coin_rate := %d\n", h->total_coin_rate);
  fprintf(fp, "frame_start_time := %d\nframe_duration := %d\n",
    h->frame_start_time, h->frame_duration);
  fprintf(fp, "loss_correction_fctr := %g\n", h->loss_correction_fctr);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Print ECAT 6.3 attnheader contents to specified file pointer
 *
 * @param h Ecat 6.3 attenuation header
 * @param fp target file pointer
 */
void ecat63PrintAttnheader(ECAT63_attnheader *h,  FILE *fp) {
  if(ECAT63_TEST) printf("ecat63PrintAttnheader()\n");
  fprintf(fp, "data_type := %d (%s)\n", h->data_type, ecat63Datatype(h->data_type));
  fprintf(fp, "dimension_1 := %d\n", h->dimension_1);
  fprintf(fp, "dimension_2 := %d\n", h->dimension_2);
  fprintf(fp, "sample_distance := %g cm\n", h->sample_distance);
  fprintf(fp, "attenuation_type := %d\n", h->attenuation_type);
  fprintf(fp, "scale_factor := %g\n", h->scale_factor);
  fprintf(fp, "x_origin := %g\ny_origin := %g\nx_radius := %g\ny_radius := %g\n",
    h->x_origin, h->y_origin, h->x_radius, h->y_radius);
  fprintf(fp, "tilt_angle := %g\nattenuation_coeff := %g\n",
    h->tilt_angle, h->attenuation_coeff);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Print ECAT 6.3 normheader contents to specified file pointer.
 *
 * @param h Ecat 6.3 normalization header
 * @param fp target file pointer
 */
void ecat63PrintNormheader(ECAT63_normheader *h, FILE *fp) {
  if(ECAT63_TEST) printf("ecat63PrintNormheader()\n");
  fprintf(fp, "data_type := %d (%s)\n", h->data_type, ecat63Datatype(h->data_type));
  fprintf(fp, "dimension_1 := %d\n", h->dimension_1);
  fprintf(fp, "dimension_2 := %d\n", h->dimension_2);
  fprintf(fp, "scale_factor := %g\n", h->scale_factor);
  fprintf(fp, "norm time := %02d.%02d.%04d %02d:%02d:%02d\n",
    h->norm_day, h->norm_month, h->norm_year,
    h->norm_hour, h->norm_minute, h->norm_second);
}
/*****************************************************************************/

/*****************************************************************************/
/*!
 * Return pointer to string describing the ECAT 6.3 data_type
 *
 * @param dtype data type code
 * @return pointer to static string
 */
char *ecat63Datatype(short int dtype) {
  static char *ecat63_datatype[]={
  /*  0 */  "Unknown",
  /*  1 */  "BYTE_TYPE",
  /*  2 */  "VAX_I2",
  /*  3 */  "VAX_I4",
  /*  4 */  "VAX_R4",
  /*  5 */  "IEEE_R4",
  /*  6 */  "SUN_I2",
  /*  7 */  "SUN_I4",
  /*  8 */  "Unknown",
  /*  9 */  "Unknown"
  };
  if(dtype>=0 && dtype<10) return(ecat63_datatype[dtype]);
  else return(ecat63_datatype[0]);
}

/*!
 * Returns pointer to string describing the calibrated data unit (ECAT 6.3).
 *
 * @param dunit data unit code
 * @return pointer to static string
 */
char *ecat63Unit(short int dunit) {
  static char *ecat63_unit[]={
  /*  0 */  "Unknown",
  /*  1 */  "Unknown",
  /*  2 */  "ECAT counts",
  /*  3 */  "uCi/ml",
  /*  4 */  "LMRGlu",
  /*  5 */  "LMRUGlu umol/min/100g",
  /*  6 */  "LMRUGlu mg/min/100g",
  /*  7 */  "nCi/mL",
  /*  8 */  "Well counts",
  /*  9 */  "Becquerels",
  /* 10 */  "kBq/mL",
  /* 11 */  "1/min",
  /* 12 */  "mL/min/100g",
  /* 13 */  "sec*kBq/mL",
  /* 14 */  "sec*nCi/mL",
  /* 15 */  "1/sec",
  /* 16 */  "Unitless",
  /* 17 */  "Unknown"
  };
  if(dunit>=0 && dunit<18) return(ecat63_unit[dunit]);
  else return(ecat63_unit[0]);
}
/*****************************************************************************/
/*!
 *  Printfs separately the sign, mantissa, and exp part of a 32-bit float,
 *  which is pointed to by the argument.
 *  Code is not optimized; do not use this in routine operations!
 *
 * @param buf printed float
 */
void float2parts(float *buf) {
  unsigned int u, exp, mantissa;
  char sign;

  memcpy(&u, buf, 4); if(u & 1L<<31) sign='-'; else sign='+';
  exp=u<<1; exp=exp>>24; mantissa=u<<9; mantissa=mantissa>>9;
  printf("%e = %c (%u/8388608 + 1)*2^(%u-127)\n", *buf, sign, mantissa, exp);
}
/*****************************************************************************/

/*****************************************************************************/
/** Print ECAT63 subheader contents into specified file pointer.
\return Returns 0 when successful.
 */
int ecat6PrintSubheader(
  /** ECAT 6.3 mainheader (not printed but needed here) */
  ECAT63_mainheader mh,
  /** File pointer to ECAT 6.3 file */
  FILE *fp,
  /** ECAT 6.3 plane */
  int plane,
  /** ECAT 6.3 frame */
  int frame,
  /** Output is written to this file pointer; it can be stdout */
  FILE *ofp
) {
  int                 mi, ret, nr=0;
  static MATRIXLIST   mlist;
  ECAT63_imageheader  image_header;
  ECAT63_scanheader   scan_header;
  ECAT63_attnheader   attn_header;
  ECAT63_normheader   norm_header;
  Matval              matval;


  /*  Read matrix list and nr */
  ecat63InitMatlist(&mlist);
  ret=ecat63ReadMatlist(fp, &mlist);
  if(ret) {
    fprintf(stderr, "Error (%d): cannot read matrix list.\n", ret);
    return 2;
  }
  if(mlist.matrixNr<=0) {
    fprintf(stderr, "Error: matrix list is empty.\n");
    return 2;
  }
  if(ECAT63_TEST>1) ecat63PrintMatlist(&mlist);

  /*
   *  Read and print subheaders one at a time
   */
  for(mi=nr=0; mi<mlist.matrixNr; mi++) {
    /* Get plane and frame nr */
    mat_numdoc(mlist.matdir[mi].matnum, &matval);
    /* Check if this is supposed to be listed or not */
    if(frame>=0 && frame!=matval.frame) continue;
    if(plane>=0 && plane!=matval.plane) continue;
    /* Read subheader */
    if(mh.file_type==IMAGE_DATA)
      ret=ecat63ReadImageheader(fp, mlist.matdir[mi].strtblk, &image_header);
    else if(mh.file_type==RAW_DATA)
      ret=ecat63ReadScanheader(fp, mlist.matdir[mi].strtblk, &scan_header);
    else if(mh.file_type==ATTN_DATA)
      ret=ecat63ReadAttnheader(fp, mlist.matdir[mi].strtblk, &attn_header);
    else if(mh.file_type==NORM_DATA)
      ret=ecat63ReadNormheader(fp, mlist.matdir[mi].strtblk, &norm_header);
    if(ret) {
      fprintf(stderr, "Error: cannot read matrix %u subheader.\n",
         mlist.matdir[mi].matnum);
      ecat63EmptyMatlist(&mlist); return 4;
    }
    /* Print subheader */
    fprintf(fp, "Matrix: plane %d frame %d gate %d bed %d\n",
      matval.plane, matval.frame, matval.gate, matval.bed);
    if(mh.file_type==IMAGE_DATA)
      ecat63PrintImageheader(&image_header, ofp);
    else if(mh.file_type==RAW_DATA)
      ecat63PrintScanheader(&scan_header, ofp);
    else if(mh.file_type==ATTN_DATA)
      ecat63PrintAttnheader(&attn_header, ofp);
    else if(mh.file_type==NORM_DATA)
      ecat63PrintNormheader(&norm_header, ofp);
    nr++; // counter
  } /* next matrix */
  ecat63EmptyMatlist(&mlist);
  
  if(nr==0 && (plane>=0 || frame>=0)) {
    fprintf(stderr, "Error: specified matrices not found.\n");
    return(11);
  }

  return(0);
}
/*****************************************************************************/

/*****************************************************************************/

