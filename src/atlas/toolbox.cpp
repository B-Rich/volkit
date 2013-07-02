#include <iostream>

#include "toolbox.h"

ToolBox::ToolBox()
    : toolType(TOOLBOX_SELECT),
      toolState(TOOL_IDLE),
      polygonDefined(false)
{
}

void ToolBox::setType(ToolType type)
{
    if (type != toolType)
    {
        if (type == TOOLBOX_POLYGON)
        {
            currPolygon = new QPolygon;
        }
        toolState = TOOL_IDLE;
        toolType = type;
    }
}

void ToolBox::setState(ToolState state)
{
    if (state != toolState)
    {
        if (toolType == TOOLBOX_POLYGON &&
            toolState == TOOL_END)
        {
            xLastPos = xCurrPos;
            yLastPos = yCurrPos;
            toolState = TOOL_CONTINUE;
        }
        else
        {
            toolState = state;
        }
    }
}

void ToolBox::setPos(int x, int y)
{
    switch(toolState)
    {
        case TOOL_START:
            xInitPos = x;
            yInitPos = y;
            break;

        case TOOL_DEFINE:
            xCurrPos = x;
            yCurrPos = y;
            break;

        case TOOL_END:
            xCurrPos = x;
            yCurrPos = y;
            if (toolType == TOOLBOX_POLYGON)
            {
                (*currPolygon) << QPoint(xInitPos, yInitPos)
                               << QPoint(xCurrPos, yCurrPos);
            }
            break;

        case TOOL_CONTINUE:
            xInitPos = xLastPos;
            yInitPos = yLastPos;
            break;

        default:
            break;
    }
}

void ToolBox::getCurrLine(QLine &line)
{
    line.setLine(xInitPos, yInitPos, xCurrPos, yCurrPos);
}

