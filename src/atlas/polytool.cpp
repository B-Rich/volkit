#include <iostream>

#include <GL/gl.h>

#include "polytool.h"

Polytool::Polytool()
    : Tool(TYPE_POLYGON, STATE_IDLE),
      polygonDefined(false)
{
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
            lastPos.set(currPos);
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
            set(initPos, x, y);
            break;

        case STATE_DEFINE:
            set(currPos, x, y);
            break;

        case STATE_END:
            set(currPos, x, y);
            polygon.push_back(new Point2(initPos));
            polygon.push_back(new Point2(currPos));
            break;

        case STATE_CONTINUE:
            initPos.set(lastPos);
            break;

        default:
            break;
    }
}

void Polytool::draw()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (int i = 0; i < polygon.size(); i++)
    {
        Point2 *p = polygon[i];
        glVertex2i(x(*p), y(*p));
    }
    glEnd();

    if (getState() == Tool::STATE_DEFINE)
    {
        Line2 line(initPos, currPos);
        glBegin(GL_LINES);
            glVertex2i(x1(line), y1(line));
            glVertex2i(x2(line), y2(line));
        glEnd();
    }
}

