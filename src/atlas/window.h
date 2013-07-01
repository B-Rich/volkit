#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>

#include <stdint.h>

#include "img/colormap.h"
#include "img/img.h"

class GLWidget;
class QMenu;
class QToolBar;
class QScrollBar;

class Window : public QMainWindow
{
    Q_OBJECT

public slots:
    void fileOpen();
    void fileClose();
    void fileColormap();
    void fileExit();

    void toolsSelect();
    void toolsPolygon();
    void toolsSample();

    void setSlice(int slice);
    void setFrame(int frame);

signals:
    void sliceChanged(int slice);
    void frameChanged(int frame);

public:
    enum Tool { TOOL_SELECT, TOOL_POLYGON, TOOL_SAMPLE };

    Window();

    void setLimits(float low, float high);

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
    void closeImg();

    QMenu *fileMenu;
    QAction *fileOpenAct;
    QAction *fileCloseAct;
    QAction *fileColormapAct;
    QAction *fileExitAct;

    QToolBar *toolsToolBar;
    QAction *toolsSelectAct;
    QAction *toolsPolygonAct;
    QAction *toolsSampleAct;

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
};

#endif

