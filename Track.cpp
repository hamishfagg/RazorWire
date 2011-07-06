#include "Track.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <time.h>

Track::Track() : lastRot( {0,0})
{
    track.push_front(Point(0,0,0));
    //track.push_front(Point(0,1000,-0.3));

    SEGMENT_LENGTH = 1;
}

void Track::GenChunk(int len)
{
    for (int i = 0; i < len; i++)
    {
        GenCurve();
    }
}

void Track::GenCurve()
{
    //generate a random modifier
    float randP = ((float)rand()/(float)RAND_MAX * 0.1f) - 0.05f;
    float randY = ((float)rand()/(float)RAND_MAX * 0.1f) - 0.05f;

    for (int i = 0; i < 50; i++)
    {
        lastRot[0] += randP; //Pitch
        lastRot[1] += randY; //Yaw

        float temp = SEGMENT_LENGTH * cos(lastRot[0]);

        track.push_front(Point(track.front().x + SEGMENT_LENGTH * sin(lastRot[1]),
                               track.front().y + SEGMENT_LENGTH * cos(lastRot[1]),
                               track.front().z + SEGMENT_LENGTH * sin(lastRot[0])));
        //std::cout << track.front().x << " " << track.front().y << " " << track.front().z << std::endl;
    }
    //std::cout << "Generated: " << randP << " " << randY << std::endl;
}

void Track::ExtendTrack(Point extendTo)
{
    track.push_front(extendTo);
}

std::deque<Point>::reverse_iterator Track::getStart()
{
    return track.rbegin();
}

void Track::Render(float elapsedTime, float radius)
{

    GLUquadricObj *quadratic=gluNewQuadric();

    // LINE-BASED WORKING CODE
    /*glLineWidth(5.0);
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    for (std::deque<Point>::iterator i = track.begin(); i+1 != track.end(); i++)
    {
        glVertex3f(i->x, i->y, i->z);
        glVertex3f((i+1)->x, (i+1)->y, (i+1)->z);
    }
    glEnd();*/

    //glPushMatrix(); // Save the old matrix
    glBegin(GL_QUADS);
    float fat = 0.5;

    //glPushMatrix();
    for (std::deque<Point>::iterator i = track.begin(); i+1 != track.end(); i++)
    {
        //glLoadIdentity();
        glPushMatrix();
        glTranslatef(i->x, i->y, i->z);

        gluSphere(quadratic, radius,4,4);
        /*glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();


        gluLookAt(i->x, i->y, i->z, (i+1)->x, (i+1)->y, (i+1)->z, 0,0,1);
        //glPushMatrix();
        //gluLookAt(i->x, i->y, i->z, 0, 0, 0, 0,1,0);

        glVertex3f(-fat + i->x, -fat + i->y, 0 + i->z);
        glVertex3f(-fat + i->x, fat+ i->y, 0+ i->z);
        glVertex3f(-fat + (i+1)->x, -fat + (i+1)->y, SEGMENT_LENGTH + (i+1)->z);
        glVertex3f(-fat + (i+1)->x, fat + (i+1)->y, SEGMENT_LENGTH + (i+1)->z);

        glVertex3f(fat, -fat, 0);
        glVertex3f(fat, fat, 0);
        glVertex3f(fat, -fat, SEGMENT_LENGTH);
        glVertex3f(fat, fat, SEGMENT_LENGTH);

        glVertex3f(-fat, fat, 0);
        glVertex3f(fat, fat, 0);
        glVertex3f(-fat, fat, SEGMENT_LENGTH);
        glVertex3f(fat, fat, SEGMENT_LENGTH);

        glVertex3f(-fat, -fat, 0);
        glVertex3f(fat, -fat, 0);
        glVertex3f(-fat, -fat, SEGMENT_LENGTH);
        glVertex3f(fat, -fat, SEGMENT_LENGTH);*/
        glPopMatrix();
    }
    glEnd();
    //glPopMatrix();

    //glPopMatrix();
}

Point Track::GetAvg()
{
    Point ret;
    ret = Point(0, 0, 0);
    for (std::deque<Point>::iterator i = track.begin(); i != track.end(); i++)
    {
        ret.x += i->x;
        ret.y += i->y;
        ret.z += i->z;
    }
    ret.x = ret.x/track.size();
    ret.y = ret.y/track.size();
    ret.z = ret.z/track.size();

    return ret;
}

float Track::GetMaxDist(Point center)
{
    float max = 0;
    float temp;
    for (std::deque<Point>::iterator i = track.begin(); i != track.end(); i++)
    {
        temp = sqrt(pow((center.x - i->x), 2) + pow((center.y - i->y), 2) + pow((center.z - i->z), 2));
        if (temp > max)
        {
            max = temp;
        }
    }
    return max;
}
