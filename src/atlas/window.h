#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>

#include <stdint.h>

#include "colormap.h"
#include "img.h"

class GLWidget;
class QMenu;
class QScrollBar;

class Window : public QMainWindow
{
    Q_OBJECT

public slots:
    void fileOpen();
    void fileColormap();
    void fileExit();

    void setSlice(int slice);
    void setFrame(int frame);

signals:
    void sliceChanged(int slice);
    void frameChanged(int frame);

public:
    Window();

    int loadImg(
        const char *fn,
        float lowLimit,
        float highLimit,
        int slice = -1,
        int frame = -1
        );

    int readImg(
        Img *img,
        float lowLimit,
        float highLimit,
        int slice = -1,
        int frame = -1 
        );

    int loadColormap(const char *fn);

private:
    void createActions();
    void createMenus();

    void readImgData();

    QMenu *fileMenu;
    QAction *fileOpenAct;
    QAction *fileColormapAct;
    QAction *fileExitAct;

    QWidget *workWidget;
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

