#ifndef COLORMAP_H
#define COLORMAP_H

#include <stdint.h>

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

  inline uint8_t getRed(int i)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    return (uint8_t)(col[i].val[0] * weight);
  }

  inline uint8_t getGreen(int i)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    return (uint8_t)(col[i].val[1] * weight);
  }

  inline uint8_t getBlue(int i)
  {
    if (!initialized) return 0;
    if (i >= nColors || i < 0) return 0;
    return (uint8_t)(col[i].val[2] * weight);
  }
};

#endif

