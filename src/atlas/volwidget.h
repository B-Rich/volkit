#ifndef VOLWIDGET_H
#define VOLWIDGET_H

#include <QGLWidget>

#include "imgbrick.h"
#include <vector>

class QWidget;

class VolWidget : public QGLWidget
{
public:
    VolWidget(QWidget *parent);
    ~VolWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void addBrick(int w, int h, int d, uint32_t *data);
    void setBricks(int x, int y, int z);
    void unsetData();
   
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    bool dataSet;
    int dataWidth, dataHeight, dataDepth;
    void *dataPtr;

    int xBricks, yBricks, zBricks;
    std::vector<ImgBrick*> bricks;

    bool updateState;

    QWidget *parentWidget;

    GLuint genTexture(ImgBrick *brick, int slice);
    void drawBrick(
        float x, float y, float z,
        float dx, float dy, float dz,
        ImgBrick *brick
        );

    GLuint genTexture3D(ImgBrick *brick);
    void drawBrick3D(
        float x, float y, float z,
        float dx, float dy, float dz,
        ImgBrick *brick
        );
};

#endif

