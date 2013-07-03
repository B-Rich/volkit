#ifndef POINT2
#define POINT2

class Point2
{
public:
    Point2() { }
    Point2(float x, float y) { set(x, y); }
    Point2(Point2 &p) { set(p.xPoint, p.yPoint); }

    void set(float x, float y) { xPoint = x; yPoint = y; }
    void set(Point2 &p) { xPoint = p.xPoint; yPoint = p.yPoint; }

    float x() { return xPoint; }
    float y() { return yPoint; }

private:
    float xPoint, yPoint;
};

#endif

