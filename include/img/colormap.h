#ifndef COLORMAP_H
#define COLORMAP_H

#include <stdint.h>

struct Color3
{
    Color3() { }
    Color3 (uint16_t red, uint16_t green, uint16_t blue)
        : r(red / 65535.0),
          g(green / 65535.0),
          b(blue / 65535.0) { }

    float r, g, b;
};

class ColorMap
{
public:
    ColorMap(float w = 255);
    ~ColorMap()
    {
        if (initialized)
        {
            delete col;
            initialized = false;
        }
    }

    int loadColormap(const char *fn);
    int getNumColors() { return nColors; }

    inline uint8_t r(int i)
    {
        if (!initialized) return 0;
        if (i >= nColors || i < 0) return 0;
        return (uint8_t) (col[i].r * weight);
    }

    inline uint8_t g(int i)
    {
        if (!initialized) return 0;
        if (i >= nColors || i < 0) return 0;
        return (uint8_t) (col[i].g * weight);
    }

    inline uint8_t b(int i)
    {
        if (!initialized) return 0;
        if (i >= nColors || i < 0) return 0;
        return (uint8_t) (col[i].b * weight);
    }

private:
    Color3 *col;
    int nColors;
    float weight;
    bool initialized;
};

#endif

