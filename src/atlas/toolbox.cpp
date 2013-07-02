#include "toolbox.h"

ToolBox::ToolBox()
    : toolType(TOOLBOX_SELECT),
      toolState(TOOL_IDLE)
{
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
        case TOOL_END:
            xCurrPos = x;
            yCurrPos = y;
            break;

        default:
            break;
    }
}

void ToolBox::getCurrLine(QPoint *p0, QPoint *p1)
{
    p0->setX(xInitPos);
    p0->setY(yInitPos);
    p1->setX(xCurrPos);
    p1->setY(yCurrPos);
}

