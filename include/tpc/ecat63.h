/******************************************************************************

  Copyright (c) 2003-2008 Turku PET Centre

  ecat63.h

  Date:
  1996-10-30 VO
  2002-01-20 VO
  2002-02-01 VO
  2002-02-12 VO
  2002-08-23 VO
  2003-07-21 VO
  2003-08-05 VO
  2003-09-08 VO
  2004-06-27 VO
  2004-12-28 VO
  2007-02-27 VO
  2007-03-13 VO
  2008-07-24 VO


******************************************************************************/
#ifndef _ECAT63_H_
#define _ECAT63_H_
/*****************************************************************************/
#define MatBLKSIZE 512
#define MatFirstDirBlk 2
/* Data types */
#define	BYTE_TYPE   1
#define	VAX_I2      2
#define VAX_I4      3
#define VAX_R4      4
#define IEEE_R4     5
#define	SUN_I2      6
#define	SUN_I4      7
/* File types */
#define	RAW_DATA    1
#define	IMAGE_DATA  2
#define	ATTN_DATA   3
#define	NORM_DATA   4
/*****************************************************************************/
#define ECAT63_SYSTEM_TYPE_DEFAULT 931
/*****************************************************************************/
/* Backup file extension */
#ifndef BACKUP_EXTENSION
#define BACKUP_EXTENSION ".bak"
#endif 
/*****************************************************************************/
extern char ecat63errmsg[];
/*****************************************************************************/
extern int ECAT63_TEST;
/*****************************************************************************/

typedef struct {
  int matnum;
  int strtblk;
  int endblk;
  int matstat;
} MatDir;

typedef struct {
  int matrixNr;
  int matrixSpace;
  MatDir *matdir;
} MATRIXLIST;

typedef struct {
  int frame, plane, gate, data, bed;
} Matval;

typedef struct ecat63_mainheader {
  char      ecat_format[14], fill1[14];
  char      original_file_name[20];
  short int sw_version;
  short int data_type;
  short int system_type;
  short int file_type;
  char      node_id[10];
  short int scan_start_day, scan_start_month, scan_start_year,
            scan_start_hour, scan_start_minute, scan_start_second;
  char      isotope_code[8];
  float     isotope_halflife;
  char      radiopharmaceutical[32];
  float     gantry_tilt, gantry_rotation, bed_elevation;
  short int rot_source_speed, wobble_speed, transm_source_type;
  float     axial_fov, transaxial_fov;
  short int transaxial_samp_mode, coin_samp_mode, axial_samp_mode;
  float     calibration_factor;
  short int calibration_units, compression_code;
  char      study_name[12], patient_id[16], patient_name[32], patient_sex,
            patient_age[10], patient_height[10], patient_weight[10],
            patient_dexterity, physician_name[32], operator_name[32],
            study_description[32];
  short int acquisition_type, bed_type, septa_type;
  char      facility_name[20];
  short int num_planes, num_frames, num_gates, num_bed_pos;
  float     init_bed_position, bed_offset[15], plane_separation;
  short int lwr_sctr_thres, lwr_true_thres, upr_true_thres;
  float     collimator;
  char      user_process_code[10];
  short int fill2[20];
} ECAT63_mainheader;

typedef struct ecat63_imageheader {
  char      fill1[126];
  short int data_type, num_dimensions, dimension_1, dimension_2;
  float     x_origin, y_origin, recon_scale, quant_scale;
  short int image_min, image_max;
  float     pixel_size, slice_width;
  int       frame_duration, frame_start_time;
  short int slice_location,
            recon_start_hour, recon_start_min, recon_start_sec;
  int       recon_duration;
  short int filter_code;
  int       scan_matrix_num, norm_matrix_num, atten_cor_mat_num;
  float     image_rotation, plane_eff_corr_fctr,
            decay_corr_fctr, loss_corr_fctr, intrinsic_tilt;
  short int processing_code, quant_units,
            recon_start_day, recon_start_month, recon_start_year;
  float     ecat_calibration_fctr, well_counter_cal_fctr, filter_params[6];
  char      annotation[40];
  short int fill2[26];
} ECAT63_imageheader;

typedef struct ecat63_scanheader {
  char      fill1[126];
  short int data_type,
            dimension_1, dimension_2,
            smoothing, processing_code;
  float     sample_distance,
            isotope_halflife;
  short int frame_duration_sec;
  int       gate_duration, r_wave_offset;
  float     scale_factor;
  short int scan_min, scan_max;
  int       prompts, delayed, multiples, net_trues;
  float     cor_singles[16], uncor_singles[16],
            tot_avg_cor, tot_avg_uncor;
  int       total_coin_rate,
            frame_start_time, frame_duration;
  float     loss_correction_fctr;
  short int fill2[22];
} ECAT63_scanheader;

typedef struct ecat63_normheader {
  short int data_type,
            dimension_1, dimension_2;
  float     scale_factor;
  short int norm_hour, norm_minute, norm_second, norm_day, norm_month, norm_year;
  float     fov_source_width;
} ECAT63_normheader;

typedef struct ecat63_attnheader {
  short int data_type, attenuation_type,
            dimension_1, dimension_2;
  float     scale_factor,
            x_origin, y_origin, x_radius, y_radius, tilt_angle,
            attenuation_coeff,
            sample_distance;
} ECAT63_attnheader;

typedef struct matdirnode {
  int    matnum;
  int    strtblk;
  int    endblk;
  int    matstat;
  struct matdirnode *next;
} MatDirNode ;

typedef struct matdirlist {
  int         nmats;
  MatDirNode *first;
  MatDirNode *last;
} MatDirList;

typedef struct matrixdata {
  int    mat_type;
  char  *shptr;
  char  *data_ptr;
  int    nviews;
  int    nelements;
  int    nblks;
  int    data_type;
} MatrixData ;

typedef struct matrix_file {
  ECAT63_mainheader *mhptr;
  MatDirList *dirlist;
  FILE *fptr ;
} Matrix_file;
/*****************************************************************************/
/* Read functions */
extern int ecat63ReadMainheader(FILE *fp, ECAT63_mainheader *h);
extern int ecat63ReadImageheader(FILE *fp, int blk, ECAT63_imageheader *h);
extern int ecat63ReadScanheader(FILE *fp, int blk, ECAT63_scanheader *h);
extern int ecat63ReadAttnheader(FILE *fp, int blk, ECAT63_attnheader *h);
extern int ecat63ReadNormheader(FILE *fp, int blk, ECAT63_normheader *h);
extern int ecat63ReadMatdata(FILE *fp, int strtblk, int blkNr, char *data, int dtype);
extern int ecat63ReadImageMatrix(FILE *fp, int strtblk, int lastblk, ECAT63_imageheader *h, float **f);
extern int ecat63ReadScanMatrix(FILE *fp, int strtblk, int lastblk, ECAT63_scanheader *h, float **f);
extern float ecat63rFloat(void *bufi, int isvax, int islittle);
extern int ecat63rInt(void *bufi, int isvax, int islittle);
extern int ecat63pxlbytes(short int data_type);
/*****************************************************************************/
/* Matrix list functions */
extern void ecat63InitMatlist(MATRIXLIST *mlist);
extern void ecat63EmptyMatlist(MATRIXLIST *mlist);
extern int ecat63ReadMatlist(FILE *fp, MATRIXLIST *ml);
extern void ecat63PrintMatlist(MATRIXLIST *ml);
extern int mat_numcod(int frame, int plane, int gate, int data, int bed);
extern void mat_numdoc(int matnum, Matval *matval);
extern int ecat63Matenter(FILE *fp, int matnum, int blkNr);
extern void ecat63SortMatlistByPlane(MATRIXLIST *ml);
extern void ecat63SortMatlistByFrame(MATRIXLIST *ml);
extern int ecat63CheckMatlist(MATRIXLIST *ml);
extern int ecat63DeleteLateFrames(MATRIXLIST *ml, int frame_nr);
extern int ecat63GetMatrixBlockSize(MATRIXLIST *mlist, int *blk_nr);
extern int ecat63GetPlaneAndFrameNr(MATRIXLIST *mlist, ECAT63_mainheader *h, int *plane_nr, int *frame_nr);
extern int ecat63GetNums(MATRIXLIST *ml, short int *num_planes, short int *num_frames, short int *num_gates, short int *num_bed_pos);
extern int ecat63GatherMatlist(MATRIXLIST *ml, short int do_planes, short int do_frames, short int do_gates, short int do_beds);
/*****************************************************************************/
/* Write functions */
extern int ecat63WriteMainheader(FILE *fp, ECAT63_mainheader *h);
extern int ecat63WriteImageheader(FILE *fp, int block, ECAT63_imageheader *h);
extern int ecat63WriteScanheader(FILE *fp, int block, ECAT63_scanheader *h);
extern int ecat63WriteAttnheader(FILE *fp, int block, ECAT63_attnheader *h);
extern int ecat63WriteNormheader(FILE *fp, int block, ECAT63_normheader *h);
extern FILE *ecat63Create(const char *fname, ECAT63_mainheader *h);
extern int ecat63WriteMatdata(FILE *fp, int strtblk, char *data, int pxlNr, int pxlSize);
extern int ecat63WriteScan(FILE *fp, int matnum, ECAT63_scanheader *h, void *data);
extern int ecat63WriteImage(FILE *fp, int matnum, ECAT63_imageheader *h, void *data);
extern int ecat63WriteNorm(FILE *fp, int matnum, ECAT63_normheader *h, void *data);
extern int ecat63WriteAttn(FILE *fp, int matnum, ECAT63_attnheader *h, void *data);
extern int ecat63WriteImageMatrix(FILE *fp, int matnum, ECAT63_imageheader *h, float *fdata);
extern int ecat63WriteScanMatrix(FILE *fp, int matnum, ECAT63_scanheader *h, float *fdata);
extern void ecat63wFloat(float *bufi, void *bufo, int tovax, int islittle);
extern void ecat63wInt(int *bufi, void *bufo, int tovax, int islittle);
extern int ecat63_is_scaling_needed(float amax, float *data, int nr);
/*****************************************************************************/
/* Printing functions */
extern void ecat63PrintMainheader(ECAT63_mainheader *h, FILE *fp);
extern void ecat63PrintImageheader(ECAT63_imageheader *h, FILE *fp);
extern void ecat63PrintScanheader(ECAT63_scanheader *h, FILE *fp);
extern void ecat63PrintAttnheader(ECAT63_attnheader *h, FILE *fp);
extern void ecat63PrintNormheader(ECAT63_normheader *h, FILE *fp);
extern int ecat6PrintSubheader(ECAT63_mainheader mh, FILE *fp,
  int plane, int frame, FILE *ofp);
extern char *ecat63Datatype(short int dtype);
extern char *ecat63Unit(short int dunit);
extern void float2parts(float *buf);
/*****************************************************************************/
#endif

