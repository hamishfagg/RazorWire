#include "Player.h"
#include <math.h>


Player::Player(std::deque<Point>::reverse_iterator tp) : controls({sf::Key::W,
                             sf::Key::S,
                             sf::Key::A,
                             sf::Key::D,
                             sf::Key::Up,
                             sf::Key::Down,
                             sf::Key::Left,
                             sf::Key::Right,}),
                             tp(tp)
{
    score = 0;
    geo_size = 3;
    geo = new Poly[geo_size];
    /*
    geo[0] = Point(50, 0, 0);
    geo[1] = Point(0, 50, 0);
    geo[2] = Point(0, 0, 0);
    */
    geo[0] = Poly(Point(-0.75, -1, 0),
                  Point(0, 1, 0),
                  Point(0, -0.5, 0.75),
                  Point(-1, 0, 0),
                  0, 1, 1, 1);
    geo[1] = Poly(Point(0.75, -1, 0),
                  Point(0, 1, 0),
                  Point(0, -0.5, 0.75),
                  Point(1, 0, 0),
                  0, 1, 1, 1);
    geo[2] = Poly(Point(-0.75, -1, 0),
                  Point(0.75, -1, 0),
                  Point(0, -0.5, 0.75),
                  Point(0, -1, 0),
                  1, 1, 0, 1);
}
float Player::trackDist(std::deque<Point>::reverse_iterator trackp) {
    // The smallest distance to the track piece provided by the iterator.
    return sqrt(pow((trackp->x - r.x),2) + pow((trackp->y - r.y),2) + pow((trackp->z - r.z),2));
}

void Player::Render(float elapsedTime) {
    Object::Render(elapsedTime);
    while ( trackDist(tp+1) < trackDist(tp)) {
        //std::cout << "In while" << std::endl;
        double temp = 10 - trackDist(tp);
        if (temp < 0)
        {
            temp = 0;
        }
        score += temp*temp;
        tp++;
    }
}

Camera::Camera(Point pos, Point rot) {
    r = pos;
    rr = rot;
}
