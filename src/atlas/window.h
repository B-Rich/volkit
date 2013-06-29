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

    void setImg(Img *img, float lowLimit, float highLimit, int slice = 0);
    void paintEvent(QPaintEvent *e);

private:
    GLWidget *glWidget;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    int currSlice;
    unsigned long *imgData;
};

#endif

