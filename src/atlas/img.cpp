#include <iostream>
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

#include "img.h"

char *Img::statusMessage[] = {
  /*  0 */ "ok",
  /*  1 */ "fault in calling routine",
  /*  2 */ "out of memory"
};

Img::Img()
{
  status = IMG_STATUS_INITIALIZED;
  statmsg = statusMessage[0];
  type = 0;
  unit = 0;
  zoom = 0.0;
  radiopharmaceutical[0] = (char)0;
  isotopeHalflife = 0.0;
  decayCorrected = (char)0;
  unit = 0;
  scanStart = 0;
  orientation = 0;
  axialFOV = transaxialFOV = sampleDistance = 0.0;
  studyNr[0] = patientName[0] = (char) 0;
  sizex = sizey = sizez = 0;
  _dataType = 0;
  _fileFormat = 0;
  scanner = 0;
  dimx = dimy = dimz = 0;
  m = (float***)NULL;
  column = (float*) NULL;
  row = (float**) NULL;
  plane = (float***) NULL;
  planeNumber = (int*) NULL;
  start = end = mid = 0;
  decayCorrFactor = 0;
}

Img::~Img()
{
  dealloc();
}

void Img::dealloc()
{
  if(status < IMG_STATUS_OCCUPIED)
    return;

  if(_col != NULL)
    free(_col);

  if(_row != NULL)
    free(_row);

  if(_pln != NULL)
    free(_pln);

  if(dimz > 0)
    free(planeNumber);

  /* Set variables */
  statmsg = statusMessage[0];
  type = 0;
  unit = 0;
  zoom = 0.0;
  radiopharmaceutical[0] = (char)0;
  isotopeHalflife = 0.0;
  decayCorrected = (char)0;
  unit = 0;
  scanStart = 0;
  orientation = 0;
  axialFOV = transaxialFOV = sampleDistance = 0.0;
  studyNr[0] = patientName[0] = (char) 0;
  sizex = sizey = sizez = 0;
  _dataType = 0;
  _fileFormat = 0;
  scanner = 0;
  dimx = dimy = dimz = 0;
  m = (float***)NULL;
  column = (float*) NULL;
  row = (float**) NULL;
  plane = (float***) NULL;
  planeNumber = (int*) NULL;
  start = end = mid = 0;
  decayCorrFactor = 0;

  /* Set status */
  status = IMG_STATUS_INITIALIZED;
}

int Img::alloc(int planes, int rows, int columns)
{
  unsigned short int zi, ri;
  float **rptr, *cptr;

  /* Check arguments */
  statmsg = statusMessage[1];

  if(status == IMG_STATUS_UNINITIALIZED)
    return 1;

  if(planes < 1 || rows < 1 || columns < 1)
    return 1;

  if(status >= IMG_STATUS_OCCUPIED)
    dealloc();

  /* Allocate memory for header data */
  statmsg = statusMessage[2];

  planeNumber = (int*) calloc(planes, sizeof(int));
  if(planeNumber == NULL) {
    return 1;
  }

  /* Allocate memory for image data */
  _pln = (float***) malloc(planes * sizeof(float**));
  if(_pln == NULL) {
    free(planeNumber);
    return 1;
  }

  _row = (float**) malloc(planes * rows * sizeof(float*));
  if(_row == NULL) {
    free(planeNumber);
    free(_pln);
    return 1;
  }

  _col = (float*) malloc(planes * rows * columns * sizeof(float));
  if(_col == NULL) {
    free(planeNumber);
    free(_pln);
    free(_row);
    return 1;
  }

  /* Set data pointers */
  rptr = _row;
  cptr = _col;

  for(zi = 0; zi < planes; zi++) {

    _pln[zi] = rptr;

    for(ri = 0; ri < rows; ri++) {

      *rptr++ = cptr;
      cptr += rows;

    } /* End for ri */

  } /* End for zi */

  m = _pln;
  plane = _pln;
  column = _col;
  row = _row;

  /* Ok */
  dimz = planes;
  dimy = rows;
  dimx = columns;

  statmsg = statusMessage[0];
  status = IMG_STATUS_OCCUPIED;

  return 0;
}

float Img::getMax()
{
  int xi, yi, zi;
  float val, max = 0.0;

  for (zi = 0; zi < dimz; zi++) {

    for (yi = 0; yi < dimy; yi++) {

      for (xi = 0; xi < dimx; xi++) {

        val = (int) m[zi][yi][xi];

        if (max < val)
          max = val;

      } /* End for xi */

    } /* End for yi */

  } /* End for xi */

  return max;
}

void Img::getHorizontalData(
  uint32_t *buf,
  float lowLimit,
  float highLimit,
  ColorMap *cmap,
  uint8_t alpha
  )
{
  uint8_t *img = (uint8_t *) buf;
  float k = 1.0 / (highLimit - lowLimit);
  float numColors = float(cmap->getNumColors() - 1);

  for (int zi = 0; zi < dimz; zi++) {

    for (int yi = 0; yi < dimy; yi++) {

      for (int xi = 0; xi < dimx; xi++) {

        float val = m[zi][yi][xi];
        float result;
        if (val > lowLimit && val < highLimit) {
            result = (val - lowLimit) * k;
            if (result < 0.0) result = 0.0;
            if (result > 1.0) result = 1.0;
        }
        else if (val > highLimit) {
            result = 1.0;
        }
        else {
            result = 0.0;
        }

        int out = int(numColors * result);
        *img = cmap->getRed(out); img++;
        *img = cmap->getGreen(out); img++;
        *img = cmap->getBlue(out); img++;
        *img = alpha; img++;

      } /* End for xi */

    } /* End for yi */

  } /* End for zi */
}

int Img::getDimx()
{
  return dimx;
}

int Img::getDimy()
{
  return dimy;
}

int Img::getDimz()
{
  return dimz;
}

char* Img::getStatusMessage()
{
  return statmsg;
}

