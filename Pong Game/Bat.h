#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bat
{
private:
    Vector2f m_Position;
    RectangleShape m_Shape;
    float m_Speed;
    float m_width;
    float m_height;
    bool m_MovingRight = false;
    bool m_MovingLeft = false;

    Vector2f m_screenSize;

public:
    // Bat();
    Bat(float, float, Vector2f);  // dimension with default position
    Bat(Vector2f &);
    Bat(float,float,float,float, Vector2f); //dimension and position
    FloatRect getPosition();
    RectangleShape & getShape();
    float getSpeed();
    float setSpeed(float);
    void moveLeft();
    void stopLeft();
    void moveRight();
    void stopRight();
    void update(Time);
};