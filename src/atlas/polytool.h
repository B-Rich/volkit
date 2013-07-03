#ifndef POLYTOOL_H
#define POLYTOOL_H

#include <QLine>
#include <QPolygon>

#include "tool.h"

class Polytool : public Tool
{
public:
    Polytool();

    void setType(ToolType type);
    void setState(ToolState state);

    void mouseDown(int x, int y);
    void mouseMove(int x, int y);
    void mouseUp(int x, int y);
    void mouseDoubleClick(int x, int y);
    void draw();

    void getCurrLine(QLine &line);
    QPolygon getPolygon() { return *currPolygon; }

protected:
    void setPos(int x, int y);

private:
    int xInitPos, yInitPos;
    int xCurrPos, yCurrPos;
    int xLastPos, yLastPos;

    bool polygonDefined;
    QPolygon *currPolygon;
};

#endif

