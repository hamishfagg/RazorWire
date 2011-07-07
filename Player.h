#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <deque>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "Object.h"

enum control_list {W, S, A, D, Up, Down, Left, Right};


class Camera : public Object {
public:
    Camera(Point, Point);
private:
};

class Player : public Object {
public:
    Player(std::deque<Point>::reverse_iterator);   // Constructor - Sets up geometry
    std::deque<Point>::reverse_iterator tp;        // Closest track point - Need tp for my bunghole

    float trackDist(std::deque<Point>::reverse_iterator);
    void Render(float);
    double score;
    // Controls (Follows control_list order)
    sf::Key::Code GetControls(control_list);
    sf::Key::Code controls[8];
private:

};
#endif // PLAYER_H_INCLUDED
