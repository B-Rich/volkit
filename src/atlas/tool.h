#ifndef TOOL_H
#define TOOL_H

#include "img/line2.h"

class Tool
{
public:
    enum ToolType
    {
        TYPE_SELECT,
        TYPE_POLYGON,
        TYPE_SAMPLE
    };

    enum ToolState
    {
        STATE_IDLE,
        STATE_START,
        STATE_DEFINE,
        STATE_END,
        STATE_CONTINUE,
        STATE_DONE
    };

    Tool(ToolType type, ToolState state)
        : toolType(type),
          toolState(state),
          xScale(1.0),
          yScale(1.0) { }

    void setType(ToolType type) { toolType = type; toolState = STATE_IDLE; }
    void setState(ToolState state) { toolState = state; }
    void setScale(float x, float y) { xScale = x; yScale = y; }

    void set(Point2 &p, int x, int y)
    {
        p.set(float(x) / xScale, float(y) / yScale);
    }

    ToolType getType() { return toolType; }
    ToolState getState() { return toolState; }

    int x(Point2 &p) { return int(p.x() * xScale); }
    int y(Point2 &p) { return int(p.y() * yScale); }
    int x1(Line2 &l) { return int(l.x1() * xScale); }
    int y1(Line2 &l) { return int(l.y1() * yScale); }
    int x2(Line2 &l) { return int(l.x2() * xScale); }
    int y2(Line2 &l) { return int(l.y2() * yScale); }

    virtual void mouseDown(int x, int y) = 0;
    virtual void mouseMove(int x, int y) = 0;
    virtual void mouseUp(int x, int y) = 0;
    virtual void mouseDoubleClick(int x, int y) = 0;
    virtual void draw() { }

protected:

    ToolType toolType;
    ToolState toolState;
    float xScale, yScale;
};

#endif

