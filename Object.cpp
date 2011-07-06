#include <SFML/Window.hpp>

#include <iostream>
#include <cstring>
#include <math.h>
#include <iomanip>
#include "Object.h"

#define PI 3.14159265

Point::Point() : x(0), y(0), z(0) {}
Point::Point(float x, float y, float z) : x(x), y(y), z(z)
{
}
float & Point::operator[](const int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        std::cerr << "Error - Called invalid index " << i << " on point." << std::endl;
        exit(EXIT_FAILURE);
    }
}
Point & Point::operator=(const Point &n) {
    for (int i = 0; i<3; ++i) {
        //Hacks
        x = n.x;
        y = n.y;
        z = n.z;
    }
    return *this;
}

Point & operator*(const Point &p1, const Point &p2) {
    Point res;
    res.x = p1.x * p2.x;
    res.y = p1.y * p2.y;
    res.z = p1.z * p2.z;
    return res;
}

Point & operator*(const Point &p, const float &f) {
    Point res;
    res.x = p.x * f;
    res.y = p.y * f;
    res.z = p.z * f;
    return res;
}

Poly::Poly() {}
Poly::Poly (Point p1, Point p2, Point p3, Point norm, float f1, float f2, float f3, float f4) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    normal = norm;
    col[0] = f1;
    col[1] = f2;
    col[2] = f3;
    col[3] = f4;
}
std::ostream &operator<<(std::ostream & out, const Point &p) {
    out << "(" << p.x << "," << p.y << "," << p.z << ")";
    return out;
}

void Poly::Render() {
    // Draws a polygon at LOCAL coords.
    // You'll need to set glRotate/glTranslate previously to render at world coords.
    glColor4f(col[0], col[1], col[2], col[3]);

    glBegin(GL_TRIANGLES);  // Draw the polygon
    for (int i = 0; i < 3; ++i) {
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f( points[i].x, points[i].y, points[i].z);
    }
    glEnd();
}

Object::Object() : max({
                       1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1
                       }),
                   rex({
                       1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1
                       }),
                  forward(Point(0, 1, 0)),
                  left(Point(1, 0, 0)),
                  up(Point(0, 0, 1))
{

}

Point Object::aroundAxis(Point v, Point a, float r) {
    //Rotates vector v around axis a.
    float q0 = cos(r/2),  q1 = sin(r/2) * a.x,  q2 = sin(r/2) * a.y,  q3 = sin(r/2) * a.z;
    float q0s = q0*q0, q1s = q1*q1, q2s = q2*q2, q3s = q3*q3;
    float q[9] = {
        (q0s + q1s - q2s - q3s),  2*(q1*q2 - q0*q3),          2*(q1*q3 + q0*q2),
        2*(q2*q1 + q0*q3),        (q0s - q1s + q2s - q3s),    2*(q2*q3 - q0*q1),
        2*(q3*q1 - q0*q2),        2*(q3*q2 + q0*q1),          (q0s - q1s - q2s + q3s)
                 };
    Point res;
    res.x = (q[0] * v.x) + (q[1] * v.y) + (q[2] * v.z);
    res.y = (q[3] * v.x) + (q[4] * v.y) + (q[5] * v.z);
    res.z = (q[6] * v.x) + (q[7] * v.y) + (q[8] * v.z);

    return res;
}

Point Object::GetHeading() {
// Returns the vector (0,1,0) twisted to the current object direction.

    float v[4] = {0,1,0,1};
    float vr[4];
    //float rot[16] = max;
    //float rot[16] = {}
    vr[0] = (max[0]  * v[0]) + (max[1]  * v[1]) + (max[2]  * v[2]) + (max[3]  * v[3]);
    vr[1] = (max[4]  * v[0]) + (max[5]  * v[1]) + (max[6]  * v[2]) + (max[7]  * v[3]);
    vr[2] = (max[8]  * v[0]) + (max[9]  * v[1]) + (max[10] * v[2]) + (max[11] * v[3]);
    vr[3] = (max[12] * v[0]) + (max[13] * v[1]) + (max[14] * v[2]) + (max[15] * v[3]);

    for (int i =0; i<16; ++i) {
        if ((i % 4) == 0) std::cout << std::endl;
        std::cout << max[i] << " ";
    }
    std::cout << std::endl << "-------"<< std::endl;

    //return Point(vr[0], vr[1], vr[2], vr[3]);
    return Point(-vr[0], vr[1], -vr[2]);
}

Point Object::GetVector()
{
    /*float temp = cos(rr.x);
    Point localX = aroundAxis(Point(1, 0, 0), Point(0, 0, 1), rr.z);
    Point localY = aroundAxis()
    Point pitch = aroundAxis(Point(0, 1, 0), localX, rr.x);
    Point localZ = aroundAxis(Point(0, 1, 0), localX, rr.x + PI/2);
    Point vec = aroundAxis(pitch, localZ, -rr.z);
    std::cout << "Local Z: " << localZ << std::endl;


    //return Point(temp*sin(rr.z), temp*cos(rr.z), sin(rr.x));
    return vec;*/
    return Point(0, 0, 0);
}
Point Object::GetUpVector()
{
    Point forwardVec = GetVector();
    float temp = cos(rr.x + PI/2);

    return aroundAxis(Point(temp*sin(rr.z), temp*cos(rr.z), sin(rr.x + PI/2)), forwardVec, -rr.y);
}

void Object::Render(float elapsedTime)
{
    /*Point pFor = GetVector();
    Point pUp = GetUpVector();

    Point localX1 = aroundAxis(Point(1, 0, 0), Point(0, 0, 1), -rr.z);
    Point localY1 = aroundAxis(Point(1, 0, 0), Point(0, 0, 1), -rr.z + PI/2);

    Point localY = aroundAxis(localY1, localX1, rr.x);
    Point localZ = aroundAxis(localY, localX1, PI/2);
    Point localX = aroundAxis(Point(1, 0, 0), localZ, -rr.z);*/

    //Add pitch
    forward = aroundAxis(forward, left, rv.x*elapsedTime);
    up = aroundAxis(up, left, rv.x*elapsedTime);

    //Add Yaw
    forward = aroundAxis(forward, up, -rv.z*elapsedTime);
    left = aroundAxis(left, up, -rv.z*elapsedTime);

    //Add Roll
    left = aroundAxis(left, forward, -rv.y*elapsedTime);
    up = aroundAxis(up, forward, -rv.y*elapsedTime);


    glLineWidth(2.0);
    glBegin(GL_LINES);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(forward.x*3, forward.y*3, forward.z*3);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(left.x*3, left.y*3, left.z*3);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(up.x*3, up.y*3, up.z*3);

    glEnd();


    glTranslatef(r.x, r.y, r.z);
    glPushMatrix(); // Save the old matrix

    glLoadMatrixf(max);

    glRotatef(rv.x * 180/PI * elapsedTime, 1, 0, 0);  //Pitch
    glRotatef(-rv.z * 180/PI * elapsedTime, 0, 0, 1); //Yaw
    glRotatef(-rv.y * 180/PI * elapsedTime, 0, 1, 0); //Roll

    glGetFloatv(GL_MODELVIEW_MATRIX, max);
    glPopMatrix();
    glMultMatrixf(max);
    //glMultMatrixd()


    for (int i = 0; i < geo_size; ++i)
    {
        geo[i].Render();
    }

    // OLD SHIT
    //glLoadIdentity();
    // Go to the correct model position


    /*Point np;
    np = toWorld(Point(0,0,1));
    std::cout << np << std::endl;
    glRotatef(rr.z, np.x, np.y, np.z); //Yaw
    np = toWorld(Point(0,1,0));
    glRotatef(rr.y, np.x, np.y, np.z); //Roll*/
    /*glLoadIdentity();
    float x = sin(rr.z);
    float y = cos(rr.z);
    float z = cos(rr.x);

    gluLookAt(r.x, r.y, r.z,
              x, y, z,
              0, 0, 1);*/



    /*  PRE-POLY CLASS CODE
    glBegin(primitive);
    for (int i = 0; i < geo_size; ++i)
    {
        //glVertex3f( geo[i].x + r.x, geo[i].y +r.y, geo[i].z + r.z);
        glVertex3f( geo[i].x, geo[i].y, geo[i].z);
    }
    glEnd();
    */
    for (int i = 0; i < 3; i++)
    {
        v[i] += a[i] * elapsedTime;
        r[i] += v[i] * elapsedTime;
        rv[i] += ra[i] * elapsedTime;
        rr[i] += rv[i] * elapsedTime;
    }

    glPopMatrix();  // Restore the old matrix
}
