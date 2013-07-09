#ifndef VOLWINDOW_H
#define VOLWINDOW_H

#include <QMainWindow>

#include <stdint.h>

#include "img/colormap.h"
#include "img/img.h"

class VolWidget;

class VolWindow : public QMainWindow
{
    Q_OBJECT

public:
    VolWindow();

    int loadImg(
        const char *fn,
        int frame = -1
        );

private:
    void readImgData();
    int readImg(
        Img *img,
        int frame = -1 
        );
    void closeImg();

    VolWidget *volWidget;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    uint32_t *imgData;

    int imgFrame;
};

#endif

