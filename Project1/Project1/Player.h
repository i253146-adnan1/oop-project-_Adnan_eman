#pragma once
#include "Entity.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include "Snowball.h"

// ─── Player (Nick or Tom) ─────────────────────────────────
class Player : public Entity
{
public:
    explicit Player(sf::Vector2f startPos, sf::Color color = sf::Color(50, 120, 220));

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    void handleInput();

    // Physics helpers called by Game
    void applyGravity(float dt);
    void setOnGround(bool v, float groundY);
    void resolveWallLeft(float wallX);
    void resolveWallRight(float wallX);
    void resolveRoof(float roofY);

    // Called when hit by enemy
    void takeDamage();

    bool isAlive()           const { return m_hp > 0; }
    int  getHP()             const { return m_hp; }
    int  getScore()          const { return m_score; }
    void addScore(int pts) { m_score += pts; }

    bool isInvincible()      const { return m_invTimer > 0.f; }

    // Retrieve newly fired snowballs this frame
    std::vector<std::unique_ptr<Snowball>>& getPendingSnowballs() { return m_pending; }

    bool isFacingRight()     const { return m_facingRight; }

private:
    void drawHUD(sf::RenderWindow& window) const;

    int   m_hp{ PLAYER_MAX_HP };
    int   m_score{ 0 };
    bool  m_onGround{ false };
    bool  m_facingRight{ true };
    float m_invTimer{ 0.f };
    float m_shootTimer{ 0.f };

    sf::Color m_color;

    std::vector<std::unique_ptr<Snowball>> m_pending;

    sf::Font m_font; // loaded inline for HUD
};