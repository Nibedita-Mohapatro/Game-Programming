#include <SFML/Graphics.hpp>
#include "Bat.cpp"
#include "Ball.cpp"
using namespace sf;

const int MAXLIVES = 3;
const int WINSCORE = 10; 
int main()
{
    VideoMode vm(1024, 768);
    RenderWindow window(vm, "Pong Game", Style::Fullscreen);
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);
    Vector2f screenSize = view.getSize();

    float batHeight = 10, batWidth = 300;
    float batSpeed = 700;

    // Get object of bat
    Bat bat1(batWidth, batHeight, screenSize);
    bat1.getShape().setFillColor(Color::White);
    bat1.setSpeed(batSpeed);

    Bat bat2(batWidth, batHeight,(screenSize.x-batWidth)/2,30, screenSize);
    bat2.getShape().setFillColor(Color::Green);
    bat2.setSpeed(batSpeed);

    // Get object of Ball
    float ballRadius = 15;
    float ball_Speed = 1000;
    Ball ball(ballRadius,screenSize.x/2-ballRadius,screenSize.y/2-ballRadius, screenSize);
    ball.getShape().setFillColor(Color::White);
    ball.setSpeed(ball_Speed);

    bool paused = false;
    bool gameOver = false;
    int p1Score = 0, p2Score=0;
    int lives = 3;

    Font font;
    font.loadFromFile("fonts/DS-DIGI.TTF");

    Text scoreP1Text;
    scoreP1Text.setFont(font);
    scoreP1Text.setCharacterSize(75);
    scoreP1Text.setFillColor(Color::White);
    scoreP1Text.setPosition(20, screenSize.y-100);
    scoreP1Text.setString("Player 1: 0");

    Text scoreP2Text;
    scoreP2Text.setFont(font);
    scoreP2Text.setCharacterSize(75);
    scoreP2Text.setFillColor(Color::White);
    scoreP2Text.setPosition(20, 20);
    scoreP2Text.setString("Player 2: 0");

    Font font2;
    font2.loadFromFile("fonts/Chicago.ttf");
    Text messageText;
    messageText.setFont(font2);
    messageText.setCharacterSize(75);
    messageText.setFillColor(Color::White);
    messageText.setString("Game Over, Press Enter to restart...");
    messageText.setOrigin(messageText.getGlobalBounds().width / 2, messageText.getGlobalBounds().height / 2);
    messageText.setPosition(screenSize.x / 2, screenSize.y / 2);

    Texture textureLives;
    textureLives.loadFromFile("graphics/heart.png");

    Sprite spriteLives[MAXLIVES];
    for (int i = 0; i < MAXLIVES; i++)
    {
        spriteLives[i].setTexture(textureLives);
        spriteLives[i].setScale(0.1, 0.1);
        float xPos = screenSize.x - 20 - (MAXLIVES - i) * 64;
        spriteLives[i].setPosition(xPos, 40);
    }

    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(75);
    livesText.setFillColor(Color::White);
    livesText.setString("Lives: ");
    float xPos = screenSize.x - 10 - (MAXLIVES * 64) - livesText.getGlobalBounds().width;
    livesText.setPosition(xPos, 20);

    Clock clock;
    while (window.isOpen())
    {
        Event event;
        Time dt;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
            {
                ball.resetPosition(p1Score+p2Score);
                p1Score = 0;
                p2Score = 0;
                // lives = 3;
                gameOver = false;
                paused = false;
                dt=clock.restart();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        if (Keyboard::isKeyPressed(Keyboard::Left))
            bat1.moveLeft();
        else
            bat1.stopLeft();
        if (Keyboard::isKeyPressed(Keyboard::Right))
            bat1.moveRight();
        else
            bat1.stopRight();

        //For Bat 2

        if (Keyboard::isKeyPressed(Keyboard::A))
            bat2.moveLeft();
        else
            bat2.stopLeft();
        if (Keyboard::isKeyPressed(Keyboard::D))
            bat2.moveRight();
        else
            bat2.stopRight();


        // Update Frame
        if (!paused)
        {
            dt = clock.restart();
            bat1.update(dt);
            bat2.update(dt);
            ball.update(dt);

            if (ball.getPosition().left <= 0 || (ball.getPosition().left + ball.getPosition().width) >= screenSize.x)
                ball.reboundSides();
            if (ball.getPosition().top <= 0)
            {
                p1Score++;
                ball.resetPosition(p1Score+p2Score);
            }
            if (ball.getPosition().top >= screenSize.y)
            {
                p2Score++;
                ball.resetPosition(p1Score+p2Score);
            }
            if (ball.getPosition().intersects(bat1.getPosition()))
            {
                ball.reboundByBat();
            }
            if (ball.getPosition().intersects(bat2.getPosition()))
            {
                ball.reboundByBat();
            }
            scoreP1Text.setString("Player 1: " + std::to_string(p1Score));
            scoreP2Text.setString("Player 2: " + std::to_string(p2Score));
            if(p1Score>=WINSCORE){
                messageText.setString("Game Over, Player 1 Wins...");
                messageText.setOrigin(messageText.getGlobalBounds().width / 2, messageText.getGlobalBounds().height / 2);
                messageText.setPosition(screenSize.x / 2, screenSize.y / 2);
                gameOver = true;
                paused = true;
            }
            if(p2Score>=WINSCORE){
                messageText.setString("Game Over, Player 2 Wins...");
                messageText.setOrigin(messageText.getGlobalBounds().width / 2, messageText.getGlobalBounds().height / 2);
                messageText.setPosition(screenSize.x / 2, screenSize.y / 2);
                gameOver = true;
                paused = true;
            }
        }

        // Draw Part
        window.clear();
        window.draw(bat1.getShape());
        window.draw(bat2.getShape());
        window.draw(ball.getShape());
        window.draw(scoreP1Text);
        window.draw(scoreP2Text);
        // window.draw(livesText);
        // for (int i = 0; i < lives; i++)
        // {
        //     window.draw(spriteLives[i]);
        // }
        if (gameOver)
            window.draw(messageText);

        window.display();
    }
}