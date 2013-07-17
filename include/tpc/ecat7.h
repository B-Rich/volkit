/******************************************************************************

  ecat7.h  (c) 2003-2010,2012 Turku PET Centre

  Date:
  2003-07-26 Vesa Oikonen
  2003-08-03 VO
    Included definitions for patient orientation.
  2003-09-04 VO
    Introduced functions for 3D scan r/w.
  2003-10-08 VO
    ECAT7_MAGICNR changed from MATRIX7 to MATRIX72v
  2004-02-07 VO
    ECAT7_MAGICNR replaced by separate defines for image volumes and sinograms:
    ECAT7V_MAGICNR and ECAT7S_MAGICNR.
    Included define for sw_version = 72.
  2004-05-23 VO
    Introduced new function for ecat7p.c.
    Added a few comments.
  2004-06-27 VO
    Introduced new function for ecat7ml.c.
  2004-07-26 VO
    Comment style changes.
  2004-09-20 VO
    Added empty comments.
  2004-09-24 VO
    Added comments.
  2004-12-28 VO
    Introduced new function ecat7_is_scaling_needed().
  2007-02-27 VO
    Introduced new functions.
  2007-03-13 VO
    Introduced new functions.
  2007-03-27 VO
    Introduced new functions.
  2008-07-24 VO
    Introduced new functions.
  2010-08-19 VO
    Changed comment, not affecting compiled code.
  2012-02-04 VO
    Changed comment, not affecting compiled code.


******************************************************************************/
#ifndef _ECAT7_H_
#define _ECAT7_H_

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
#ifndef MatBLKSIZE
#define MatBLKSIZE 512
#endif
#ifndef MatFirstDirBlk
#define MatFirstDirBlk 2
#endif
/*****************************************************************************/
#define ECAT7V_MAGICNR "MATRIX72v"
#define ECAT7S_MAGICNR "MATRIX7011"
#define ECAT7_SW_VERSION 72
/*****************************************************************************/
/** Matrix data types */
#define	ECAT7_BYTE      1
#define	ECAT7_VAXI2     2
#define ECAT7_VAXI4     3
#define ECAT7_VAXR4     4
#define ECAT7_IEEER4    5
#define	ECAT7_SUNI2     6
#define	ECAT7_SUNI4     7
/*****************************************************************************/
/** Matrix filetypes */
#define ECAT7_UNKNOWN   0
#define ECAT7_2DSCAN    1
#define ECAT7_IMAGE16   2
#define ECAT7_ATTEN     3
#define ECAT7_2DNORM    4
#define ECAT7_POLARMAP  5
#define ECAT7_VOLUME8   6
#define ECAT7_VOLUME16  7
#define ECAT7_PROJ      8
#define ECAT7_PROJ16    9
#define ECAT7_IMAGE8    10
#define ECAT7_3DSCAN    11
#define ECAT7_3DSCAN8   12
#define ECAT7_3DNORM    13
#define ECAT7_3DSCANFIT 14
/*****************************************************************************/
/** Patient orientation */
#define ECAT7_Feet_First_Prone           0
#define ECAT7_Head_First_Prone           1
#define ECAT7_Feet_First_Supine          2
#define ECAT7_Head_First_Supine          3
#define ECAT7_Feet_First_Decubitus_Right 4
#define ECAT7_Head_First_Decubitus_Right 5
#define ECAT7_Feet_First_Decubitus_Left  6
#define ECAT7_Head_First_Decubitus_Left  7
#define ECAT7_Unknown_Orientation        8
/*****************************************************************************/
/* Backup file extension */
#ifndef BACKUP_EXTENSION
#define BACKUP_EXTENSION ".bak"
#endif 
/*****************************************************************************/
extern char ecat7errmsg[];
/*****************************************************************************/
extern int ECAT7_TEST;
/*****************************************************************************/
typedef struct ecat7_mainheader {  /* 512 bytes */
  /** Unix file type indentification number */
  char      magic_number[14];
  /** Scan file's creation number */
  char      original_file_name[32];
  /** */
  short int sw_version;
  /** Scanner model */
  short int system_type;
  /** Matrix file type */
  short int file_type;
  /** Serial number of the gantry */
  char      serial_number[10];
  /** Date and time when acquisition was started (sec from base time) */
  int       scan_start_time;
  /** String representation of the isotope */
  char      isotope_name[8];
  /** Half-life of isotope (sec) */
  float     isotope_halflife;
  /** String representation of the tracer name */
  char      radiopharmaceutical[32];
  /** Angle (degrees) */
  float     gantry_tilt;
  /** Angle (degrees) */
  float     gantry_rotation;
  /** Bed height from lowest point (cm) */
  float     bed_elevation;
  /** */
  float     intrinsic_tilt;
  /** */
  short int wobble_speed;
  /** */
  short int transm_source_type;
  /** Total distance scanned (cm) */
  float     distance_scanned;
  /** Diameter of transaxial view (cm) */
  float     transaxial_fov;
  /** 0=no mash, 1=mash of 2, 2=mash of 4 */
  short int angular_compression;
  /** 0=Net trues, 1=Prompts and Delayed, 3=Prompts, Delayed, and Multiples */
  short int coin_samp_mode;
  /** 0=Normal, 1=2X, 2=3X */
  short int axial_samp_mode;
  float     ecat_calibration_factor;
  /** 0=Uncalibrated; 1=Calibrated; 2=Processed */
  short int calibration_units;
  /** Whether data_units[] is filled or not? */
  short int calibration_units_label;
  /** */
  short int compression_code;
  /** */
  char      study_type[12];
  /** */
  char      patient_id[16];
  /** */
  char      patient_name[32];
  /** */
  char      patient_sex;
  /** */
  char      patient_dexterity;
  /** Patient age (years) */
  float     patient_age;
  /** Patient height (cm) */
  float     patient_height;
  /** Patient weight (kg) */
  float     patient_weight;
  /** YYYYMMDD. In HR+ files this field may contain birth date as seconds from
   *  time zero, thus negative number when born before 1970, but those are
   *  converted to YYYYMMDD when file is read */
  int       patient_birth_date;
  /** */
  char      physician_name[32];
  /** */
  char      operator_name[32];
  /** */
  char      study_description[32];
  /** 0=Undefined; 1=Blank; 2=Transmission; 3=Static emission; 4=Dynamic emission;
      5=Gated emission; 6=Transmission rectilinear; 7=Emission rectilinear */
  short int acquisition_type;
  /** */
  short int patient_orientation;
  /** */
  char      facility_name[20];
  /** */
  short int num_planes;
  /** Highest frame number in partially reconstruction files */
  short int num_frames;
  /** */
  short int num_gates;
  /** */
  short int num_bed_pos;
  /** */
  float     init_bed_position;
  /** */
  float     bed_position[15];
  /** Physical distance between adjacent planes (cm) */
  float     plane_separation;
  /** */
  short int lwr_sctr_thres;
  /** */
  short int lwr_true_thres;
  /** */
  short int upr_true_thres;
  /** */
  char      user_process_code[10];
  /** */
  short int acquisition_mode;
  /** Width of view sample (cm) */
  float     bin_size;
  /** Fraction of decay by positron emission (included in calibration factor
   *  in TPC) */
  float     branching_fraction;
  /** Time of injection */
  int       dose_start_time;
  /** Radiopharmaceutical dosage at time of injection (Bq/cc) */
  float     dosage;
  /** */
  float     well_counter_corr_factor;
  /** Free text field; fixed strings: "ECAT counts/sec", "Bq/cc" */
  char      data_units[32];
  /** */
  short int septa_state;
  /** */
  short int fill_cti[6];   
} ECAT7_mainheader;
/*****************************************************************************/
typedef struct ecat7_imageheader {  /* 512 bytes */
  /** */
  short int data_type;
  /** */
  short int num_dimensions;
  /** */
  short int x_dimension;
  /** */
  short int y_dimension;
  /** */
  short int z_dimension;
  /** cm */
  float     x_offset;
  /** cm */
  float     y_offset;
  /** cm */
  float     z_offset;
  /** Reconstruction magnification factor */
  float     recon_zoom;
  /** */
  float     scale_factor;
  /** */
  short int image_min;
  /** */
  short int image_max;
  /** X dimension pixel size (cm) */
  float     x_pixel_size;
  /** Y dimension pixel size (cm) */
  float     y_pixel_size;
  /** Z dimension pixel size (cm) */
  float     z_pixel_size;
  /** msec */
  int       frame_duration;
  /** Offset from first frame (msec) */
  int       frame_start_time;
  /** */
  short int filter_code;
  /** cm */
  float     x_resolution;
  /** cm */
  float     y_resolution;
  /** cm */
  float     z_resolution;
  /** Number R elements from sinogram */
  float     num_r_elements;
  /** Nr of angles from sinogram */
  float     num_angles;
  /** Rotation in the xy plane (degrees) */
  float     z_rotation_angle;
  /** */
  float     decay_corr_fctr;
  /** */
  int       processing_code;
  /** */
  int       gate_duration;
  /** */
  int       r_wave_offset;
  /** */
  int       num_accepted_beats;
  /** */
  float     filter_cutoff_frequency;
  /** */
  float     filter_resolution;
  /** */
  float     filter_ramp_slope;
  /** */
  short int filter_order;
  /** */
  float     filter_scatter_fraction;
  /** */
  float     filter_scatter_slope;
  /** */
  char      annotation[40];
  /** */
  float     mt_1_1;
  /** */
  float     mt_1_2;
  /** */
  float     mt_1_3;
  /** */
  float     mt_2_1;
  /** */
  float     mt_2_2;
  /** */
  float     mt_2_3;
  /** */
  float     mt_3_1;
  /** */
  float     mt_3_2;
  /** */
  float     mt_3_3;
  /** */
  float     rfilter_cutoff;
  /** */
  float     rfilter_resolution;
  /** */
  short int rfilter_code;
  /** */
  short int rfilter_order;
  /** */
  float     zfilter_cutoff;
  /** */
  float     zfilter_resolution;
  /** */
  short int zfilter_code;
  /** */
  short int zfilter_order;
  /** */
  float     mt_1_4;
  /** */
  float     mt_2_4;
  /** */
  float     mt_3_4;
  /** */
  short int scatter_type;
  /** */
  short int recon_type;
  /** */
  short int recon_views;
  /** */
  short int fill_cti[87];
  /** */
  short int fill_user[49];
} ECAT7_imageheader;
/*****************************************************************************/
typedef struct ecat7_scanheader { /* 1024 bytes */
  /** */
  short int data_type;
  /** */
  short int num_dimensions;
  /** Total elemenst collected (r dimension ) */
  short int num_r_elements;
  /** Total views collected (theta dimension) */
  short int num_angles;
  /** Bit 0 - Norm; Bit 1 - Atten; Bit 2 - Smooth */
  short int corrections_applied;
  /** Nr of elements in z dimension for each ring difference segment */
  short int num_z_elements[64];
  /** Max ring difference (d dimension) in this frame */
  short int ring_difference;
  /** RThetaZD or RZThetaD */
  short int storage_order;
  /** Span */
  short int axial_compression;
  /** Resolution in r dimension (cm) */
  float     x_resolution;
  /** Resolution in Theta dimension (rad) */
  float     v_resolution;
  /** Resolution in z dimension (cm) */
  float     z_resolution;
  /** */
  float     w_resolution;
  /** */
  short int fill_gate[6];
  /** */
  int       gate_duration;
  /** Time from start of first gate (msec) */
  int       r_wave_offset;
  /** */
  int       num_accepted_beats;
  /** */
  float     scale_factor;
  /** */
  short int scan_min;
  /** */
  short int scan_max;
  /** */
  int       prompts;
  /** */
  int       delayed;
  /** */
  int       multiples;
  /** */
  int       net_trues;
  /** */
  float     tot_avg_cor;
  /** */
  float     tot_avg_uncor;
  /** */
  int       total_coin_rate;
  /** Time offset from first frame (msec) */
  int       frame_start_time;
  /** Total duration of current frame (msec) */
  int       frame_duration;
  /** */
  float     deadtime_correction_factor;
  /** */
  short int fill_cti[90];
  /** */
  short int fill_user[50];
  /** */
  float     uncor_singles[128];
} ECAT7_scanheader;
/*****************************************************************************/
typedef struct ecat7_2Dscanheader { /* 512 bytes */
  short int data_type;
  short int num_dimensions;
  short int num_r_elements;
  short int num_angles;
  short int corrections_applied;
  short int num_z_elements;
  short int ring_difference;
  float     x_resolution;
  float     y_resolution;
  float     z_resolution;
  float     w_resolution;
  short int fill_gate[6];
  int       gate_duration;
  int       r_wave_offset;
  int       num_accepted_beats;
  float     scale_factor;
  short int scan_min;
  short int scan_max;
  int       prompts;
  int       delayed;
  int       multiples;
  int       net_trues;
  float     cor_singles[16];
  float     uncor_singles[16];
  float     tot_avg_cor;
  float     tot_avg_uncor;
  int       total_coin_rate;
  int       frame_start_time;
  int       frame_duration;
  float     deadtime_correction_factor;
  short int physical_planes[8];
  short int fill_cti[83];
  short int fill_user[50];
} ECAT7_2Dscanheader;
/*****************************************************************************/
typedef struct ecat7_2Dnormheader {
  short int data_type;
  short int num_dimensions;
  short int num_r_elements;
  short int num_angles;
  short int num_z_elements;
  short int ring_difference;
  float     scale_factor;
  float     norm_min;
  float     norm_max;
  float     fov_source_width;
  float     norm_quality_factor;
  short int norm_quality_factor_code;
  short int storage_order;
  short int span;
  short int z_elements[64];
  short int fill_cti[123];
  short int fill_user[50];
} ECAT7_2Dnormheader;
/*****************************************************************************/
typedef struct ecat7_attenheader {
  /** */
  short int data_type;
  /** */
  short int num_dimensions;
  /** */
  short int attenuation_type;
  /** Total elements collected (x dim) */
  short int num_r_elements;
  /** Total views collected (y dim) */
  short int num_angles;
  /** Total elements collected (z dim) */
  short int num_z_elements;
  /** Max acceptance angle */
  short int ring_difference;
  /** Resolution in the x dimension (cm) */
  float     x_resolution;
  /** Resolution in the y dimension (cm) */
  float     y_resolution;
  /** Resolution in the z dimension (cm) */
  float     z_resolution;
  /** TBD */
  float     w_resolution;
  /** Attenuation scale factor */
  float     scale_factor;
  /** Ellipse offset in x axis from center (cm) */
  float     x_offset;
  /** Ellipse offset in y axis from center (cm) */
  float     y_offset;
  /** Ellipse radius in x axis (cm) */
  float     x_radius;
  /** Ellipse radius in y axis (cm) */
  float     y_radius;
  /** Tilt angle of the ellipse (degrees) */
  float     tilt_angle;
  /** Mu-absorption coefficient (cm-1) */
  float     attenuation_coeff;
  /** */
  float     attenuation_min;
  /** */
  float     attenuation_max;
  /** */
  float     skull_thickness;
  /** */
  short int num_additional_atten_coeff;
  /** */
  float     additional_atten_coeff[8];
  /** */
  float     edge_finding_threshold;
  /** Data storage order (RThetaZD, RZThetaD) */
  short int storage_order;
  /** Axial compression specifier (nr of ring differences spanned by a segment) */
  short int span;
  /** Nr of 'planes' in z direction for each ring difference segment */
  short int z_elements[64];
  /** */
  short int fill_cti[86];
  /** */
  short int fill_user[50];
} ECAT7_attenheader;
/*****************************************************************************/
typedef struct ecat7_normheader {
  short int data_type;
  /** Total elemenst collected (y dimension) */
  short int num_r_elements;
  /** Transaxial crystals per block */
  short int num_transaxial_crystals;
  /** Nr of crystal rings */
  short int num_crystal_rings;
  short int crystals_per_ring;
  /** Nr of rows in plane geometric correction array */
  short int num_geo_corr_planes;
  /** Upper energy limit */
  short int uld;
  /** Lower energy limit */
  short int lld;
  /** Scatter energy threshold */
  short int scatter_energy;
  /** */
  float     norm_quality_factor;
  /** */
  short int norm_quality_factor_code;
  /** */
  float     ring_dtcor1[32];
  /** */
  float     ring_dtcor2[32];
  /** */
  float     crystal_dtcor[8];
  /** Nr of ring differences included in each segment */
  short int span;
  /** Maximum ring difference acquired */
  short int max_ring_diff;
  /** */
  short int fill_cti[48];
  /** */
  short int fill_user[50];
} ECAT7_normheader;
/*****************************************************************************/
typedef struct ecat7_polmapheader {
  short int data_type;
  short int polar_map_type;         
  short int num_rings;
  short int sectors_per_ring[32];   
  float     ring_position[32];           
  short int ring_angle[32];      
  short int start_angle;   
  short int long_axis_left[3];         
  short int long_axis_right[3];   
  short int position_data;     
  short int image_min; 
  short int image_max;
  float     scale_factor;
  float     pixel_size;   
  int       frame_duration;
  int       frame_start_time;        
  short int processing_code;          
  short int quant_units;
  char      annotation[40];
  int       gate_duration;
  int       r_wave_offset;
  int       num_accepted_beats;
  char      polar_map_protocol[20];
  char      database_name[30];   
  short int fill_cti[27];       
  short int fill_user[27];
} ECAT7_polmapheader;
/*****************************************************************************/
typedef struct {
  int id;
  int strtblk;
  int endblk;
  int status;
} ECAT7_MatDir;
typedef struct {
  int matrixNr;
  int matrixSpace;
  ECAT7_MatDir *matdir;
} ECAT7_MATRIXLIST;
typedef struct {
  int frame, plane, gate, data, bed;
} ECAT7_Matval;
/*****************************************************************************/
/* Read functions */
extern int ecat7ReadMainheader(FILE *fp, ECAT7_mainheader *h);
extern int ecat7ReadImageheader(FILE *fp, int blk, ECAT7_imageheader *h);
extern int ecat7ReadAttenheader(FILE *fp, int blk, ECAT7_attenheader *h);
extern int ecat7ReadPolmapheader(FILE *fp, int blk, ECAT7_polmapheader *h);
extern int ecat7ReadNormheader(FILE *fp, int blk, ECAT7_normheader *h);
extern int ecat7ReadScanheader(FILE *fp, int blk, ECAT7_scanheader *h);
extern int ecat7Read2DScanheader(FILE *fp, int blk, ECAT7_2Dscanheader *h);
extern int ecat7Read2DNormheader(FILE *fp, int blk, ECAT7_2Dnormheader *h);
extern int ecat7ReadMatrixdata(FILE *fp, int start_block, int block_nr,
      char *data, int dtype);
extern float ecat7rFloat(void *bufi, int isvax, int islittle);
extern int ecat7rInt(void *bufi, int isvax, int islittle);
extern int ecat7ReadImageMatrix(FILE *fp, int first_block, int last_block,
      ECAT7_imageheader *h, float **fdata);
extern int ecat7Read2DScanMatrix(FILE *fp, int first_block, int last_block,
      ECAT7_2Dscanheader *h, float **fdata);
extern int ecat7ReadScanMatrix(FILE *fp, int first_block, int last_block,
      ECAT7_scanheader *h, float **fdata);
extern int ecat7ReadPolarmapMatrix(FILE *fp, int first_block, int last_block,
      ECAT7_polmapheader *h, float **fdata);
extern int ecat7pxlbytes(short int data_type);
/*****************************************************************************/
/* Matrix list functions */
extern void ecat7InitMatlist(ECAT7_MATRIXLIST *mlist);
extern void ecat7EmptyMatlist(ECAT7_MATRIXLIST *mlist);
extern int ecat7ReadMatlist(FILE *fp, ECAT7_MATRIXLIST *ml);
extern void ecat7PrintMatlist(ECAT7_MATRIXLIST *ml);
extern int ecat7EnterMatrix(FILE *fp, int matrix_id, int block_nr);
extern int ecat7_val_to_id(int frame, int plane, int gate, int data, int bed);
extern void ecat7_id_to_val(int matrix_id, ECAT7_Matval *matval);
extern void ecat7SortMatlistByPlane(ECAT7_MATRIXLIST *ml);
extern void ecat7SortMatlistByFrame(ECAT7_MATRIXLIST *ml);
extern int ecat7CheckMatlist(ECAT7_MATRIXLIST *ml);
extern int ecat7DeleteLateFrames(ECAT7_MATRIXLIST *ml, int frame_nr);
extern int ecat7GetPlaneAndFrameNr(ECAT7_MATRIXLIST *mlist, ECAT7_mainheader *h,
      int *plane_nr, int *frame_nr);
extern int ecat7GetMatrixBlockSize(ECAT7_MATRIXLIST *mlist, int *blk_nr);
extern int ecat7GetNums(ECAT7_MATRIXLIST *ml, ECAT7_mainheader *mh, FILE *fp,
      short int *num_planes, short int *num_frames, short int *num_gates,
      short int *num_bed_pos);
extern int ecat7GatherMatlist(ECAT7_MATRIXLIST *ml, short int do_planes,
      short int do_frames, short int do_gates, short int do_beds);
/*****************************************************************************/
/* Write functions */
extern int ecat7WriteMainheader(FILE *fp, ECAT7_mainheader *h);
extern int ecat7WriteImageheader(FILE *fp, int blk, ECAT7_imageheader *h);
extern int ecat7WriteAttenheader(FILE *fp, int blk, ECAT7_attenheader *h);
extern int ecat7WritePolmapheader(FILE *fp, int blk, ECAT7_polmapheader *h);
extern int ecat7WriteNormheader(FILE *fp, int blk, ECAT7_normheader *h);
extern int ecat7WriteScanheader(FILE *fp, int blk, ECAT7_scanheader *h);
extern int ecat7Write2DScanheader(FILE *fp, int blk, ECAT7_2Dscanheader *h);
extern int ecat7Write2DNormheader(FILE *fp, int blk, ECAT7_2Dnormheader *h);
extern int ecat7WritePolarmapMatrix(FILE *fp, int matrix_id,
      ECAT7_polmapheader *h, float *fdata);
extern int ecat7WriteMatrixdata(FILE *fp, int start_block, char *data,
      int pxl_nr, int pxl_size);
/*void ecat7wFloat(float *bufi, void *bufo, int tovax, int islittle);*/
/*void ecat7wInt(int *bufi, void *bufo, int tovax, int islittle);*/
extern FILE *ecat7Create(const char *fname, ECAT7_mainheader *h);
extern int ecat7WriteImageMatrix(FILE *fp, int matrix_id, ECAT7_imageheader *h,
      float *fdata);
extern int ecat7Write2DScanMatrix(FILE *fp, int matrix_id, ECAT7_2Dscanheader *h,
      float *fdata);
extern int ecat7WriteScanMatrix(FILE *fp, int matrix_id, ECAT7_scanheader *h,
      float *fdata);
extern int ecat7_is_scaling_needed(float amax, float *data, int nr);
/*****************************************************************************/
/* Printing functions */
extern void ecat7PrintMainheader(ECAT7_mainheader *h, FILE *fp);
extern void ecat7PrintImageheader(ECAT7_imageheader *h, FILE *fp);
extern void ecat7PrintScanheader(ECAT7_scanheader *h, FILE *fp);
extern void ecat7PrintAttenheader(ECAT7_attenheader *h, FILE *fp);
extern void ecat7PrintPolmapheader(ECAT7_polmapheader *h, FILE *fp);
extern void ecat7PrintNormheader(ECAT7_normheader *h, FILE *fp);
extern void ecat7Print2DScanheader(ECAT7_2Dscanheader *h, FILE *fp);
extern void ecat7Print2DNormheader(ECAT7_2Dnormheader *h, FILE *fp);
extern int ecat7PrintSubheader(ECAT7_mainheader mh, FILE *fp,
      int plane, int frame, FILE *ofp);
/* Descriptive strings for printing */
extern char* ecat7filetype(short int file_type);
extern char* ecat7acquisitiontype(short int acquisition_type);
extern char* ecat7datatype(short int data_type);
/*****************************************************************************/
/* Header edit functions */
extern int ecat7EditMHeader(ECAT7_mainheader *h, char *field, char *value);
extern int ecat7EditSHeader(ECAT7_scanheader *h, char *field, char *value);
extern int ecat7EditVHeader(ECAT7_imageheader *h, char *field, char *value);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif

