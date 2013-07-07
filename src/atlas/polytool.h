#ifndef POLYTOOL_H
#define POLYTOOL_H

#include <vector>

#include "img/line2.h"
#include "tool.h"

class PolyTool : public Tool
{
public:
    PolyTool();

    void setType(ToolType type);
    void setState(ToolState state);

    void mouseDown(int x, int y);
    void mouseMove(int x, int y);
    void mouseUp(int x, int y);
    void mouseDoubleClick(int x, int y);
    void draw();

protected:
    void setPos(int x, int y);

private:
    Point2 initPos;
    Point2 currPos;
    Point2 lastPos;

    bool polygonDefined;
    std::vector<Point2*> polygon;
};

#endif

