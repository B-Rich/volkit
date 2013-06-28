#include <QtGui>

#include <iostream>
#include <string.h>

#include "glwidget.h"
#include "window.h"

Window::Window() :
    imgLoaded(false)
{
    glWidget = new GLWidget;

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("Atlas"));
}

void Window::setImg(Img *img, float lowLimit, float highLimit, int slice)
{
    imgBase = img;
    imgData = new long[img->getDimx() * img->getDimy() * img->getDimz()];
    img->getRGBAData(imgData, lowLimit, highLimit);
    currSlice = slice;
    imgLoaded = true;
}

void Window::paintEvent(QPaintEvent *e)
{
    if (imgLoaded)
    {
        glWidget->setData(
            imgBase->getDimx(), imgBase->getDimy(),
            &imgData[currSlice * imgBase->getDimx() * imgBase->getDimy()]
            );
    }

    QWidget::paintEvent(e);
}

