#ifndef V_H
#define V_H

#include "ecat7.h"
#include "img.h"

class V : public Img {

private:

  static char *imgmsg[];

  FILE *fp;

  ECAT7_mainheader main_header;
  ECAT7_MATRIXLIST mlist;

  int dimx, dimy, dimz, frameNr, planeNr, pxlNr;
  bool fileOpen, matlistLoaded, imgAllocated;

  int readMainHeader();
  int readMatrixList();
  int calculateNr();
  int readSubHeader();
  void setFileFormat();
  void setImgUnit();
  void headerToImg();

public:

  V();
  V(char *fname);
  V(char *fname, int t);
  ~V();

  int open(char *fname);
  void close();
  int read(int t);

  int getFrameNr();
};

#endif

