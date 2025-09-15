#include <SFML/Graphics.hpp>
#include "Bat.cpp"
#include "Ball.cpp"
using namespace sf;

const int MAXLIVES = 3;

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
    Bat bat(batWidth, batHeight, screenSize);
    bat.getShape().setFillColor(Color::White);
    bat.setSpeed(batSpeed);

    // Get object of Ball
    float ballRadius = 15;
    float ball_Speed = 1000;
    Ball ball(ballRadius, screenSize);
    ball.getShape().setFillColor(Color::White);
    ball.setSpeed(ball_Speed);

    bool paused = false;
    bool gameOver = false;
    int score = 0;
    int lives = 3;

    Font font;
    font.loadFromFile("fonts/DS-DIGI.TTF");

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(75);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, 20);
    scoreText.setString("Score: 0");

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
                score = 0;
                lives = 3;
                gameOver = false;
                paused = false;
                ball.resetPosition();
                dt=clock.restart();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        if (Keyboard::isKeyPressed(Keyboard::Left))
            bat.moveLeft();
        else
            bat.stopLeft();
        if (Keyboard::isKeyPressed(Keyboard::Right))
            bat.moveRight();
        else
            bat.stopRight();


        // Update Frame
        if (!paused)
        {
            dt = clock.restart();
            bat.update(dt);
            ball.update(dt);

            if (ball.getPosition().left <= 0 || (ball.getPosition().left + ball.getPosition().width) >= screenSize.x)
                ball.reboundSides();
            if (ball.getPosition().top <= 0)
            {
                ball.reboundTop();
                score++;
                ball.getShape().setPosition(ball.getPosition().left, 0.01f);
                // ball.getShape().setPosition(ball.getShape().getPosition().x,0.01f);
            }
            if (ball.getPosition().top >= screenSize.y)
            {
                lives--;
                ball.reboundBottom();
                if (lives <= 0)
                {
                    gameOver = true;
                    paused = true;
                }
            }
            if (ball.getPosition().intersects(bat.getPosition()))
            {
                ball.reboundByBat();
            }
            scoreText.setString("Score: " + std::to_string(score));
        }

        // Draw Part
        window.clear();
        window.draw(bat.getShape());
        window.draw(ball.getShape());
        window.draw(scoreText);
        window.draw(livesText);
        for (int i = 0; i < lives; i++)
        {
            window.draw(spriteLives[i]);
        }
        if (gameOver)
            window.draw(messageText);

        window.display();
    }
}