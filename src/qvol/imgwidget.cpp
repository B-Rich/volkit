#include <QtGui>
#include <QtOpenGL>

#include <iostream>
#include <math.h>
#include <string.h>

#include "tool.h"
#include "imgwidget.h"

ImgWidget::ImgWidget(Tool *tool, QWidget *parent)
    : currTool(tool),
      QGLWidget(parent),
      dataState(DATA_STATE_EMPTY),
      textureFilter(TEXTURE_FILTER_LINEAR),
      parentWidget(parent)
{
    setMouseTracking(true);
}

ImgWidget::~ImgWidget()
{
    makeCurrent();
}

QSize ImgWidget::minimumSizeHint() const
{
    return QSize(256, 256);
}

QSize ImgWidget::sizeHint() const
{
    return QSize(512, 512);
}

void ImgWidget::mousePressEvent(QMouseEvent *event)
{
    currTool->mouseDown(event->pos().x(), event->pos().y());
}

void ImgWidget::mouseMoveEvent(QMouseEvent *event)
{
    currTool->mouseMove(event->pos().x(), event->pos().y());
    repaint();
}

void ImgWidget::mouseReleaseEvent(QMouseEvent *event)
{
    currTool->mouseMove(event->pos().x(), event->pos().y());
    repaint();
}

void ImgWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    currTool->mouseDoubleClick(event->pos().x(), event->pos().y());
    currTool->setType(Tool::TYPE_SELECT);
}

void ImgWidget::setTool(Tool *tool)
{
    currTool = tool;
    currTool->setScale(float(width()), float(height()));
}

void ImgWidget::setData(int w, int h, void *data)
{
    dataWidth = w;
    dataHeight = h;
    dataPtr = data;
    dataState = DATA_STATE_SET;
    currTool->setScale(float(width()), float(height()));
    repaint();
}

void ImgWidget::initDataGL()
{
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    if (textureFilter ==  TEXTURE_FILTER_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else if (textureFilter ==  TEXTURE_FILTER_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexImage2D(GL_TEXTURE_2D, 0,
                 GL_RGBA, dataWidth, dataHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, dataPtr);
    glBindTexture(GL_TEXTURE_2D, 0);
    dataState = DATA_STATE_READY;
}

void ImgWidget::unsetData()
{
    if (dataState == DATA_STATE_READY)
    {
        glDeleteTextures(1, &texId);
    }
    dataState = DATA_STATE_EMPTY;
    repaint();
}

void ImgWidget::initializeGL()
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

void ImgWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (dataState == DATA_STATE_SET)
    {
        initDataGL();
    }
    if (dataState == DATA_STATE_READY)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2i(0, 0);
            glTexCoord2f(1.0, 0.0);
            glVertex2i(width(), 0);
            glTexCoord2f(1.0, 1.0);
            glVertex2i(width(), height());
            glTexCoord2f(0.0, 1.0);
            glVertex2i(0, height());
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
    currTool->draw();
}

void ImgWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    currTool->setScale(float(w), float(h));
}

