#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "colormap.h"
#include "img.h"

class GLWidget;
class QSlider;

class Window : public QWidget
{
    Q_OBJECT


public slots:
    void setSlice(int slice);

signals:
    void sliceChanged(int slice);

public:
    Window();

    int loadColormap(const char *fn);
    void setImg(Img *img, float lowLimit, float highLimit, int slice = 0);

protected:
    void readImgData();

private:
    GLWidget *glWidget;
    QSlider *slider;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    unsigned long *imgData;

    int imgSlice;
    float imgLowLimit, imgHighLimit;
};

#endif

