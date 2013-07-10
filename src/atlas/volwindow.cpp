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
      imgLoaded(false)
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
    imgBase->setTransparency(Img::TRANSPARENCY_VOXEL);
    imgBase->getData(imgData, &colorMap);
    volWidget->setData(
        imgBase->getWidth(), imgBase->getHeight(), imgBase->getDepth(),
        &imgData[imgBase->getWidth() * imgBase->getHeight()]
        );
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
        imgData =
            new uint32_t[img->getWidth() * img->getHeight() * img->getDepth()];
        if (imgData)
        {
            imgBase = img;
            readImgData();
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

void VolWindow::closeImg()
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

int VolWindow::loadColormap(const char *fn)
{
    int  result = colorMap.loadColormap(fn);
    if ((result == 0) && imgLoaded)
    {
        readImgData();
    }

    return result;
}

