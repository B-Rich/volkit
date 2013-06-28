#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSlider;
class GLWidget;

class Window : public QWidget
{

public:
    Window();

private:
    GLWidget *glWidget;
};

#endif

