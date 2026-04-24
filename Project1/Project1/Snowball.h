#pragma once
#include "Entity.h"
#include "Constants.h"

// ─── Snowball fired by the player ─────────────────────────
class Snowball : public Entity
{
public:
    Snowball(sf::Vector2f pos, float dirX);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    bool isFacingRight() const { return m_facingRight; }

private:
    bool m_facingRight;
};