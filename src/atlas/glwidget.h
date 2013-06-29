#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setData(int w, int h, void *data);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    bool dataSet;
    int dataWidth, dataHeight;
    void *dataPtr;

    bool updateState;
    float xDataZoom, yDataZoom;
};

#endif

