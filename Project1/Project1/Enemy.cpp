#include "Enemy.h"
#include "RollingSnowball.h"
#include <cmath>

Enemy::Enemy(sf::Vector2f pos)
{
    m_shape.setSize({ 28.f, 28.f });
    m_shape.setOrigin(14.f, 14.f);
    m_shape.setPosition(pos);
    updateColor();
}

void Enemy::update(float dt)
{
    if (m_state == EnemyState::Dead) return;

    // Flash timer
    if (m_flashTimer > 0.f) m_flashTimer -= dt;

    // Handle melt timer while covered
    if (m_state == EnemyState::SnowCovered1 ||
        m_state == EnemyState::SnowCovered2 ||
        m_state == EnemyState::SnowCovered3)
    {
        m_meltTimer -= dt;
        if (m_meltTimer <= 0.f)
            meltSnow();
        return;   // covered enemies don't walk
    }

    // Normal walking patrol
    m_shape.move({ m_dirX * ENEMY_SPEED * dt, 0.f });

    // Simple turn timer (enemy reverses every few seconds)
    m_turnTimer += dt;
    if (m_turnTimer > 2.5f)
    {
        m_dirX = -m_dirX;
        m_turnTimer = 0.f;
    }

    updateColor();
}

void Enemy::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);

    // Eyes (simple white circles drawn as small rects)
    if (m_state == EnemyState::Walking)
    {
        sf::RectangleShape eye({ 5.f, 5.f });
        eye.setFillColor(sf::Color::White);
        eye.setPosition(m_shape.getPosition() + sf::Vector2f(-8.f, -6.f));
        window.draw(eye);
        eye.setPosition(m_shape.getPosition() + sf::Vector2f(3.f, -6.f));
        window.draw(eye);
    }

    // Snow overlay layers
    if (m_state >= EnemyState::SnowCovered1)
    {
        sf::RectangleShape snow({ 28.f, 10.f });
        snow.setOrigin(14.f, 5.f);
        snow.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, 10.f));
        snow.setFillColor(sf::Color(210, 235, 255, 200));
        window.draw(snow);
    }
    if (m_state >= EnemyState::SnowCovered2)
    {
        sf::RectangleShape snow({ 28.f, 18.f });
        snow.setOrigin(14.f, 9.f);
        snow.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, 5.f));
        snow.setFillColor(sf::Color(220, 240, 255, 210));
        window.draw(snow);
    }
    if (m_state == EnemyState::SnowCovered3)
    {
        sf::RectangleShape snow({ 30.f, 30.f });
        snow.setOrigin(15.f, 15.f);
        snow.setPosition(m_shape.getPosition());
        snow.setFillColor(sf::Color(230, 245, 255, 230));
        snow.setOutlineColor(sf::Color::White);
        snow.setOutlineThickness(2.f);
        window.draw(snow);
    }
}

void Enemy::hitBySnow()
{
    m_flashTimer = 0.12f;
    switch (m_state)
    {
    case EnemyState::Walking:
        m_state = EnemyState::SnowCovered1;
        m_meltTimer = SNOWBALL_COVER_TIME;
        break;
    case EnemyState::SnowCovered1:
        m_state = EnemyState::SnowCovered2;
        m_meltTimer = SNOWBALL_COVER_TIME;
        break;
    case EnemyState::SnowCovered2:
        m_state = EnemyState::SnowCovered3;
        m_meltTimer = SNOWBALL_COVER_TIME;
        break;
    case EnemyState::SnowCovered3:
        // already fully covered – extra hits reset melt timer
        m_meltTimer = SNOWBALL_COVER_TIME;
        break;
    default: break;
    }
    updateColor();
}

void Enemy::meltSnow()
{
    if (m_state == EnemyState::SnowCovered3)
        m_state = EnemyState::SnowCovered2;
    else if (m_state == EnemyState::SnowCovered2)
        m_state = EnemyState::SnowCovered1;
    else if (m_state == EnemyState::SnowCovered1)
        m_state = EnemyState::Walking;

    if (m_state != EnemyState::Walking)
        m_meltTimer = SNOWBALL_COVER_TIME;

    updateColor();
}

std::unique_ptr<RollingSnowball> Enemy::kick(float dirX)
{
    m_state = EnemyState::Dead;
    deactivate();
    return std::make_unique<RollingSnowball>(m_shape.getPosition(), dirX);
}

void Enemy::updateColor()
{
    switch (m_state)
    {
    case EnemyState::Walking:
        m_shape.setFillColor(m_flashTimer > 0.f ? sf::Color(255, 180, 180) : sf::Color(180, 60, 60));
        break;
    case EnemyState::SnowCovered1:
        m_shape.setFillColor(sf::Color(160, 80, 80));
        break;
    case EnemyState::SnowCovered2:
        m_shape.setFillColor(sf::Color(140, 100, 120));
        break;
    case EnemyState::SnowCovered3:
        m_shape.setFillColor(sf::Color(100, 130, 180));
        break;
    default:
        m_shape.setFillColor(sf::Color::Transparent);
        break;
    }
}