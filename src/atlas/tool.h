#ifndef TOOL_H
#define TOOL_H

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
          toolState(state) { }

    void setType(ToolType type) { toolType = type; toolState = STATE_IDLE; }
    void setState(ToolState state) { toolState = state; }

    ToolType getType() { return toolType; }
    ToolState getState() { return toolState; }

    virtual void mouseDown(int x, int y) = 0;
    virtual void mouseMove(int x, int y) = 0;
    virtual void mouseUp(int x, int y) = 0;
    virtual void mouseDoubleClick(int x, int y) = 0;
    virtual void draw() { }

protected:
    ToolType toolType;
    ToolState toolState;
};

#endif

