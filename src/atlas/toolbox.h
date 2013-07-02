#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QPoint>

enum ToolType { TOOLBOX_SELECT, TOOLBOX_POLYGON, TOOLBOX_SAMPLE };
enum ToolState { TOOL_IDLE, TOOL_START, TOOL_DEFINE, TOOL_END };

class ToolBox
{
public:
    ToolBox();

    void setType(ToolType type) { toolType = type; }
    void setState(ToolState state) { toolState = state; }
    void setPos(int x, int y);

    ToolType getType() { return toolType; }
    ToolState getState() { return toolState; }
    void getCurrLine(QPoint *p0, QPoint *p1);

private:
    ToolType toolType;
    ToolState toolState;
    int xInitPos, yInitPos;
    int xCurrPos, yCurrPos;
};

#endif

