#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "colormap.h"

ColorMap::ColorMap(float weight)
{
  float di = 1.0f / 256.0f;
  float val = 0.0f;

  this->weight = weight;

  nColors = 256;

  col = new Color[nColors];

  for (int i = 0; i < nColors; i++) {

    col[i].val[0] = val;
    col[i].val[1] = val;
    col[i].val[2] = val;

    val += di;
  }

  initialized = true;
}

ColorMap::ColorMap(const char *fn, float weight) {

  this->weight = weight;

  if (loadColormap (fn) == 0) {
    initialized = true;
    return;
  }

  fprintf(stderr, "Colormap file: %s not found\n"
        	  "Revert to default colormap\n", fn);

  ColorMap(this->weight);
}

int ColorMap::loadColormap(const char *fn) {

  if (initialized) {
    initialized = false;
    delete col;
  }

  FILE *fp;

  if ( (fp = fopen(fn, "r") ) == NULL) {
    fprintf(stderr, "Unable to open file: %s\n", fn);
    return 1;
  }

  fscanf(fp, "%d\n", &nColors);

#ifdef DEBUG
  printf("Loading colormap with %d colors.\n", nColors);
#endif

  col = new Color[nColors];

  char s1[80], s2[80], s3[80];
  float r,g,b;

  for (int i = 0; i < nColors; i++) {

    fscanf(fp, "%s %s %s\n", s1, s2, s3);

    r = atof(s1);
    g = atof(s2);
    b = atof(s3);

    col[i].val[0] = r;
    col[i].val[1] = g;
    col[i].val[2] = b;

#ifdef DEBUG
    printf("%s, %s, %s:", s1, s2, s3);
    printf("%f, %f, %f\n",r,g,b);
#endif

  }

  fclose(fp);

  initialized = true;

  return 0;
}

