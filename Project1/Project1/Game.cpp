#include "Game.h"
#include <algorithm>
#include <cmath>

Game::Game()
    : m_window(sf::VideoMode(WINDOW_W, WINDOW_H), "Snow Bros - Level 1",
        sf::Style::Close | sf::Style::Titlebar)
    , m_player({ static_cast<float>(WINDOW_W / 2), static_cast<float>(WINDOW_H - 80) })
{
    m_window.setFramerateLimit(static_cast<unsigned>(FPS));
    m_overlayFontLoaded = false; // no font needed
    spawnInitialEnemies();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event{};
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                m_window.close();

            if (m_state != GameState::Playing &&
                event.key.code == sf::Keyboard::Return)
            {
                m_player = Player({ static_cast<float>(WINDOW_W / 2),
                                     static_cast<float>(WINDOW_H - 80) });
                m_enemies.clear();
                m_snowballs.clear();
                m_rollingBalls.clear();
                spawnInitialEnemies();
                m_state = GameState::Playing;
                m_stateTimer = 0.f;
            }
        }
    }
}

void Game::update(float dt)
{
    if (m_state != GameState::Playing)
    {
        m_stateTimer += dt;
        return;
    }

    m_player.handleInput();
    m_player.applyGravity(dt);
    m_player.update(dt);
    resolvePlayerVsTiles();

    auto& pending = m_player.getPendingSnowballs();
    for (auto& sb : pending)
        m_snowballs.push_back(std::move(sb));
    pending.clear();

    updateSnowballs(dt);
    updateEnemies(dt);
    updateRollingBalls(dt);
    checkPlayerEnemyCollision();
    checkPlayerRollingCollision();

    m_hud.update(m_player.getScore(), m_player.getHP(), 1);

    if (!m_player.isAlive())
    {
        m_state = GameState::GameOver;
        m_stateTimer = 0.f;
    }

    bool allDead = true;
    for (auto& e : m_enemies)
        if (e->isAlive()) { allDead = false; break; }

    if (allDead && !m_enemies.empty())
    {
        m_state = GameState::LevelClear;
        m_stateTimer = 0.f;
    }
}

void Game::render()
{
    m_window.clear(sf::Color(15, 20, 50));

    m_tileMap.draw(m_window);

    for (auto& e : m_enemies)      if (e->isActive())  e->draw(m_window);
    for (auto& sb : m_snowballs)    if (sb->isActive()) sb->draw(m_window);
    for (auto& rb : m_rollingBalls) if (rb->isActive()) rb->draw(m_window);

    if (m_player.isAlive())
        m_player.draw(m_window);

    m_hud.draw(m_window);

    if (m_state == GameState::LevelClear) drawLevelClear();
    if (m_state == GameState::GameOver)   drawGameOver();

    m_window.display();
}

void Game::resolveEntityVsTiles(Entity& entity,
    sf::Vector2f& velocity,
    bool& onGround,
    sf::Vector2f size)
{
    sf::FloatRect bounds = entity.getBounds();

    auto vTiles = m_tileMap.getSolidTiles(bounds);
    for (auto& tile : vTiles)
    {
        sf::FloatRect overlap;
        if (!bounds.intersects(tile, overlap)) continue;

        if (overlap.width > overlap.height)
        {
            float centreY = bounds.top + bounds.height / 2.f;
            float tileCY = tile.top + tile.height / 2.f;

            if (centreY < tileCY)
            {
                entity.setPosition({ entity.getPosition().x,
                    entity.getPosition().y - overlap.height });
                onGround = true;
                velocity.y = 0.f;
            }
            else
            {
                entity.setPosition({ entity.getPosition().x,
                    entity.getPosition().y + overlap.height });
                velocity.y = 0.f;
            }
            bounds = entity.getBounds();
        }
    }

    auto hTiles = m_tileMap.getSolidTiles(bounds);
    for (auto& tile : hTiles)
    {
        sf::FloatRect overlap;
        if (!bounds.intersects(tile, overlap)) continue;

        if (overlap.width <= overlap.height)
        {
            float centreX = bounds.left + bounds.width / 2.f;
            float tileCX = tile.left + tile.width / 2.f;

            if (centreX < tileCX)
            {
                entity.setPosition({ entity.getPosition().x - overlap.width,
                    entity.getPosition().y });
                velocity.x = 0.f;
            }
            else
            {
                entity.setPosition({ entity.getPosition().x + overlap.width,
                    entity.getPosition().y });
                velocity.x = 0.f;
            }
            bounds = entity.getBounds();
        }
    }
}

void Game::resolvePlayerVsTiles()
{
    sf::FloatRect bounds = m_player.getBounds();
    bool onGround = false;

    auto vTiles = m_tileMap.getSolidTiles(bounds);
    for (auto& tile : vTiles)
    {
        sf::FloatRect overlap;
        if (!bounds.intersects(tile, overlap)) continue;

        if (overlap.width > overlap.height)
        {
            float centreY = bounds.top + bounds.height / 2.f;
            float tileCY = tile.top + tile.height / 2.f;

            if (centreY < tileCY)
            {
                m_player.setOnGround(true, tile.top);
                onGround = true;
            }
            else
            {
                m_player.resolveRoof(tile.top + tile.height);
            }
            bounds = m_player.getBounds();
        }
    }
    if (!onGround)
        m_player.setOnGround(false, 0.f);

    bounds = m_player.getBounds();
    auto hTiles = m_tileMap.getSolidTiles(bounds);
    for (auto& tile : hTiles)
    {
        sf::FloatRect overlap;
        if (!bounds.intersects(tile, overlap)) continue;

        if (overlap.width <= overlap.height)
        {
            float centreX = bounds.left + bounds.width / 2.f;
            float tileCX = tile.left + tile.width / 2.f;

            if (centreX < tileCX)
                m_player.resolveWallRight(tile.left);
            else
                m_player.resolveWallLeft(tile.left + tile.width);

            bounds = m_player.getBounds();
        }
    }
}

void Game::resolveEnemyVsTiles(Enemy& enemy)
{
    sf::FloatRect bounds = enemy.getBounds();
    auto tiles = m_tileMap.getSolidTiles(bounds);

    for (auto& tile : tiles)
    {
        sf::FloatRect overlap;
        if (!bounds.intersects(tile, overlap)) continue;

        if (overlap.width > overlap.height)
        {
            float centreY = bounds.top + bounds.height / 2.f;
            float tileCY = tile.top + tile.height / 2.f;

            if (centreY < tileCY)
            {
                enemy.setPosition({ enemy.getPosition().x,
                    tile.top - bounds.height / 2.f });
            }
            bounds = enemy.getBounds();
        }
        else
        {
            float centreX = bounds.left + bounds.width / 2.f;
            float tileCX = tile.left + tile.width / 2.f;
            if (centreX < tileCX)
                enemy.setPosition({ tile.left - bounds.width / 2.f,
                    enemy.getPosition().y });
            else
                enemy.setPosition({ tile.left + tile.width + bounds.width / 2.f,
                    enemy.getPosition().y });
            bounds = enemy.getBounds();
        }
    }
}

void Game::resolveRollingVsTiles(RollingSnowball& ball)
{
    ball.applyGravity(0.016f);
    sf::FloatRect bounds = ball.getBounds();
    auto tiles = m_tileMap.getSolidTiles(bounds);

    for (auto& tile : tiles)
    {
        sf::FloatRect overlap;
        if (!bounds.intersects(tile, overlap)) continue;

        float centreY = bounds.top + bounds.height / 2.f;
        float tileCY = tile.top + tile.height / 2.f;

        if (overlap.width > overlap.height && centreY < tileCY)
        {
            ball.resolveFloor(tile.top);
            bounds = ball.getBounds();
        }
    }
}

void Game::updateSnowballs(float dt)
{
    for (auto& sb : m_snowballs)
    {
        if (!sb->isActive()) continue;
        sb->update(dt);

        sf::FloatRect sbBounds = sb->getBounds();
        auto tiles = m_tileMap.getSolidTiles(sbBounds);
        if (!tiles.empty()) { sb->deactivate(); continue; }

        for (auto& e : m_enemies)
        {
            if (!e->isAlive()) continue;
            if (sbBounds.intersects(e->getBounds()))
            {
                e->hitBySnow();
                m_player.addScore(SCORE_SNOWBALL_HIT);
                sb->deactivate();
                break;
            }
        }
    }

    m_snowballs.erase(
        std::remove_if(m_snowballs.begin(), m_snowballs.end(),
            [](const auto& s) { return !s->isActive(); }),
        m_snowballs.end());
}

void Game::updateEnemies(float dt)
{
    for (auto& e : m_enemies)
    {
        if (!e->isActive()) continue;
        e->update(dt);
        resolveEnemyVsTiles(*e);

        if (e->isFullyCovered() &&
            e->getBounds().intersects(m_player.getBounds()))
        {
            float dir = (e->getPosition().x > m_player.getPosition().x) ? 1.f : -1.f;
            auto rb = e->kick(dir);
            m_player.addScore(SCORE_KILL);
            m_rollingBalls.push_back(std::move(rb));
        }
    }

    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const auto& e) { return !e->isActive(); }),
        m_enemies.end());
}

void Game::updateRollingBalls(float dt)
{
    for (auto& rb : m_rollingBalls)
    {
        if (!rb->isActive()) continue;
        rb->update(dt);
        resolveRollingVsTiles(*rb);

        sf::FloatRect rbBounds = rb->getBounds();

        for (auto& e : m_enemies)
        {
            if (!e->isAlive()) continue;
            if (rbBounds.intersects(e->getBounds()))
            {
                float dir = (rb->getPosition().x > e->getPosition().x) ? 1.f : -1.f;
                auto newRb = e->kick(dir);
                m_player.addScore(SCORE_BOUNCING_KILL);
                m_rollingBalls.push_back(std::move(newRb));
            }
        }
    }

    m_rollingBalls.erase(
        std::remove_if(m_rollingBalls.begin(), m_rollingBalls.end(),
            [](const auto& r) { return !r->isActive(); }),
        m_rollingBalls.end());
}

void Game::checkPlayerEnemyCollision()
{
    if (m_player.isInvincible()) return;

    for (auto& e : m_enemies)
    {
        if (!e->isAlive()) continue;
        if (e->getState() == EnemyState::Walking &&
            e->getBounds().intersects(m_player.getBounds()))
        {
            m_player.takeDamage();
            break;
        }
    }
}

void Game::checkPlayerRollingCollision()
{
    if (m_player.isInvincible()) return;

    for (auto& rb : m_rollingBalls)
    {
        if (!rb->isActive()) continue;
        if (rb->getBounds().intersects(m_player.getBounds()))
        {
            m_player.takeDamage();
            break;
        }
    }
}

void Game::spawnInitialEnemies()
{
    const std::vector<sf::Vector2f> spawnPoints =
    {
        {  5 * 32.f + 16.f,  4 * 32.f - 16.f },
        { 17 * 32.f + 16.f,  4 * 32.f - 16.f },
        {  6 * 32.f + 16.f,  8 * 32.f - 16.f },
        { 16 * 32.f + 16.f,  8 * 32.f - 16.f },
        {  4 * 32.f + 16.f, 12 * 32.f - 16.f },
        { 17 * 32.f + 16.f, 12 * 32.f - 16.f },
        { 10 * 32.f + 16.f, 16 * 32.f - 16.f },
        { 13 * 32.f + 16.f, 20 * 32.f - 16.f },
    };

    for (auto& pos : spawnPoints)
        m_enemies.push_back(std::make_unique<Enemy>(pos));
}

void Game::drawLevelClear()
{
    sf::RectangleShape overlay({ static_cast<float>(WINDOW_W),
                                 static_cast<float>(WINDOW_H) });
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    m_window.draw(overlay);

    // Green banner = Level Clear
    sf::RectangleShape banner({ 300.f, 60.f });
    banner.setFillColor(sf::Color(0, 160, 0, 220));
    banner.setOrigin(150.f, 30.f);
    banner.setPosition(WINDOW_W / 2.f, WINDOW_H / 2.f);
    m_window.draw(banner);
}

void Game::drawGameOver()
{
    sf::RectangleShape overlay({ static_cast<float>(WINDOW_W),
                                 static_cast<float>(WINDOW_H) });
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    m_window.draw(overlay);

    // Red banner = Game Over
    sf::RectangleShape banner({ 300.f, 60.f });
    banner.setFillColor(sf::Color(160, 0, 0, 220));
    banner.setOrigin(150.f, 30.f);
    banner.setPosition(WINDOW_W / 2.f, WINDOW_H / 2.f);
    m_window.draw(banner);
}
