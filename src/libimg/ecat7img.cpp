#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>

extern "C" {
#include "ecat7.h"
#include "swap.h"
}

#include "img/img.h"
#include "img/ecat7img.h"

const char *Ecat7Img::imgmsg[] =
{
    "ok",                                // 0
    "fault in calling routine",          // 1
    "out of memory",                     // 2
    "cannot open file",                  // 3
    "unknown file format",               // 4
    "unsupported file type",             // 5
    "invalid matrix list",               // 6
    "missing matrix",                    // 7
    "variable matrix size",              // 8
    "cannot read subheader",             // 9
    "cannot read matrix",                // 10
    "axial compression is not supported" // 11
};

Ecat7Img::Ecat7Img()
{
    matlistLoaded = false;
}

Ecat7Img::~Ecat7Img()
{
    close();
}

int Ecat7Img::readMainHeader()
{
    int ret;

    // Read main header
    ret = ecat7ReadMainheader(fp, &main_header);
    if(ret)
    {
        statmsg = imgmsg[4];
        return 1;
    }

    // Check for magic number
    if(strncmp(main_header.magic_number, ECAT7V_MAGICNR, 7) != 0)
    {
        statmsg = imgmsg[4];
        return 1;
    }

    // Check if file_type is supported
    if(main_header.file_type != ECAT7_VOLUME8 &&
       main_header.file_type != ECAT7_VOLUME16 &&
       main_header.file_type != ECAT7_IMAGE8 &&
       main_header.file_type != ECAT7_IMAGE16 &&
       main_header.file_type != ECAT7_2DSCAN &&
       main_header.file_type != ECAT7_3DSCAN8 &&
       main_header.file_type != ECAT7_3DSCAN &&
       main_header.file_type != ECAT7_3DSCANFIT)
    {
        statmsg = imgmsg[5];
        return 1;
    }

    return 0;
}

int Ecat7Img::readMatrixList()
{
    int ret;

    // Read matrix list
    ecat7InitMatlist(&mlist);
    ret = ecat7ReadMatlist(fp, &mlist);

    if(ret || mlist.matrixNr<1 || ecat7CheckMatlist(&mlist))
    {
        statmsg = imgmsg[6];
        return 1;
    }

    // Sort by plane
    ecat7SortMatlistByPlane(&mlist);

    matlistLoaded = true;

    return 0;
}

int Ecat7Img::calculateNr()
{
    ECAT7_Matval matval;
    int ret, m, frame, plane, prev_frame, prev_plane;
    int blkNr = 0;

    // Calculate the number of planes, frames and gates
    // Check that all planes have equal nr of frames (gates)
    // and check that frames (gates) are consequentally numbered
    prev_plane = plane = -1;
    prev_frame = frame = -1;
    frameNr = planeNr = 0;
    ret = 0;

    for(m = 0; m < mlist.matrixNr; m++)
    {
        // Get next matrix
        ecat7_id_to_val(mlist.matdir[m].id, &matval);
        plane = matval.plane;

        // Get current frame or gate
        if(main_header.num_frames >= main_header.num_gates)
        {
            frame = matval.frame;
        }
        else
        {
            frame = matval.gate;
        }

        if(plane != prev_plane)
        {
            frameNr = 1;
            planeNr++;
        }
        else
        {
            frameNr++;
            if(prev_frame > 0 && frame != prev_frame + 1)
            {
                ret = 1;
                break;
            }
        }
        prev_plane = plane;
        prev_frame = frame;

        // Calculate and check the size of one data matrix
        if(m == 0)
        {
            blkNr = mlist.matdir[m].endblk - mlist.matdir[m].strtblk;
        }
        else if(blkNr != mlist.matdir[m].endblk - mlist.matdir[m].strtblk)
        {
            ret = 2;
            break;
        }
    } // next matrix

    if(ret == 1 || (frameNr * planeNr != mlist.matrixNr))
    {
        statmsg = imgmsg[7];
        return 1;
    }

    if(ret == 2)
    {
        statmsg = imgmsg[8];
        return 1;
    }

    return 0;
}

int Ecat7Img::readSubHeader()
{
    int ret, i, m;
    ECAT7_imageheader image_header;
    ECAT7_2Dscanheader scan2d_header;
    ECAT7_scanheader scan_header;

    // Read the first subheader to get planeNr from volumes and to get x&y dim
    m = 0;
    zSize = 1;
    statmsg = imgmsg[9];

    switch(main_header.file_type)
    {
        case ECAT7_IMAGE8:
        case ECAT7_IMAGE16:
        case ECAT7_VOLUME8:
        case ECAT7_VOLUME16:
            type = IMG_TYPE_IMAGE;
            ret = ecat7ReadImageheader(
                fp,
                mlist.matdir[m].strtblk,
                &image_header
                );

            xSize = image_header.x_dimension;
            ySize = image_header.y_dimension;
            if(image_header.num_dimensions > 2 && image_header.z_dimension > 1)
            {
                planeNr = zSize = image_header.z_dimension;
            }
            break;

        case ECAT7_2DSCAN:
            type = IMG_TYPE_RAW;
            ret = ecat7Read2DScanheader(
                fp,
                mlist.matdir[m].strtblk,
                &scan2d_header
                );

                xSize = scan2d_header.num_r_elements;
                ySize = scan2d_header.num_angles;
                if(scan2d_header.num_dimensions > 2 &&
                   scan2d_header.num_z_elements > 1)
                {
                    planeNr = zSize = scan2d_header.num_z_elements;
                }
                break;

        case ECAT7_3DSCAN:
        case ECAT7_3DSCAN8:
        case ECAT7_3DSCANFIT:
            type = IMG_TYPE_RAW;
            ret = ecat7ReadScanheader(
                fp,
                mlist.matdir[m].strtblk,
                &scan_header
                );

            xSize = scan_header.num_r_elements;
            ySize = scan_header.num_angles;

            for(i = zSize = 0; i < 64; i++)
            {
                zSize += scan_header.num_z_elements[i];
            }

            planeNr = zSize;
            break;

        default:
            xSize = ySize = zSize = planeNr=0;
            ret = -1;
            break;
    }

    pxlNr = xSize * ySize;
    if(ret || pxlNr < 1 || planeNr < 1)
    {
        return 1;
    }

    statmsg = imgmsg[0];

    return 0;
}

void Ecat7Img::setImgUnit()
{
    switch(unit)
    {
        case 1:
        case 2:
            main_header.calibration_units = 0;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "ECAT counts/sec");
            break;

        case 3:
            main_header.calibration_units = 1;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "kBq/cc");
            break;

        case 4:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "sec*kBq/mL");
            break;

        case 5:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "1/sec");
            break;

        case 6:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "1/min");
            break;

        case 7:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "mL/mL");
            break;

        case 8:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "mL/dL");
            break;

        case 9:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "mL/(mL*min)");
            break;

        case 10:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "mL/(dL*min)");
            break;

        case 11:
            main_header.calibration_units = 1;
            main_header.calibration_units_label = 0;
            strcpy(main_header.data_units, "");
            break;

        case 12:
            main_header.calibration_units = 1;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "nCi/cc");
            break;

        case 13:
            main_header.calibration_units = 1;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "MBq/cc");
            break;

        case 14:
            main_header.calibration_units = 1;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "Bq/cc");
            break;

        case 15:
            main_header.calibration_units = 1;
            main_header.calibration_units_label = 1;
            strcpy(main_header.data_units, "uCi/cc");
            break;

        default:
            main_header.calibration_units = 2;
            main_header.calibration_units_label = 0;
            strcpy(main_header.data_units, "");
            break;
    }
}

void Ecat7Img::headerToImg()
{
    scanner = main_header.system_type;
    setImgUnit();
    strncpy(radiopharmaceutical, main_header.radiopharmaceutical, 32);
    isotopeHalflife = main_header.isotope_halflife;
    scanStart = main_header.scan_start_time;
    axialFOV = 10.0 * main_header.distance_scanned;
    transaxialFOV = 10.0 * main_header.transaxial_fov;
    strncpy(studyNr, main_header.study_type, MAX_STUDYNR_LEN);
    strcpy(patientName, main_header.patient_name);
    sizez = 10.0 * main_header.plane_separation;
    switch(main_header.file_type)
    {
        case ECAT7_IMAGE8:
        case ECAT7_IMAGE16:
        case ECAT7_VOLUME8:
        case ECAT7_VOLUME16:
            type = IMG_TYPE_IMAGE;
            break;

        default:
            type = IMG_TYPE_RAW;
            break;
    }

    orientation = main_header.patient_orientation;
}

void Ecat7Img::setFileFormat()
{
    // Set fileFormat
    switch(main_header.file_type)
    {
        case ECAT7_IMAGE8:
        case ECAT7_IMAGE16:
            _fileFormat = IMG_E7_2D;
            break;

        case ECAT7_VOLUME8:
        case ECAT7_VOLUME16:
            _fileFormat = IMG_E7;
            break;

        case ECAT7_2DSCAN:
            _fileFormat = IMG_E7_2D;
            break;

        case ECAT7_3DSCAN:
        case ECAT7_3DSCAN8:
        case ECAT7_3DSCANFIT:
            _fileFormat = IMG_E7;
            break;

        default:
            _fileFormat = IMG_UNKNOWN;
            break;
    }
}

int Ecat7Img::open(const char *fname)
{
    if(!fname)
    {
        statmsg = imgmsg[1];
        return 1;
    }

    // Open file for read
    fp = fopen(fname, "rb");
    if (!fp)
    {
        statmsg = imgmsg[3];
        return 1;
    }
    fileOpen = true;

    // Read header
    if (readMainHeader() > 0)
    {
        close();
        return 1;
    }

    // Read matrix list
    if (readMatrixList() > 0)
    {
        close();
        return 1;
    }

    // Calculate number of planeNr, frameNr
    if (calculateNr() > 0)
    {
        close();
        return 1;
    }

    // Read subheader and calculate xSize, ySize, zSize and pxlNr
    if (readSubHeader() > 0)
    {
        close();
        return 1;
    }

    if (alloc(planeNr, xSize, ySize) > 0)
    {
        statmsg = imgmsg[2];
        close();
        return 1;
    }

    // Copy information from mainheader
    headerToImg();

    // Set file format
    setFileFormat();

    return 0;
}

void Ecat7Img::close()
{
    // dealloc checks if image is occupied
    dealloc();

    if (matlistLoaded)
    {
        ecat7EmptyMatlist(&mlist);
        matlistLoaded = false;
    }

    if (fileOpen)
    {
        fclose(fp);
        fileOpen = false;
    }
}

int Ecat7Img::read(int t)
{
    int ret, i, pi, xi, yi;

    ECAT7_imageheader image_header;
    ECAT7_2Dscanheader scan2d_header;
    ECAT7_scanheader scan_header;
    ECAT7_Matval matval;

    float *fdata = 0, *fptr;

    if (!fileOpen || status != IMG_STATUS_OCCUPIED)
    {
        statmsg = imgmsg[1];
        return 1;
    }

    if(zSize > 1)
    {
        // Read ECAT volume matrices
        ecat7_id_to_val(mlist.matdir[t].id, &matval);

        // Read subheader and data
        if(type == IMG_TYPE_IMAGE)
        {
            ret = ecat7ReadImageMatrix(
                fp,
                mlist.matdir[t].strtblk,
                mlist.matdir[t].endblk,
                &image_header,
                &fdata
                );
        }
        else
        {
            ret = ecat7ReadScanMatrix(
                fp,
                mlist.matdir[t].strtblk,
                mlist.matdir[t].endblk,
                &scan_header,
                &fdata
                );
        }

        if(ret || fdata == 0)
        {
            statmsg = imgmsg[10];
            return 1;
        }

        // Copy subheader information
        if(type == IMG_TYPE_IMAGE)
        {
            _dataType = image_header.data_type;
            start = image_header.frame_start_time / 1000.0;
            end = start + image_header.frame_duration / 1000.0;
            mid = 0.5 * (start + end);

            if(image_header.decay_corr_fctr > 1.0)
            {
                decayCorrFactor = image_header.decay_corr_fctr;
                decayCorrected = 1;
            }

            zoom = image_header.recon_zoom;
            sizex = 10.0 * image_header.x_pixel_size;
            sizey = 10.0 * image_header.y_pixel_size;
            sizez = 10.0 * image_header.z_pixel_size;

        }
        else
        {
            _dataType = scan_header.data_type;
            start = scan_header.frame_start_time / 1000.0;
            end = start + scan2d_header.frame_duration / 1000.0;
            mid = 0.5 * (start + end);

            if(scan_header.x_resolution > 0.0)
            {
                sampleDistance = 10.0 * scan_header.x_resolution;
            }
            else
            {
                sampleDistance = 10.0 * main_header.bin_size;
            }

            // also, correct for dead-time
            if(scan_header.deadtime_correction_factor > 0.0)
            {
                for(i = 0, fptr = fdata; i < pxlNr; i++, fptr++)
                {
                    *fptr *= scan_header.deadtime_correction_factor;
                }
            }
        }

        // Copy matrix data through volume planes
        for(pi = 0; pi < zSize; pi++)
        {
            for(yi = 0, fptr = fdata + pi * pxlNr; yi < ySize; yi++)
            {
                for(xi = 0; xi < xSize; xi++)
                {
                    m[pi][yi][xi] = *fptr++;
                } // End for xi
            } // End for yi
        } // End for pi

        delete fdata;

        // Set plane numbers
        for(pi = 0; pi < zSize; pi++)
        {
            planeNumber[pi] = pi + 1;
        }
    }
    else
    {
        // Read separate matrices
        fprintf(stderr, "Error - Seprate matrices not implemented.\n");
    }

    // Calibrate
    if(main_header.ecat_calibration_factor > 0.0)
    {
        for(pi = 0; pi < zSize; pi++)
        {
            for(yi = 0; yi < ySize; yi++)
            {
                for(xi = 0; xi < xSize; xi++)
                {
                    m[pi][yi][xi] *= main_header.ecat_calibration_factor;
                } // End for xi
            } // End for yi
        } // End for pi
    }

    statmsg = imgmsg[0];

    return 0;
}

