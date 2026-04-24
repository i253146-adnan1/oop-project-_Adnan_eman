#pragma once
#include "Entity.h"
#include "Constants.h"
#include <memory>

class RollingSnowball;

// ─── Snow cover state ─────────────────────────────────────
enum class EnemyState
{
    Walking,          // normal patrol
    SnowCovered1,     // lightly covered
    SnowCovered2,     // half covered
    SnowCovered3,     // fully covered – can be kicked
    Dead
};

// ─── Enemy (Bigfoot-like goon from Snow Bros level 1) ──────
class Enemy : public Entity
{
public:
    explicit Enemy(sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    // Called by a player snowball hit
    void hitBySnow();

    // Called when timer runs out while covered
    void meltSnow();

    bool isFullyCovered()  const { return m_state == EnemyState::SnowCovered3; }
    bool isAlive()         const { return m_state != EnemyState::Dead; }

    EnemyState getState()  const { return m_state; }

    // Creates rolling ball; caller takes ownership
    std::unique_ptr<RollingSnowball> kick(float dirX);

private:
    void updateColor();

    EnemyState m_state{ EnemyState::Walking };
    float      m_meltTimer{ 0.f };
    float      m_dirX{ 1.f };            // patrol direction
    float      m_turnTimer{ 0.f };
    bool       m_onGround{ false };

    // Visual hit flash
    float      m_flashTimer{ 0.f };
};