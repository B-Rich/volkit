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

void VolWidget::addBrick(int w, int h, int d, uint32_t *data)
{
    ImgBrick *brick = new ImgBrick(w, h, d, data);
    bricks.push_back(brick);
}

void VolWidget::setBricks(int x, int y, int z)
{
    xBricks = x;
    yBricks = y;
    zBricks = z;
    dataSet = true;
    repaint();
}

void VolWidget::unsetData()
{
    if (dataSet)
    {
        for (int i = 0; i < bricks.size(); i++)
        {
            ImgBrick *brick = bricks[i];
            delete brick;
        }
        bricks.clear();

        dataSet = false;
        repaint();
    }
}

void VolWidget::initializeGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, width(), height()); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLuint VolWidget::genTexture(ImgBrick *brick, int slice)
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
                 GL_RGBA, brick->width, brick->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 &brick->data[slice * brick->width * brick->height]);

    return id;
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

        std::vector<ImgBrick*>::iterator it = bricks.begin();

        float z = -1.0, dz = 2.0 / zBricks;
        for (int zi = 0; zi < zBricks; zi++)
        {
            float y = 1.0, dy = 2.0 / yBricks;
            for (int yi = 0; yi < yBricks; yi++)
            {
                float x = -1.0, dx = 2.0 / xBricks;
                for (int xi = 0; xi < xBricks; xi++)
                {
                    ImgBrick *brick = *it;
                    ++it;
                    for (int i = brick->depth - 2; i >= 0; i--)
                    {
                        GLuint id = genTexture(brick, i);
                        float zSlice = 1.0 - 2.0 * float(i) / brick->depth;
                        glBegin(GL_QUADS);
                            glTexCoord2f(0.0, 0.0);
                            glVertex3f(x, y - dy, zSlice);
                            glTexCoord2f(1.0, 0.0);
                            glVertex3f(x + dx, y - dy, zSlice);
                            glTexCoord2f(1.0, 1.0);
                            glVertex3f(x + dx, y, zSlice);
                            glTexCoord2f(0.0, 1.0);
                            glVertex3f(x, y, zSlice);
                        glEnd();
                        glDeleteTextures(1, &id);
                    } // End for i
                    x += dx;
                } // End for xi
                y -= dy;
            } // End for yi
        } // End for zi
    }
}

void VolWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

