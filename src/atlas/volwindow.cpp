#include <QtGui>

#include <stdlib.h>
#include <iostream>
#include <string>

#include "img/vffimg.h"
#include "img/ecat7img.h"
#include "volwidget.h"
#include "volwindow.h"

VolWindow::VolWindow()
    : colorMap(),
      imgLoaded(false),
      imgRead(false)
{
    workWidget = new QWidget;
    setCentralWidget(workWidget);

    volWidget = new VolWidget(this);

    frameScroll = new QScrollBar(Qt::Horizontal);
    connect(frameScroll, SIGNAL(valueChanged(int)), this, SLOT(setFrame(int)));
    connect(this, SIGNAL(frameChanged(int)), frameScroll, SLOT(setValue(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(volWidget);
    mainLayout->addWidget(frameScroll);

    workWidget->setLayout(mainLayout);

    frameScroll->setValue(0);

    setWindowTitle(tr("Volume window"));
}

void VolWindow::setFrame(int frame)
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

void VolWindow::readImgData()
{
    if (imgRead)
    {
        volWidget->unsetData();
    }

    const int xBricks = 1;
    const int yBricks = 1;
    const int zBricks = 1;

    volWidget->setBricks(xBricks, yBricks, zBricks);
    int w = imgBase->getWidth() / xBricks;
    int h = imgBase->getHeight() / yBricks;
    int d = imgBase->getDepth() / zBricks;
    int sz = w * h * d;
    for (int zi = zBricks - 1; zi >= 0; zi--)
    {
        for (int yi = 0; yi < yBricks; yi++)
        {
            for (int xi = 0; xi < xBricks; xi++)
            {
                uint32_t *data = new uint32_t[sz];
                imgBase->getData(
                     data,
                     &colorMap,
                     xi * w, xi * w + w,
                     yi * h, yi * h + h,
                     zi * d, zi * d + d
                    );
                volWidget->addBrick(w, h, d, data);
            } // End for xi
        } // End for yi
    } // End for zi

    imgRead = true;
}

int VolWindow::loadImg(
    const char *fn,
    int frame
    )
{
    Img *img = 0;

    std::string fname(fn);
    size_t pos = fname.find_last_of(".");
    std::string ext = fname.substr(pos + 1);
    if (ext.compare("vff") == 0)
    {
        img = new VffImg;
    }
    else if (ext.compare("v") == 0)
    {
        img = new Ecat7Img;
    }

    int result = img->open(fn);
    if (result == 0)
    {
        result = readImg(img, frame);
    }

    return result;
}

int VolWindow::readImg(
    Img *img,
    int frame
    )
{
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
        imgBase = img;
        imgBase->setTransparency(Img::TRANSPARENCY_VOXEL);
        readImgData();
        frameScroll->setRange(0, imgBase->getFrameNr() - 1);
        frameScroll->setValue(imgFrame);

        imgLoaded = true;
    }

    return result;
}

void VolWindow::closeImg()
{
    if (imgRead)
    {
        volWidget->unsetData();
        imgRead = false;
    }

    if (imgLoaded)
    {
        imgLoaded = false;
        imgBase->close();
        delete imgBase;
        imgBase = 0;
    }
}

int VolWindow::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) && imgLoaded)
    {
        readImgData();
    }

    return result;
}

