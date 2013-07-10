#ifndef VOLWINDOW_H
#define VOLWINDOW_H

#include <QMainWindow>

#include <stdint.h>

#include "img/colormap.h"
#include "img/img.h"

class QScrollBar;
class VolWidget;

class VolWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void setFrame(int frame);

signals:
    void frameChanged(int frame);

public:
    VolWindow();

    int loadImg(
        const char *fn,
        int frame = -1
        );

    int loadColormap(const char *fn);

private:
    void readImgData();
    int readImg(
        Img *img,
        int frame = -1 
        );
    void closeImg();

    QWidget *workWidget;
    VolWidget *volWidget;
    QScrollBar *frameScroll;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    bool imgRead;

    int imgFrame;
};

#endif

