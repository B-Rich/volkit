#ifndef LINE2_H
#define LINE2_H

#include "volimg/point2.h"

class Line2
{
public:

    Line2() { }
    Line2(float x1, float y1, float x2, float y2) { set(x1, y1, x2, y2); }
    Line2(Point2 p1, Point2 p2) { set(p1, p2); }

    void set(float x1, float y1, float x2, float y2)
    {
        p1Line.set(x1, y1);
        p2Line.set(x2, y2);
    }

    void set(Point2 p1, Point2 p2)
    {
        p1Line.set(p1);
        p2Line.set(p2);
    }

    float x1() { return p1Line.x(); }
    float y1() { return p1Line.y(); }
    float x2() { return p2Line.x(); }
    float y2() { return p2Line.y(); }

private:
    Point2 p1Line, p2Line;
};

#endif

