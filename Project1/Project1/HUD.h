#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// ─── HUD – score bar at the top of the screen ─────────────
class HUD
{
public:
    HUD();

    void update(int score, int hp, int level);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Font        m_font;
    sf::Text        m_scoreText;
    sf::Text        m_hpText;
    sf::Text        m_levelText;
    sf::RectangleShape m_bar;
    bool            m_fontLoaded{ false };

    // Fallback shapes when no font is available
    void drawFallback(sf::RenderWindow& window) const;

    int  m_score{ 0 };
    int  m_hp{ 3 };
    int  m_level{ 1 };
};