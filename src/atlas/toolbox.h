#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QLine>
#include <QPolygon>

#include "tool.h"

enum ToolType
{
    TOOLBOX_SELECT,
    TOOLBOX_POLYGON,
    TOOLBOX_SAMPLE
};

#if 0
enum ToolState
{
    TOOL_IDLE,
    TOOL_START,
    TOOL_DEFINE,
    TOOL_END,
    TOOL_CONTINUE,
    TOOL_DONE
};
#endif

class ToolBox : Tool
{
public:
    ToolBox();

    void setType(ToolType type);
    void setState(ToolState state);

    ToolType getType() { return toolType; }
    ToolState getState() { return toolState; }

    void mouseDown(int x, int y);
    void mouseMove(int x, int y);
    void mouseUp(int x, int y);
    void mouseDoubleClick(int x, int y);

    void getCurrLine(QLine &line);
    QPolygon getPolygon() { return *currPolygon; }

protected:
    void setPos(int x, int y);

private:
    ToolType toolType;
    ToolState toolState;
    int xInitPos, yInitPos;
    int xCurrPos, yCurrPos;
    int xLastPos, yLastPos;

    bool polygonDefined;
    QPolygon *currPolygon;
};

#endif

