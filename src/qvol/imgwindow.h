#ifndef IMGWINDOW_H
#define IMGWINDOW_H

#include <QMainWindow>

#include <stdint.h>

#include "volimg/colormap.h"
#include "volimg/img.h"
#include "tool.h"
#include "imgparams.h"

class QWidget;
class QMenu;
class QToolBar;
class QScrollBar;
class ImgWidget;
class ImgParams;

class ImgWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void imageOpen();
    void imageClose();
    void imageHorizontal();
    void imageSagittal();
    void imageCoronal();
    void imageParams();
    void imageColormap();
    void imageExit();

    void toolsSelect();
    void toolsPolygon();
    void toolsSample();

    void setSlice(int slice);
    void setFrame(int frame);

signals:
    void sliceChanged(int slice);
    void frameChanged(int frame);

public:
    ImgWindow();

    void setOrientation(Img::Orientation o);
    void setParams(float low, float high, bool trans);

    int loadImg(
        const char *fn,
        int slice = -1,
        int frame = -1
        );

    int loadColormap(const char *fn);

private:
    void createActions();
    void createMenus();
    void createToolBars();

    void readImgData();
    int readImg(
        Img *img,
        int slice = -1,
        int frame = -1 
        );
    void setLimit(float low, float high);
    void closeImg();

    QMenu *imageMenu;
    QAction *imageOpenAct;
    QAction *imageCloseAct;
    QAction *imageHorizontalAct;
    QAction *imageSagittalAct;
    QAction *imageCoronalAct;
    QAction *imageParamsAct;
    QAction *imageColormapAct;
    QAction *imageExitAct;

    QToolBar *toolsToolBar;
    QAction *toolsSelectAct;
    QAction *toolsPolygonAct;
    QAction *toolsSampleAct;

    QWidget *workWidget;
    ImgWidget *imgWidget;
    QScrollBar *sliceScroll;
    QScrollBar *frameScroll;

    ColorMap colorMap;

    bool imgLoaded;
    Img *imgBase;
    uint32_t *imgData;

    int imgSlice;
    int imgFrame;

    ImgParams imgParams;
    Tool *selectedTool;
};

#endif

