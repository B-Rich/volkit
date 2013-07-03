#include <iostream>

#include "toolbox.h"

ToolBox::ToolBox()
    : toolType(TOOLBOX_SELECT),
      toolState(STATE_IDLE),
      polygonDefined(false)
{
}

void ToolBox::mouseDown(int x, int y)
{
    if (getState() == STATE_DEFINE)
    {
        setState(STATE_END);
        setPos(x, y);
    }
    setState(STATE_START);
    setPos(x, y);
    setState(STATE_DEFINE);
}

void ToolBox::mouseMove(int x, int y)
{
    if (getState() == STATE_DEFINE)
    {
        setPos(x, y);
    }
}

void ToolBox::mouseUp(int x, int y)
{
    setState(STATE_END);
    setPos(x, y);
    setState(STATE_START);
    setPos(x, y);
    setState(STATE_DEFINE);
}

void ToolBox::mouseDoubleClick(int x, int y)
{
    setState(STATE_END);
    setPos(x, y);
    setState(STATE_IDLE);
}

void ToolBox::setType(ToolType type)
{
    if (type != toolType)
    {
        if (type == TOOLBOX_POLYGON)
        {
            currPolygon = new QPolygon;
        }
        toolState = STATE_IDLE;
        toolType = type;
    }
}

void ToolBox::setState(ToolState state)
{
    if (state != toolState)
    {
        if (toolType == TOOLBOX_POLYGON &&
            toolState == STATE_END)
        {
            xLastPos = xCurrPos;
            yLastPos = yCurrPos;
            toolState = STATE_CONTINUE;
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
        case STATE_START:
            xInitPos = x;
            yInitPos = y;
            break;

        case STATE_DEFINE:
            xCurrPos = x;
            yCurrPos = y;
            break;

        case STATE_END:
            xCurrPos = x;
            yCurrPos = y;
            if (toolType == TOOLBOX_POLYGON)
            {
                (*currPolygon) << QPoint(xInitPos, yInitPos)
                               << QPoint(xCurrPos, yCurrPos);
            }
            break;

        case STATE_CONTINUE:
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

