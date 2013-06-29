#include <QtGui>
#include <QtOpenGL>

#include <iostream>
#include <math.h>
#include <string.h>

#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
      dataSet(false),
      updateState(false),
      dataZoom(1.0)
{
}

GLWidget::~GLWidget()
{
    makeCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(256, 256);
}

QSize GLWidget::sizeHint() const
{
    return QSize(512, 512);
}

void GLWidget::setData(int w, int h, void *data)
{
    dataWidth = w;
    dataHeight = h;
    dataPtr = data;
    dataSet = true;
    repaint();
}

void GLWidget::setDataZoom(float zoom)
{
    if (zoom != dataZoom)
    {
        dataZoom = zoom;
        updateState = true;
        repaint();
    }
}

void GLWidget::initializeGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glViewport(0, 0, width(), height()); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (dataSet)
    {
        if (updateState)
        {
            glRasterPos2i(0, int(dataZoom * float(dataHeight)));
            glPixelZoom(dataZoom, dataZoom);
            updateState = false;
        }
        glDrawPixels(dataWidth, dataHeight, GL_RGBA, GL_UNSIGNED_BYTE, dataPtr);
    }
}

