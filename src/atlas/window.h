#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include <stdint.h>

#include "colormap.h"
#include "img.h"

class GLWidget;
class QScrollBar;

class Window : public QWidget
{
    Q_OBJECT


public slots:
    void setSlice(int slice);
    void setFrame(int frame);

signals:
    void sliceChanged(int slice);
    void frameChanged(int frame);

public:
    Window();

    int loadColormap(const char *fn);

    int readImg(Img *img,
        float lowLimit,
        float highLimit,
        int slice = 0,
        int frame = 0
        );

    void setImgZoom(float zoom);

protected:
    void readImgData();

private:
    GLWidget *glWidget;
    QScrollBar *sliceScroll;
    QScrollBar *frameScroll;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    uint32_t *imgData;

    int imgSlice;
    int imgFrame;
    float imgLowLimit, imgHighLimit;
};

#endif

