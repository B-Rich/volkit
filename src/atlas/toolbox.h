#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QLine>

enum ToolType
{
    TOOLBOX_SELECT,
    TOOLBOX_POLYGON,
    TOOLBOX_SAMPLE
};

enum ToolState
{
    TOOL_IDLE,
    TOOL_START,
    TOOL_DEFINE,
    TOOL_END,
    TOOL_CONTINUE,
    TOOL_DONE
};

class ToolBox
{
public:
    ToolBox();

    void setType(ToolType type);
    void setState(ToolState state);
    void setPos(int x, int y);

    ToolType getType() { return toolType; }
    ToolState getState() { return toolState; }
    void getCurrLine(QLine &line);

private:
    ToolType toolType;
    ToolState toolState;
    int xInitPos, yInitPos;
    int xCurrPos, yCurrPos;
    int xLastPos, yLastPos;
};

#endif

