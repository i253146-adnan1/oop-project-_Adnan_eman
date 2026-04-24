#pragma once
#include <SFML/Graphics.hpp>

// ─── Abstract base for every game object ──────────────────
class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

    bool isActive() const { return m_active; }
    void deactivate() { m_active = false; }

    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
    void setPosition(sf::Vector2f pos) { m_shape.setPosition(pos); }

protected:
    sf::RectangleShape m_shape;
    sf::Vector2f       m_velocity{ 0.f, 0.f };
    bool               m_active{ true };
};