#include <QtGui>
#include <QtOpenGL>

#include <iostream>
#include <math.h>
#include <string.h>

#include "glwidget.h"

GLWidget::GLWidget(ToolBox *tool, QWidget *parent)
    : currTool(tool),
      QGLWidget(parent),
      dataSet(false),
      updateState(true),
      xDataZoom(1.0),
      yDataZoom(1.0)
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

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    currTool->setState(TOOL_START);
    currTool->setPos(event->pos().x(), event->pos().y());
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    currTool->setState(TOOL_DEFINE);
    currTool->setPos(event->pos().x(), event->pos().y());
    repaint();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    currTool->setState(TOOL_END);
    currTool->setPos(event->pos().x(), event->pos().y());
}

void GLWidget::calculateDataZoom(int w, int h)
{
    xDataZoom = float(w) / float(dataWidth);
    yDataZoom = float(h) / float(dataHeight);
    updateState = true;
}

void GLWidget::setData(int w, int h, void *data)
{
    dataWidth = w;
    dataHeight = h;
    dataPtr = data;
    dataSet = true;
    calculateDataZoom(width(), height());
    repaint();
}

void GLWidget::unsetData()
{
    dataSet = false;
    repaint();
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
            glRasterPos2i(0, int(yDataZoom * float(dataHeight)));
            glPixelZoom(xDataZoom, yDataZoom);
            updateState = false;
        }
        glDrawPixels(dataWidth, dataHeight, GL_RGBA, GL_UNSIGNED_BYTE, dataPtr);
    }

    if (currTool->getType() == TOOLBOX_POLYGON &&
        currTool->getState() == TOOL_DEFINE)
    {
        QPoint start, end;
        currTool->getCurrLine(&start, &end);
        glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2i(start.x(), start.y());
            glVertex2i(end.x(), end.y());
        glEnd();
    }
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    calculateDataZoom(w, h);
}

