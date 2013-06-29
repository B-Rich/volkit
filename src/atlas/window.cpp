#include <QtGui>

#include <iostream>
#include <string.h>

#include "glwidget.h"
#include "window.h"

Window::Window() :
    colorMap(),
    imgLoaded(false)
{
    glWidget = new GLWidget;

    slider = new QSlider(Qt::Vertical);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setSlice(int)));
    connect(this, SIGNAL(sliceChanged(int)), slider, SLOT(setValue(int)));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(slider);
    setLayout(mainLayout);

    slider->setValue(0);
    setWindowTitle(tr("Atlas"));
}

void Window::setSlice(int slice)
{
    if (slice != imgSlice)
    {
        imgSlice = slice;
        glWidget->setData(
            imgBase->getDimx(), imgBase->getDimy(),
            &imgData[imgSlice * imgBase->getDimx() * imgBase->getDimy()]
        );
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
    imgBase->getRGBAData(imgData, imgLowLimit, imgHighLimit, &colorMap);
    glWidget->setData(
        imgBase->getDimx(), imgBase->getDimy(),
        &imgData[imgSlice * imgBase->getDimx() * imgBase->getDimy()]
        );
}

void Window::setImg(Img *img, float lowLimit, float highLimit, int slice)
{
    imgBase = img;
    imgData =
        new unsigned long[img->getDimx() * img->getDimy() * img->getDimz()];
    imgSlice = slice;
    imgLowLimit = lowLimit;
    imgHighLimit = highLimit;
    slider->setRange(0, imgBase->getDimz() - 1);
    slider->setSingleStep(1);
    slider->setValue(60);
    readImgData();
    imgLoaded = true;
}

