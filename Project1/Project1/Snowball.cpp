#include "Snowball.h"

Snowball::Snowball(sf::Vector2f pos, float dirX)
{
    m_facingRight = (dirX > 0.f);
    m_velocity.x = dirX * SNOWBALL_SPEED;
    m_velocity.y = 0.f;

    m_shape.setSize({ 16.f, 16.f });
    m_shape.setOrigin(8.f, 8.f);
    m_shape.setPosition(pos);
    m_shape.setFillColor(sf::Color(200, 230, 255));
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(1.f);
}

void Snowball::update(float dt)
{
    m_shape.move(m_velocity * dt);

    // Deactivate if it leaves the screen horizontally
    float x = m_shape.getPosition().x;
    if (x < 0.f || x > static_cast<float>(WINDOW_W))
        deactivate();
}

void Snowball::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}