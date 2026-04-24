#include "RollingSnowball.h"

RollingSnowball::RollingSnowball(sf::Vector2f pos, float dirX)
{
    m_velocity.x = dirX * 320.f;
    m_velocity.y = 0.f;

    m_shape.setSize({ m_radius * 2.f, m_radius * 2.f });
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setPosition(pos);
    m_shape.setFillColor(sf::Color(160, 210, 255));
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2.f);
}

void RollingSnowball::applyGravity(float dt)
{
    if (!m_onGround)
        m_velocity.y += GRAVITY * dt;
}

void RollingSnowball::resolveFloor(float floorY)
{
    float bottom = m_shape.getPosition().y + m_radius;
    if (bottom >= floorY)
    {
        m_shape.setPosition({ m_shape.getPosition().x, floorY - m_radius });
        m_velocity.y = 0.f;
        m_onGround = true;
    }
    else
    {
        m_onGround = false;
    }
}

void RollingSnowball::update(float dt)
{
    m_shape.move(m_velocity * dt);

    // Bounce off side walls
    float x = m_shape.getPosition().x;
    if (x - m_radius < static_cast<float>(TILE_SIZE))
    {
        m_shape.setPosition({ static_cast<float>(TILE_SIZE) + m_radius, m_shape.getPosition().y });
        m_velocity.x = std::abs(m_velocity.x);
    }
    else if (x + m_radius > static_cast<float>(WINDOW_W - TILE_SIZE))
    {
        m_shape.setPosition({ static_cast<float>(WINDOW_W - TILE_SIZE) - m_radius, m_shape.getPosition().y });
        m_velocity.x = -std::abs(m_velocity.x);
    }
}

void RollingSnowball::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}