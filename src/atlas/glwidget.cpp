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
    setMouseTracking(true);
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

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    currTool->mouseDown(event->pos().x(), event->pos().y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    currTool->mouseMove(event->pos().x(), event->pos().y());
    repaint();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    currTool->mouseMove(event->pos().x(), event->pos().y());
    repaint();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    currTool->mouseDoubleClick(event->pos().x(), event->pos().y());
    currTool->setType(TOOLBOX_SELECT);
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

    if (currTool->getType() == TOOLBOX_POLYGON)
    {
        glColor3f(1.0, 1.0, 1.0);
        QPolygon polygon = currTool->getPolygon();
        glBegin(GL_LINES);
        for (int i = 0; i < polygon.size(); i++)
        {
            QPoint p = polygon.at(i);
            glVertex2i(p.x(), p.y());
        }
        glEnd();

        if (currTool->getState() == Tool::STATE_DEFINE)
        {
            QLine line;
            currTool->getCurrLine(line);
            glBegin(GL_LINES);
                glVertex2i(line.x1(), line.y1());
                glVertex2i(line.x2(), line.y2());
            glEnd();
        }
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

