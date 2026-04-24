#include "Player.h"
#include <cmath>

Player::Player(sf::Vector2f startPos, sf::Color color)
    : m_color(color)
{
    m_shape.setSize({ 28.f, 34.f });
    m_shape.setOrigin(14.f, 17.f);
    m_shape.setPosition(startPos);
    m_shape.setFillColor(color);
}

void Player::handleInput()
{
    float vx = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        vx -= PLAYER_SPEED;
        m_facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        vx += PLAYER_SPEED;
        m_facingRight = true;
    }

    m_velocity.x = vx;

    // Jump
    if (m_onGround &&
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Z)))
    {
        m_velocity.y = JUMP_VELOCITY;
        m_onGround = false;
    }

    // Shoot snowball
    m_shootTimer -= 0.016f; // approx; proper dt applied in update
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && m_shootTimer <= 0.f)
    {
        float dir = m_facingRight ? 1.f : -1.f;
        sf::Vector2f spawnPos = m_shape.getPosition() + sf::Vector2f(dir * 20.f, 0.f);
        m_pending.push_back(std::make_unique<Snowball>(spawnPos, dir));
        m_shootTimer = SHOOT_COOLDOWN;
    }
}

void Player::applyGravity(float dt)
{
    if (!m_onGround)
        m_velocity.y += GRAVITY * dt;
}

void Player::setOnGround(bool v, float groundY)
{
    m_onGround = v;
    if (v)
    {
        m_shape.setPosition({ m_shape.getPosition().x, groundY - 17.f });
        if (m_velocity.y > 0.f) m_velocity.y = 0.f;
    }
}

void Player::resolveWallLeft(float wallX)
{
    m_shape.setPosition({ wallX + 14.f, m_shape.getPosition().y });
    m_velocity.x = 0.f;
}

void Player::resolveWallRight(float wallX)
{
    m_shape.setPosition({ wallX - 14.f, m_shape.getPosition().y });
    m_velocity.x = 0.f;
}

void Player::resolveRoof(float roofY)
{
    m_shape.setPosition({ m_shape.getPosition().x, roofY + 17.f });
    m_velocity.y = 0.f;
}

void Player::update(float dt)
{
    if (m_invTimer > 0.f) m_invTimer -= dt;
    if (m_shootTimer > 0.f) m_shootTimer -= dt;

    m_shape.move(m_velocity * dt);

    // Invincibility flicker
    if (m_invTimer > 0.f)
    {
        bool visible = (static_cast<int>(m_invTimer * 10.f) % 2 == 0);
        m_shape.setFillColor(visible ? m_color : sf::Color::Transparent);
    }
    else
    {
        m_shape.setFillColor(m_color);
    }
}

void Player::takeDamage()
{
    if (m_invTimer > 0.f) return;
    --m_hp;
    m_invTimer = PLAYER_INV_TIME;
    m_velocity.y = JUMP_VELOCITY * 0.6f; // knock-back bounce
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);

    // Simple face detail
    if (m_invTimer <= 0.f || (static_cast<int>(m_invTimer * 10.f) % 2 == 0))
    {
        sf::RectangleShape eye({ 5.f, 5.f });
        eye.setFillColor(sf::Color::White);
        sf::Vector2f p = m_shape.getPosition();
        eye.setPosition(p + sf::Vector2f(-8.f, -8.f));
        window.draw(eye);
        eye.setPosition(p + sf::Vector2f(3.f, -8.f));
        window.draw(eye);
    }
}