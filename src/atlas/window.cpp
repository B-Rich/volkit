#include <QtGui>

#include <iostream>
#include <stdlib.h>

#include "v.h"
#include "glwidget.h"
#include "window.h"

Window::Window() :
    colorMap(),
    imgLoaded(false),
    imgLowLimit(0.0),
    imgHighLimit(65000.0)
{
    workWidget = new QWidget;
    setCentralWidget(workWidget);

    glWidget = new GLWidget;

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

    setWindowTitle(tr("Atlas"));
}

void Window::createActions()
{
    fileOpenAct = new QAction(tr("&Open"), this);
    connect(fileOpenAct, SIGNAL(triggered()), this, SLOT(fileOpen()));

    fileColormapAct = new QAction(tr("&Colormap"), this);
    connect(fileColormapAct, SIGNAL(triggered()), this, SLOT(fileColormap()));

    fileExitAct = new QAction(tr("&Exit"), this);
    connect(fileExitAct, SIGNAL(triggered()), this, SLOT(fileExit()));
}

void Window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileOpenAct);
    fileMenu->addAction(fileColormapAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fileExitAct);
}

void Window::fileOpen()
{
    QString fn =
        QFileDialog::getOpenFileName(this, tr("Open File..."),
            QString(), tr("Ecat7 (*.v);;All Files (*)"));
    if (!fn.isEmpty())
    {
        loadImg(fn.toStdString().c_str(), imgLowLimit, imgHighLimit);
    }
}

void Window::fileColormap()
{
    QString fn =
        QFileDialog::getOpenFileName(this, tr("Load Colormap..."),
            QString(), tr("Colormap (*.cmap);;All Files (*)"));
    if (!fn.isEmpty())
    {
        loadColormap(fn.toStdString().c_str());
    }
}

void Window::fileExit()
{
    exit(0);
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
        imgLowLimit,
        imgHighLimit,
        &colorMap
        );
    glWidget->setData(
        imgBase->getDimx(), imgBase->getDimy(),
        &imgData[imgSlice * imgBase->getDimx() * imgBase->getDimy()]
        );
}

int Window::loadImg(
    const char *fn,
    float lowLimit,
    float highLimit,
    int slice,
    int frame
    )
{
    V *img = new V;
    int result = img->open(fn);

    if (result == 0)
    {
        result = readImg(img, lowLimit, highLimit, slice, frame);
    }

    return result;
}

int Window::readImg(
    Img *img,
    float lowLimit,
    float highLimit,
    int slice,
    int frame
    )
{
    imgLowLimit = lowLimit;
    imgHighLimit = highLimit;

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

int Window::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) &&imgLoaded)
    {
        readImgData();
    }

    return result;
}

