#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "toolbox.h"

class GLWidget : public QGLWidget
{
public:
    GLWidget(ToolBox *tool, QWidget *parent = 0);
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

    ToolBox *currTool;

    bool dataSet;
    int dataWidth, dataHeight;
    void *dataPtr;

    bool updateState;
    float xDataZoom, yDataZoom;
};

#endif

