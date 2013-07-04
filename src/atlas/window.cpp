#include <QtGui>

#include <iostream>
#include <stdlib.h>

#include "img/v.h"
#include "glwidget.h"
#include "polytool.h"
#include "window.h"

Window::Window()
    : rangesDialog(this),
      colorMap(),
      imgLoaded(false)
{
    workWidget = new QWidget;
    setCentralWidget(workWidget);

    selectedTool = new Polytool;
    glWidget = new GLWidget(selectedTool, this);

    sliceScroll = new QScrollBar(Qt::Vertical);
    connect(sliceScroll, SIGNAL(valueChanged(int)), this, SLOT(setSlice(int)));
    connect(this, SIGNAL(sliceChanged(int)), sliceScroll, SLOT(setValue(int)));

    QHBoxLayout *horLayout = new QHBoxLayout;
    horLayout->addWidget(glWidget);
    horLayout->addWidget(sliceScroll);

    frameScroll = new QScrollBar(Qt::Horizontal);
    connect(frameScroll, SIGNAL(valueChanged(int)), this, SLOT(setFrame(int)));
    connect(this, SIGNAL(frameChanged(int)), sliceScroll, SLOT(setValue(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horLayout);
    mainLayout->addWidget(frameScroll);

    workWidget->setLayout(mainLayout);

    sliceScroll->setValue(0);
    frameScroll->setValue(0);

    createActions();
    createMenus();
    createToolBars();
    statusBar()->showMessage(tr("Ready"));

    setWindowTitle(tr("Atlas"));
}

void Window::fileOpen()
{
    QString fn =
        QFileDialog::getOpenFileName(this, tr("Open File..."),
            QString(), tr("Ecat7 (*.v);;All Files (*)"));
    if (!fn.isEmpty())
    {
        if (imgLoaded)
        {
            closeImg();
        }

        int result = loadImg(fn.toStdString().c_str());
        if (result == 0)
        {
            statusBar()->showMessage(tr("Loaded image: ") + fn);
        }
        else
        {
            statusBar()->showMessage(tr("Unable to load image: ") + fn);
        }
    }
}

void Window::fileClose()
{
    if (imgLoaded)
    {
        closeImg();
        glWidget->unsetData();
    }
}

void Window::fileRanges()
{
    rangesDialog.show();
}

void Window::fileColormap()
{
    QString fn =
        QFileDialog::getOpenFileName(this, tr("Load Colormap..."),
            QString(), tr("Colormap (*.map);;All Files (*)"));
    if (!fn.isEmpty())
    {
        int result = loadColormap(fn.toStdString().c_str());
        if (result == 0)
        {
            statusBar()->showMessage(tr("Loaded colormap: ") + fn);
        }
        else
        {
            statusBar()->showMessage(tr("Unable to load colormap: ") + fn);
        }
    }
}

void Window::fileExit()
{
    exit(0);
}

void Window::toolsSelect()
{
    selectedTool->setType(Tool::TYPE_SELECT);
    statusBar()->showMessage(tr("Click on object to select"));
}

void Window::toolsPolygon()
{
    selectedTool->setType(Tool::TYPE_POLYGON);
    statusBar()->showMessage(tr("Draw polygon"));
}

void Window::toolsSample()
{
    selectedTool->setType(Tool::TYPE_SAMPLE);
    statusBar()->showMessage(tr("Select point to sample"));
}

void Window::createActions()
{
    fileOpenAct = new QAction(tr("&Open"), this);
    connect(fileOpenAct, SIGNAL(triggered()), this, SLOT(fileOpen()));

    fileCloseAct = new QAction(tr("&Close"), this);
    connect(fileCloseAct, SIGNAL(triggered()), this, SLOT(fileClose()));

    fileRangesAct = new QAction(tr("&Ranges"), this);
    connect(fileRangesAct, SIGNAL(triggered()), this, SLOT(fileRanges()));

    fileColormapAct = new QAction(tr("&Colormap"), this);
    connect(fileColormapAct, SIGNAL(triggered()), this, SLOT(fileColormap()));

    fileExitAct = new QAction(tr("&Exit"), this);
    connect(fileExitAct, SIGNAL(triggered()), this, SLOT(fileExit()));

    toolsSelectAct = new QAction(
        QIcon(":/icons/select.png"),
        tr("Select"),
        this
        );
    toolsSelectAct->setStatusTip(tr("Select object"));
    connect(toolsSelectAct, SIGNAL(triggered()), this, SLOT(toolsSelect()));

    toolsPolygonAct = new QAction(
        QIcon(":/icons/polygon.png"),
        tr("Polygon"),
        this
        );
    toolsPolygonAct->setStatusTip(tr("Draw polygon"));
    connect(toolsPolygonAct, SIGNAL(triggered()), this, SLOT(toolsPolygon()));

    toolsSampleAct = new QAction(
        QIcon(":/icons/sample.png"),
        tr("Sample"),
        this
        );
    toolsSampleAct->setStatusTip(tr("Sample image values"));
    connect(toolsSampleAct, SIGNAL(triggered()), this, SLOT(toolsSample()));
}

void Window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileOpenAct);
    fileMenu->addAction(fileCloseAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fileRangesAct);
    fileMenu->addAction(fileColormapAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fileExitAct);
}

void Window::createToolBars()
{
    toolsToolBar = addToolBar(tr("Tools"));
    toolsToolBar->addAction(toolsSelectAct);
    toolsToolBar->addAction(toolsPolygonAct);
    toolsToolBar->addAction(toolsSampleAct);
}

void Window::setSlice(int slice)
{
    if (slice != imgSlice)
    {
        imgSlice = slice;
        if (imgLoaded)
        {
            glWidget->setData(
                imgBase->getDimx(), imgBase->getDimy(),
                &imgData[imgSlice * imgBase->getDimx() * imgBase->getDimy()]
                );
        }
    }
}

void Window::setFrame(int frame)
{
    if (frame != imgFrame)
    {
        imgFrame = frame;
        if (imgLoaded)
        {
           imgBase->read(frame);
           readImgData();
        }
    }
}

void Window::readImgData()
{
    imgBase->getHorizontalData(
        imgData,
        &colorMap
        );

    glWidget->setData(
        imgBase->getDimx(), imgBase->getDimy(),
        &imgData[imgSlice * imgBase->getDimx() * imgBase->getDimy()]
        );
}

int Window::loadImg(
    const char *fn,
    int slice,
    int frame
    )
{
    V *img = new V;
    int result = img->open(fn);

    if (result == 0)
    {
        img->setLimits(0.0, 65000.0);
        result = readImg(img, slice, frame);
    }

    return result;
}

int Window::readImg(
    Img *img,
    int slice,
    int frame
    )
{
    if (slice < 0)
    {
        imgSlice = img->getDimz() / 2;
    }
    else
    {
        imgSlice = slice;
    }

    if (frame < 0)
    {
        imgFrame = img->getFrameNr() / 2;
    }
    else
    {
        imgFrame = frame;
    }

    int result = img->read(imgFrame);
    if (result == 0)
    {
        imgData =
            new uint32_t[img->getDimx() * img->getDimy() * img->getDimz()];
        if (imgData)
        {
            imgBase = img;
            readImgData();

            sliceScroll->setRange(0, imgBase->getDimz() - 1);
            sliceScroll->setValue(imgSlice);
            frameScroll->setRange(0, imgBase->getFrameNr() - 1);
            frameScroll->setValue(imgFrame);

            imgLoaded = true;
        }
        else
        {
            result = -1;
        }
    }

    return result;
}

void Window::closeImg()
{
    if (imgLoaded)
    {
        imgLoaded = false;
        delete imgData;
        imgData = 0;
        imgBase->close();
        delete imgBase;
        imgBase = 0;
    }
}

int Window::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) &&imgLoaded)
    {
        readImgData();
    }

    return result;
}

