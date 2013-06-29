#ifndef COLORMAP_H
#define COLORMAP_H

struct Color {

  float val[3];
};

class ColorMap {

  private:

  Color *col;
  int nColors;
  float weight;
  bool initialized;

  public:

  ColorMap(float weight = 255.0f);
  ColorMap(const char *fn, float weight = 255.0f);

  ~ColorMap() {
    if (initialized) {
      delete col;
      initialized = false;
    }
  }

  int loadColormap(const char *fn);

  int getNumColors()
  {
    return nColors;
  }

  inline char unsigned getRed(int i)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    return (unsigned char)(col[i].val[0] * weight);
  }

  inline char unsigned getGreen(int i)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    return (unsigned char)(col[i].val[1] * weight);
  }

  inline char unsigned getBlue(int i)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    return (unsigned char)(col[i].val[2] * weight);
  }

  inline unsigned long getRGBA(int i, int alpha = 255)
  {
      return (unsigned long)(
      	((unsigned long)(col[i].val[0] * weight)) |
      	((unsigned long)(col[i].val[1] * weight) << 8) |
      	((unsigned long)(col[i].val[2] * weight) << 16) |
      	((unsigned long)(alpha) << 24)
      );
  }

  inline unsigned long getLong(int i, bool msbFirst)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    if (msbFirst) {
      return (unsigned long)(
      	((unsigned long)(col[i].val[0] * weight)) |
      	((unsigned long)(col[i].val[1] * weight) << 8) |
      	((unsigned long)(col[i].val[2] * weight) << 16)
      );
    }
    else {
      return (unsigned long)(
      	((unsigned long)(col[i].val[0] * weight) << 16) |
      	((unsigned long)(col[i].val[1] * weight) << 8) |
      	((unsigned long)(col[i].val[2] * weight))
      );
    }
  }

  inline unsigned short getShort(int i, bool msbFirst)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    if (msbFirst) {
      return (unsigned long)(
      	((unsigned long)(col[i].val[0] * weight)) |
      	((unsigned long)(col[i].val[1] * weight) << 5) |
      	((unsigned long)(col[i].val[2] * weight) << 11)
      );
    }
    else {
      return (unsigned long)(
      	((unsigned long)(col[i].val[0] * weight) << 5) |
      	((unsigned long)(col[i].val[1] * weight) << 11) |
      	((unsigned long)(col[i].val[2] * weight))
      );
    }
  }

};

#endif

