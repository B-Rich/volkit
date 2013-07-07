#include <QtGui>

#include <iostream>

#include "sampletool.h"

SampleTool::SampleTool(QMainWindow *parent)
    : Tool(TYPE_SAMPLE, STATE_IDLE),
      parentWidget(parent)
{
}

void SampleTool::mouseDown(int x, int y)
{
    Point2 p;
    set(p, x, y);

    QString str;
    QTextStream(&str) << "(" << p.x() << ", " << p.y() << ")";
    parentWidget->statusBar()->showMessage(str);
}

void SampleTool::mouseMove(int x, int y)
{
}

void SampleTool::mouseUp(int x, int y)
{
}

void SampleTool::mouseDoubleClick(int x, int y)
{
}

void SampleTool::draw()
{
}

