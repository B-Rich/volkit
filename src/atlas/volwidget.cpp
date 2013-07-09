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
    glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void VolWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (dataSet)
    {
        if (updateState)
        {
            // TODO: It seems all textures cannot fit in memory
            updateState = false;
        }

        int sliceSize = dataWidth * dataHeight;
        uint32_t *ptr = (uint32_t *) dataPtr;
        for (int i = dataDepth - 2; i >= 0; i--)
        {
            GLuint id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0,
                         GL_RGBA, dataWidth, dataHeight, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, &ptr[i * sliceSize]);

            float z = -1.0 + 2.0 * float(i) / dataDepth;
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(-1.0, -1.0, z);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.0, -1.0, z);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.0, 1.0, z);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-1.0, 1.0, z);
            glEnd();

            glDeleteTextures(1, &id);
        }
    }
}

void VolWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

