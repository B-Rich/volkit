#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QGLWidget>

#include "tool.h"

class QWidget;

class ImgWidget : public QGLWidget
{
public:
    ImgWidget(Tool *tool, QWidget *parent);
    ~ImgWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void setData(int w, int h, void *data);
    void unsetData();

    void setTool(Tool *tool);

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

