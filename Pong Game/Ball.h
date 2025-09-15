#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Ball
{
private:
    Vector2f m_Position;
    Vector2f m_initial_position;
    CircleShape m_Shape;
    float m_Speed;
    float m_radius;
    float m_directionX=0.5f;
    float m_directionY=0.5f;

    Vector2f m_screenSize;

public:
    Ball(float, Vector2f);  // dimension with default position
    Ball(float,float,float, Vector2f); //dimension and position
    FloatRect getPosition();
    CircleShape & getShape();
    float getSpeed();
    float setSpeed(float);

    void reboundTop();
    void reboundSides();
    void reboundBottom();
    void reboundByBat();

    void resetPosition(int);
    void update(Time);
};