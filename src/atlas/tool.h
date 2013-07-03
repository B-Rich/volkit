#ifndef TOOL_H
#define TOOL_H

class Tool
{
public:
    enum ToolState
    {
        STATE_IDLE,
        STATE_START,
        STATE_DEFINE,
        STATE_END,
        STATE_CONTINUE,
        STATE_DONE
    };

    virtual void mouseDown(int x, int y) = 0;
    virtual void mouseMove(int x, int y) = 0;
    virtual void mouseUp(int x, int y) = 0;
    virtual void mouseDoubleClick(int x, int y) = 0;
};

#endif

