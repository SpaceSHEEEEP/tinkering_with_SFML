#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

// CONSTANTS
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;
const float BALL_RADIUS = 10.0f;

sf::Keyboard::Scan escapeKey = sf::Keyboard::Scan::Escape;

float randFloat(float min, float max)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// CLASSES
class Ball
{
    sf::CircleShape m_ball;
    float m_x{0};
    float m_y{0};
    float m_dx{randFloat(-100.0f, 100.0f)};
    float m_dy{randFloat(-100.0f, 100.0f)};
    float m_radius{randFloat(10.0f, 20.0f)};

public:
    void updateBall(float dt)
    {
        m_ball.move({m_dx * dt, m_dy * dt});
        m_x += m_dx * dt;
        m_y += m_dy * dt;

        // check window borders
        if (m_y + m_radius*2 > WINDOW_HEIGHT) // bottom
        {
            m_ball.setPosition({m_x, WINDOW_HEIGHT - 2*m_radius - 1}); // -1 for some buffer
            m_dy *= -1;
            m_y = WINDOW_HEIGHT - 2*m_radius - 1;
        }
        else if (m_y < 0) // top
        {
            m_ball.setPosition({m_x, 1}); 
            m_dy *= -1;
            m_y = 1;
        }
        if (m_x + m_radius*2 > WINDOW_WIDTH) // right
        {
            m_ball.setPosition({WINDOW_WIDTH - 2*m_radius - 1, m_y}); // -1 for some buffer
            m_dx *= -1;
            m_x = WINDOW_WIDTH - 2*m_radius - 1;
        }
        else if (m_x < 0) // left
        {
            m_ball.setPosition({1, m_y}); 
            m_dx *= -1;
            m_x = 1;
        }
    }

    void drawBall(sf::RenderWindow & window)
    {
        window.draw(m_ball);
    }

    Ball(float xin, float yin)
        : m_x{xin}, m_y{yin} 
        {
            m_ball.setPosition({xin, yin});
            m_ball.setFillColor(sf::Color(randFloat(0,255), randFloat(0,255), randFloat(0,255)));
            m_ball.setRadius(m_radius);
        };
};

class BallMachine
{
    std::vector<Ball> m_balls{};

public:
    void updateBalls(float dt) // i should assign each ball their own velocity
    {
        for (Ball & b : m_balls)
        {
            b.updateBall(dt);
        }
    }

    void drawBalls(sf::RenderWindow & window)
    {
        for (Ball & b : m_balls)
        {
            b.drawBall(window);
        }
    }

    void addBall(sf::Vector2i mousePos)
    {
        float x_temp = mousePos.x;
        float y_temp = mousePos.y;
        m_balls.push_back(Ball(x_temp, y_temp));
        
        std::cout << "ball added at (" << x_temp << ", " << y_temp << ")" << '\n';
        std::cout << "we have " << m_balls.size() << " now " << '\n';
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Bouncing Balls");
    window.setFramerateLimit(60);
    sf::Clock clock;
    BallMachine ballMachine;

    while (window.isOpen())
    {
        sf::Time timeSinceLastFrame = clock.restart();
        float dt = timeSinceLastFrame.asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
        }
        if (sf::Keyboard::isKeyPressed(escapeKey)) window.close();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            ballMachine.addBall(sf::Mouse::getPosition(window));
        }

        ballMachine.updateBalls(dt);

        window.clear();
        ballMachine.drawBalls(window);
        window.display();
    }
}