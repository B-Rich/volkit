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

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("Atlas"));
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
    readImgData();
    imgLoaded = true;
}

