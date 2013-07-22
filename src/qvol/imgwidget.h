#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QGLWidget>

#include "tool.h"

class QWidget;

class ImgWidget : public QGLWidget
{
public:
    enum DataState { DATA_STATE_EMPTY, DATA_STATE_SET, DATA_STATE_READY };
    enum TextureFilter { TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_LINEAR };

    ImgWidget(Tool *tool, QWidget *parent);
    ~ImgWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void setTool(Tool *tool);

    void setData(int w, int h, void *data);
    void unsetData();

protected:
    void initDataGL();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    Tool *currTool;
    float xDataZoom, yDataZoom;

    DataState dataState;
    int dataWidth, dataHeight;
    void *dataPtr;
    GLuint texId;
    TextureFilter textureFilter;

    QWidget *parentWidget;
};

#endif

