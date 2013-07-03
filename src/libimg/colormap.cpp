#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "img/colormap.h"

ColorMap::ColorMap(float w)
{
    float di = 1.0 / 256.0;
    float val = 0.0;
    weight = w;

    nColors = 256;
    col = new Color3[nColors];
    for (int i = 0; i < nColors; i++)
    {
        col[i].r = val;
        col[i].g = val;
        col[i].b = val;
        val += di;
    }

    initialized = true;
}

int ColorMap::loadColormap(const char *fn)
{
    int status = -1;

    if (initialized)
    {
        initialized = false;
        delete col;
    }

    std::vector<Color3> values;
    std::ifstream file;
    file.open(fn);
    if (file.is_open())
    {
        std::string line;
        std::stringstream ss;
        while(file.good())
        {
            std::getline(file, line);
            std::stringstream ss(line);

            uint16_t r, g, b;
            ss >> r;
            ss >> g;
            ss >> b;

            Color3 c(r, g, b);
            values.push_back(c);
        }

        file.close();

        nColors = values.size();
        col = new Color3[nColors];
        for (int i = 0; i < nColors; i++)
        {
            col[i].r = values[i].r;
            col[i].g = values[i].g;
            col[i].b = values[i].b;
        }

        status = 0;
        initialized = true;
    }

    return status;
}

