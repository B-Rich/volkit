#include <QtGui>

#include <iostream>
#include <stdlib.h>

#include "glwidget.h"
#include "window.h"

Window::Window() :
    colorMap(),
    imgLoaded(false)
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

    fileExitAct = new QAction(tr("&Exit"), this);
    connect(fileExitAct, SIGNAL(triggered()), this, SLOT(fileExit()));
}

void Window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileOpenAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fileExitAct);
}

void Window::fileOpen()
{
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
int Window::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) &&imgLoaded)
    {
        readImgData();
    }

    return result;
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

int Window::readImg(
    Img *img,
    float lowLimit,
    float highLimit,
    int slice,
    int frame
    )
{
    int result;

    result = img->read(frame);
    if (result == 0)
    {
        imgBase = img;
        imgData =
            new uint32_t[img->getDimx() * img->getDimy() * img->getDimz()];
        imgSlice = slice;
        imgFrame = frame;
        imgLowLimit = lowLimit;
        imgHighLimit = highLimit;
        sliceScroll->setRange(0, imgBase->getDimz() - 1);
        sliceScroll->setValue(imgSlice);
        frameScroll->setRange(0, imgBase->getFrameNr() - 1);
        frameScroll->setValue(imgFrame);
        readImgData();
        imgLoaded = true;
    }

    return result;
}

