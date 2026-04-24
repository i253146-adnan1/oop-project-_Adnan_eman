#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Constants.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Snowball.h"
#include "RollingSnowball.h"
#include "HUD.h"

enum class GameState { Playing, LevelClear, GameOver };

// ─── Game – owns window, game-loop, and all game objects ──
class Game
{
public:
    Game();
    void run();

private:
    // ── Loop phases ────────────────────────────────────────
    void processEvents();
    void update(float dt);
    void render();

    // ── Physics / collision ────────────────────────────────
    void resolveEntityVsTiles(Entity& entity, sf::Vector2f& velocity,
        bool& onGround, sf::Vector2f size);

    void resolvePlayerVsTiles();
    void resolveEnemyVsTiles(Enemy& enemy);
    void resolveRollingVsTiles(RollingSnowball& ball);

    // ── Gameplay logic ─────────────────────────────────────
    void updateSnowballs(float dt);
    void updateEnemies(float dt);
    void updateRollingBalls(float dt);
    void checkPlayerEnemyCollision();
    void checkPlayerRollingCollision();
    void spawnInitialEnemies();

    // ── Overlay screens ────────────────────────────────────
    void drawLevelClear();
    void drawGameOver();

    // ── Members ────────────────────────────────────────────
    sf::RenderWindow m_window;
    sf::Clock        m_clock;
    GameState        m_state{ GameState::Playing };

    TileMap  m_tileMap;
    Player   m_player;
    HUD      m_hud;

    std::vector<std::unique_ptr<Enemy>>          m_enemies;
    std::vector<std::unique_ptr<Snowball>>        m_snowballs;
    std::vector<std::unique_ptr<RollingSnowball>> m_rollingBalls;

    float m_stateTimer{ 0.f };   // for level-clear / game-over delay

    // ── Overlay font (optional) ────────────────────────────
    sf::Font m_overlayFont;
    bool     m_overlayFontLoaded{ false };
};