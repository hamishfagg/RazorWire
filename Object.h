#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>

class Point {
public:
    Point();
    Point(float, float, float);
    float &operator[](const int);
    Point &operator=(const Point &);
    float x, y, z;

    friend Point &operator*(const Point &, const Point &);
    friend Point &operator*(const Point &, const float &);
    friend std::ostream & operator<<(std::ostream &, const Point &);
};

class Poly {
//Probably not the most optimised solution.
public:
    Poly();
    Poly(Point, Point, Point, Point, float, float, float, float);
    void Render();
private:
    Point points[3];
    float col[4];
    Point normal;
};

class Object
{
// A generic object that is affected by physics.
public:
    Object();
    Point toWorld(Point);
    Point GetVector();
    Point GetHeading();
    Point aroundAxis(Point, Point, float);
    Point GetUpVector();
    virtual void Render(float);   // Renders the geometry at the current local coords.

    Point a;      // Accel, vel, pos
    Point v;
    Point r;
    Point ra;    // Rotational accel, vel, pos
    Point rv;
    Point rr;       //x = pitch
    Point forward;
    Point left;
    Point up;
    int geo_size;
    Poly *geo;

    GLfloat max[16], rex[16];
};

#endif // OBJECT_H_INCLUDED
