#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED
#include <queue>
#include <SFML/Window.hpp>

#include "Object.h"


class Track : public Object {
public:
    Track();
    void GenChunk(int);
    void Render(float, float);
    void GenCurve();
    Point GetAvg();
    float GetMaxDist(Point);
    std::deque<Point>::reverse_iterator getStart();
private:
    void ExtendTrack(Point);

    float SEGMENT_LENGTH;
    std::deque<Point> track;
    float lastRot[2];
};

#endif // TRACK_H_INCLUDED
