#ifndef SAMPLETOOL_H
#define SAMPLETOOL_H

#include "img/point2.h"

#include "tool.h"
#include "window.h"

class QMainWindow;

class SampleTool : public Tool
{
public:
    SampleTool(QMainWindow *parent);

    void mouseDown(int x, int y);
    void mouseMove(int x, int y);
    void mouseUp(int x, int y);
    void mouseDoubleClick(int x, int y);
    void draw();

private:
    QMainWindow *parentWidget;
};

#endif

