#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "colormap.h"
#include "img.h"

class GLWidget;

class Window : public QWidget
{

public:
    Window();

    int loadColormap(const char *fn);
    void setImg(Img *img, float lowLimit, float highLimit, int slice = 0);

protected:
    void readImgData();

private:
    GLWidget *glWidget;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    unsigned long *imgData;

    int imgSlice;
    float imgLowLimit, imgHighLimit;
};

#endif

