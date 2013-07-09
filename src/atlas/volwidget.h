#ifndef VOLWIDGET_H
#define VOLWIDGET_H

#include <QGLWidget>

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

    void setData(int w, int h, int d, void *data);
    void unsetData();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    bool dataSet;
    int dataWidth, dataHeight, dataDepth;
    void *dataPtr;

    bool updateState;

    QWidget *parentWidget;
};

#endif

