#pragma once
#include "Entity.h"
#include "Constants.h"

// ─── Rolling snowball (enemy fully covered → pushed by player) ─
class RollingSnowball : public Entity
{
public:
    RollingSnowball(sf::Vector2f pos, float dirX);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    void applyGravity(float dt);
    void resolveFloor(float floorY);

    bool isOnGround() const { return m_onGround; }

private:
    bool  m_onGround{ false };
    float m_radius{ 18.f };
};