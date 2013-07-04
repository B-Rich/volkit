#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "tool.h"

class QWidget;

class GLWidget : public QGLWidget
{
public:
    GLWidget(Tool *tool, QWidget *parent);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void setData(int w, int h, void *data);
    void unsetData();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    void calculateDataZoom(int w, int h);

    Tool *currTool;

    bool dataSet;
    int dataWidth, dataHeight;
    void *dataPtr;

    bool updateState;
    float xDataZoom, yDataZoom;

    QWidget *parentWidget;
};

#endif

