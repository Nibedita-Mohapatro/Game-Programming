#include"Ball.h"

Ball::Ball(float radius, Vector2f screenSize){
    m_screenSize = screenSize;
    m_radius = radius;
    m_Shape.setRadius(m_radius);
    m_Position.x = m_screenSize.x/2 - m_radius;
    m_Position.y = 30;
    m_Shape.setPosition(m_Position);
    m_initial_position = m_Position;
}  // dimension with default position
Ball::Ball(float radius,float posX,float posY, Vector2f screenSize){
    m_screenSize = screenSize;
    m_radius = radius;
    m_Shape.setRadius(m_radius);
    m_Position.x = posX;
    m_Position.y = posY;
    m_Shape.setPosition(m_Position);
    m_initial_position = m_Position;
} //dimension and position
FloatRect Ball::getPosition(){
    return m_Shape.getGlobalBounds();
}
CircleShape & Ball::getShape(){
    return m_Shape;
}
float Ball::getSpeed(){
    return m_Speed;
}
float Ball::setSpeed(float speed){
    m_Speed = speed;
}

void Ball::reboundTop(){
    m_directionY = -m_directionY;
}
void Ball::reboundSides(){
    m_directionX = -m_directionX;
}
void Ball::reboundBottom(){
    resetPosition(20);
}
void Ball::reboundByBat(){
    m_directionY = -m_directionY;
}

void Ball::resetPosition(int seed){
    m_Position = m_initial_position;
    m_Shape.setPosition(m_Position);
    srand(time(0)*seed);
    int val = rand()%5000;
    if(val%2){  // +ve X
        if(m_directionX<0)
            m_directionX = -m_directionX;
    }else{    // -ve X
        if(m_directionX>0)
            m_directionX = -m_directionX;
    }
    srand(time(0)*seed*2);
    val = rand()%5000;
    if(val%2){  // +ve Y
        if(m_directionY<0)
            m_directionY = -m_directionY;
    }else{    // -ve Y
        if(m_directionY>0)
            m_directionY = -m_directionY;
    }

}
void Ball::update(Time dt){
    m_Position.x += m_Speed*m_directionX*dt.asSeconds();
    m_Position.y += m_Speed*m_directionY*dt.asSeconds();
    m_Shape.setPosition(m_Position);
}