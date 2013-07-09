#include <QtGui>
#include <QtOpenGL>

#include <iostream>
#include <math.h>
#include <string.h>

#include "volwidget.h"

VolWidget::VolWidget(QWidget *parent)
    : QGLWidget(parent),
      dataSet(false),
      updateState(true),
      parentWidget(parent)
{
}

VolWidget::~VolWidget()
{
    makeCurrent();
}

QSize VolWidget::minimumSizeHint() const
{
    return QSize(256, 256);
}

QSize VolWidget::sizeHint() const
{
    return QSize(512, 512);
}

void VolWidget::mousePressEvent(QMouseEvent *event)
{
}

void VolWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void VolWidget::mouseReleaseEvent(QMouseEvent *event)
{
    repaint();
}

void VolWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void VolWidget::setData(int w, int h, int d, void *data)
{
    dataWidth = w;
    dataHeight = h;
    dataDepth = d;
    dataPtr = data;
    dataSet = true;
    repaint();
}

void VolWidget::unsetData()
{
    dataSet = false;
    repaint();
}

void VolWidget::initializeGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glViewport(0, 0, width(), height()); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), height(), 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void VolWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (dataSet)
    {
        if (updateState)
        {
            updateState = false;
        }
    }
}

void VolWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

