#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace sf;
using namespace std;

enum class Side
{
    LEFT,
    RIGHT,
    NONE
};
const int NUM_BRANCHES = 6;
Sprite spriteBranches[NUM_BRANCHES];
Side branchesPosition[NUM_BRANCHES];

void updateBranchesPosition(int);

int main()
{
    VideoMode vm(1024, 768);
    RenderWindow window(vm, "Timber Game", Style::Fullscreen);
    Vector2u screenSize = window.getSize();
    cout << "Resolution x: " << screenSize.x << " y: " << screenSize.y << endl;

    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);

    Side playerSide = Side::LEFT;
    const int player_L = 515;
    const int player_R = 1260;
    const int axe_L = 660;
    const int axe_R = 1110;

    bool paused = true, gameOver = true, inAction = false;

    // load Background
    Texture textrureBackground;
    textrureBackground.loadFromFile("graphics/background.png");
    // Sprite spriteBackground;
    // spriteBackground.setTexture(textrureBackground);
    Sprite spriteBackground(textrureBackground);

    // load tree
    Texture textrureTree;
    textrureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree(textrureTree);
    spriteTree.setPosition(810, 0);

    // load bee
    Texture textrureBee;
    textrureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee(textrureBee);
    spriteBee.setPosition(view.getSize().x - 60, 900);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    // load clouds
    Texture textrureCloud;
    textrureCloud.loadFromFile("graphics/cloud.png");

    // cloud1
    Sprite spriteCloud1(textrureCloud);
    spriteCloud1.setPosition(0, 0);
    // cloud2
    Sprite spriteCloud2(textrureCloud);
    spriteCloud2.setPosition(0, 150);
    // cloud3
    Sprite spriteCloud3(textrureCloud);
    spriteCloud3.setPosition(0, 300);
    bool cloud1Active = false, cloud2Active = false, cloud3Active = false;
    float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;

    Text scoreText, messageText, pauseText, gameOverText;
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    scoreText.setFont(font);
    scoreText.setCharacterSize(100);
    scoreText.setColor(Color::Red);
    scoreText.setString("Score: 0");
    scoreText.setPosition(20, 20);

    messageText.setFont(font);
    messageText.setCharacterSize(75);
    messageText.setColor(Color::White);
    messageText.setString("Press Enter to start");
    FloatRect rect = messageText.getLocalBounds();
    messageText.setOrigin(rect.width / 2, rect.height / 2);
    messageText.setPosition(view.getSize().x / 2, view.getSize().y / 2);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(75);
    gameOverText.setColor(Color::White);
    gameOverText.setString("");
    gameOverText.setPosition(view.getSize().x / 2, view.getSize().y / 2 - 70);

    pauseText.setFont(font);
    pauseText.setCharacterSize(75);
    pauseText.setColor(Color::White);
    pauseText.setString("Press 'P' to pause/unpause");
    rect = pauseText.getLocalBounds();
    pauseText.setOrigin(rect.width / 2, rect.height / 2);
    pauseText.setPosition(view.getSize().x / 2, view.getSize().y / 2);

    float maxWidthTimeBar = 400;
    float elapsedTime = 6.0f;
    float widthPerSeconds = maxWidthTimeBar / elapsedTime;

    RectangleShape timeBar(Vector2f(maxWidthTimeBar, 50));
    timeBar.setFillColor(Color::Green);
    timeBar.setPosition((view.getSize().x - maxWidthTimeBar) / 2, view.getSize().y - 20);

    // load Player
    Texture textrurePlayer;
    textrurePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer(textrurePlayer);
    spritePlayer.setPosition(player_R, 775);

    // load Axe
    Texture textrureAxe;
    textrureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe(textrureAxe);
    spriteAxe.setPosition(axe_R, 875);

    // load Axe
    Texture textrureRIP;
    textrureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP(textrureRIP);
    spriteRIP.setPosition(player_R, 817);

    // load branches
    Texture textrureBranch;
    textrureBranch.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        spriteBranches[i].setTexture(textrureBranch);
        spriteBranches[i].setOrigin(220, 40);
        updateBranchesPosition(i);
    }

    // load log
    Texture textrureLog;
    textrureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog(textrureLog);

    spriteLog.setPosition(2000, 2000);
    bool logActive = false;
    float logSpeedX = 2000.0f;
    float logSpeedY = -1500.0f;

    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("audio/chop.wav");
    Sound chop(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("audio/death.wav");
    Sound death(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("audio/out_of_time.wav");
    Sound oot(ootBuffer);


    int score = 0;
    Clock clock;
    while (window.isOpen())
    {

        Event event;
        Time dt = clock.restart();
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && gameOver)
            {
                for (int i = 0; i < NUM_BRANCHES/2; i++)
                    branchesPosition[NUM_BRANCHES-i-1]=Side::NONE;
                score =0;
                spritePlayer.setPosition(player_R,spritePlayer.getPosition().y);
                paused = false;
                gameOver = false;
                elapsedTime = 6.0f;
                dt = clock.restart();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P && !gameOver)
            {
                paused = !paused;
            }
            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Left) && !paused)
            {
                score++;
                elapsedTime += (2.0f / score) + 0.15f;
                if (elapsedTime > 6.0f)
                    elapsedTime = 6.0f;
                updateBranchesPosition(score);
                spritePlayer.setPosition(player_L, spritePlayer.getPosition().y);
                spriteAxe.setPosition(axe_L, spriteAxe.getPosition().y);
                spriteRIP.setPosition(player_L, spriteRIP.getPosition().y);
                playerSide = Side::LEFT;
                spriteLog.setPosition(810, 780);
                logSpeedX = 3000.0f;
                logActive = true;
                chop.play();
            }
            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Right) && !paused)
            {
                score++;
                elapsedTime += (2.0f / score) + 0.15f;
                if (elapsedTime > 6.0f)
                    elapsedTime = 6.0f;
                updateBranchesPosition(score);
                spritePlayer.setPosition(player_R, spritePlayer.getPosition().y);
                spriteAxe.setPosition(axe_R, spriteAxe.getPosition().y);
                spriteRIP.setPosition(player_R, spriteRIP.getPosition().y);
                playerSide = Side::RIGHT;
                spriteLog.setPosition(810, 780);
                logSpeedX = -3000.0f;
                logActive = true;
                chop.play();
            }
            if (event.type == Event::KeyReleased)
            {
                spriteAxe.setPosition(3000, spriteAxe.getPosition().y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        // For Game purposes
        if (!paused)
        {
            elapsedTime -= dt.asSeconds();
            if (elapsedTime <= 0)
            {
                gameOver = true;
                paused = true;
                gameOverText.setString("Out of Time");
                FloatRect rect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(rect.width / 2, rect.height / 2);
                oot.play();
            }

            if(playerSide==branchesPosition[NUM_BRANCHES-1]){
                gameOver = true;
                paused =true;
                gameOverText.setString("SQUISHED...");
                FloatRect rect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(rect.width/2,rect.height/2);
                spritePlayer.setPosition(2500,spritePlayer.getPosition().y);
                death.play();
            }
        }

        // Update fRAMES;
        if (!paused)
        {
            // Movement of Bee
            if (!beeActive)
            {
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;
                srand((int)time(0) * 20);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                // bee will fly
                float x = spriteBee.getPosition().x - beeSpeed * dt.asSeconds();
                spriteBee.setPosition(x, spriteBee.getPosition().y);
                if (x < -100)
                    beeActive = false;
            }

            // Movement of cloud1
            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-100, height);
                cloud1Active = true;
            }
            else
            {
                // bee will fly
                float x = spriteCloud1.getPosition().x + cloud1Speed * dt.asSeconds();
                spriteCloud1.setPosition(x, spriteCloud1.getPosition().y);
                if (x > 2000)
                    cloud1Active = false;
            }

            // Movement of cloud2
            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200) + 200;
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                srand((int)time(0) * 20);
                float scale = ((rand() % 70) + 30) / 100.0;
                spriteCloud2.setScale(scale, scale);
                spriteCloud2.setPosition(-100, height);
                cloud2Active = true;
            }
            else
            {
                // bee will fly
                float x = spriteCloud2.getPosition().x + cloud2Speed * dt.asSeconds();
                spriteCloud2.setPosition(x, spriteCloud2.getPosition().y);
                if (x > 2000)
                    cloud2Active = false;
            }
            // Movement of cloud3
            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200) + 200;
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-100, height);
                cloud3Active = true;
            }
            else
            {
                // cloud3 will fly
                float x = spriteCloud3.getPosition().x + cloud3Speed * dt.asSeconds();
                spriteCloud3.setPosition(x, spriteCloud3.getPosition().y);
                if (x > 2000)
                    cloud3Active = false;
            }

            float widthTimeBar = (widthPerSeconds * elapsedTime);
            timeBar.setSize(Vector2f(widthTimeBar, 50));

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                int y_position = i * 150;
                if (branchesPosition[i] == Side::LEFT)
                {
                    spriteBranches[i].setPosition(600, y_position + 40);
                    spriteBranches[i].setRotation(180);
                }
                if (branchesPosition[i] == Side::RIGHT)
                {
                    spriteBranches[i].setPosition(1320, y_position + 40);
                    spriteBranches[i].setRotation(0);
                }
                if (branchesPosition[i] == Side::NONE)
                    spriteBranches[i].setPosition(3000, y_position + 40);
            }
            scoreText.setString("Score " + std::to_string(score));
            if(logActive){
                float newX = spriteLog.getPosition().x + logSpeedX*dt.asSeconds();
                float newY = spriteLog.getPosition().y + logSpeedY*dt.asSeconds();
                spriteLog.setPosition(newX,newY);
                if((newX > 2000 || newX < -100) && newY<-100)
                    logActive = false;
            }
        } // End of game area

        // Draw the window
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        if (!paused)
        {
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                window.draw(spriteBranches[i]);
            }
        }
        window.draw(spriteLog);

        window.draw(spriteBee);
        window.draw(scoreText);
        if (gameOver)
        {
            window.draw(gameOverText);
            window.draw(messageText);
        }
        if (paused && !gameOver)
            window.draw(pauseText);
        window.draw(timeBar);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        if (gameOver && elapsedTime>0.0f)
            window.draw(spriteRIP);
        window.display();
    }
    return 0;
}

void updateBranchesPosition(int seed)
{
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
        branchesPosition[i] = branchesPosition[i - 1];

    srand((int)(time(0))*seed);
    int position = rand() % 4;
    switch (position)
    {
    case 0:
        branchesPosition[0] = Side::LEFT;
        break;
    case 1:
        branchesPosition[0] = Side::RIGHT;
        break;
    default:
        branchesPosition[0] = Side::NONE;
        break;
    }
}
