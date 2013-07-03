#include <iostream>

#include <GL/gl.h>

#include "polytool.h"

Polytool::Polytool()
    : Tool(TYPE_SELECT, STATE_IDLE),
      polygonDefined(false)
{
    currPolygon = new QPolygon;
}

void Polytool::mouseDown(int x, int y)
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

void Polytool::mouseMove(int x, int y)
{
    if (getState() == STATE_DEFINE)
    {
        setPos(x, y);
    }
}

void Polytool::mouseUp(int x, int y)
{
    setState(STATE_END);
    setPos(x, y);
    setState(STATE_START);
    setPos(x, y);
    setState(STATE_DEFINE);
}

void Polytool::mouseDoubleClick(int x, int y)
{
    setState(STATE_END);
    setPos(x, y);
    setState(STATE_IDLE);
}

void Polytool::setState(ToolState state)
{
    if (state != toolState)
    {
        if (toolType == TYPE_POLYGON &&
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

void Polytool::setPos(int x, int y)
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
            (*currPolygon) << QPoint(xInitPos, yInitPos)
                           << QPoint(xCurrPos, yCurrPos);
            break;

        case STATE_CONTINUE:
            xInitPos = xLastPos;
            yInitPos = yLastPos;
            break;

        default:
            break;
    }
}

void Polytool::getCurrLine(QLine &line)
{
    line.setLine(xInitPos, yInitPos, xCurrPos, yCurrPos);
}

void Polytool::draw()
{
    if (getType() == Tool::TYPE_POLYGON)
    {
        glColor3f(1.0, 1.0, 1.0);
        QPolygon polygon = getPolygon();
        glBegin(GL_LINES);
        for (int i = 0; i < polygon.size(); i++)
        {
            QPoint p = polygon.at(i);
            glVertex2i(p.x(), p.y());
        }
        glEnd();

        if (getState() == Tool::STATE_DEFINE)
        {
            QLine line;
            getCurrLine(line);
            glBegin(GL_LINES);
                glVertex2i(line.x1(), line.y1());
                glVertex2i(line.x2(), line.y2());
            glEnd();
        }
    }
}

