#include "Bat.h"

Bat::Bat(float width, float height, Vector2f screenSize)
{
    m_screenSize = screenSize;
    m_width = width;
    m_height = height;
    m_Shape.setSize(Vector2f(m_width,m_height));
    m_Position.x = (m_screenSize.x - m_width)/2;
    m_Position.y = m_screenSize.y-30;
    m_Shape.setPosition(m_Position);
}
Bat:: Bat(float width,float height,float posX,float posY, Vector2f screenSize){
    m_screenSize = screenSize;
    m_width = width;
    m_height = height;
    m_Shape.setSize(Vector2f(m_width,m_height));
    m_Position.x = posX;
    m_Position.y = posY;
    m_Shape.setPosition(m_Position);
}
Bat::Bat(Vector2f &pos)
{
    m_Position.x = pos.x;
    m_Position.y = pos.y;
    m_Shape.setPosition(m_Position);
}
FloatRect Bat::getPosition()
{
    return m_Shape.getGlobalBounds();
}
RectangleShape & Bat::getShape()
{
    return m_Shape;
}
float Bat::getSpeed()
{
    return m_Speed;
}
float Bat::setSpeed(float speed)
{
    m_Speed = speed;
}
void Bat::moveLeft()
{
    m_MovingLeft = true;
}
void Bat::stopLeft()
{    m_Shape.setPosition(m_Position);

    m_MovingLeft = false;
}
void Bat::moveRight()
{
    m_MovingRight = true;
}
void Bat::stopRight()
{
    m_MovingRight = false;
}
void Bat::update(Time dt)
{
    if (m_MovingLeft)
    {
        m_Position.x -= (dt.asSeconds() * m_Speed);
        if (m_Position.x < 0)
        {
            m_Position.x = 0;
            m_MovingLeft = false;
        }
    }
    if (m_MovingRight)
    {
        m_Position.x +=  (dt.asSeconds() * m_Speed);
        float rightPos = m_screenSize.x - m_Shape.getLocalBounds().width;
        if (m_Position.x > rightPos)
        {
            m_Position.x = rightPos;
            m_MovingRight = false;
        }
    }
    m_Shape.setPosition(m_Position);
}