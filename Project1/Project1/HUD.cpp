#include "HUD.h"
#include "Constants.h"

HUD::HUD()
{
    m_bar.setSize({ static_cast<float>(WINDOW_W), 24.f });
    m_bar.setFillColor(sf::Color(10, 10, 40, 220));

    // No font loading at all — uses shape-based fallback always
    m_fontLoaded = false;
}

void HUD::update(int score, int hp, int level)
{
    m_score = score;
    m_hp = hp;
    m_level = level;
}

void HUD::draw(sf::RenderWindow& window) const
{
    // Draw top bar
    window.draw(m_bar);

    // Always use shape-based display (no font needed)
    drawFallback(window);
}

void HUD::drawFallback(sf::RenderWindow& window) const
{
    // ── HP pips (red squares on left) ─────────────────────
    sf::RectangleShape pip({ 14.f, 14.f });
    pip.setFillColor(sf::Color(220, 60, 60));
    for (int i = 0; i < m_hp; ++i)
    {
        pip.setPosition(8.f + i * 18.f, 5.f);
        window.draw(pip);
    }

    // ── Score bar (green, grows with score) ───────────────
    float scoreWidth = static_cast<float>(m_score) / 10.f;
    if (scoreWidth > 200.f) scoreWidth = 200.f;

    sf::RectangleShape scoreOutline({ 200.f, 10.f });
    scoreOutline.setFillColor(sf::Color::Transparent);
    scoreOutline.setOutlineColor(sf::Color(80, 220, 80));
    scoreOutline.setOutlineThickness(1.f);
    scoreOutline.setPosition(static_cast<float>(WINDOW_W / 2) - 100.f, 7.f);
    window.draw(scoreOutline);

    sf::RectangleShape scoreBar({ scoreWidth, 10.f });
    scoreBar.setFillColor(sf::Color(80, 220, 80));
    scoreBar.setPosition(static_cast<float>(WINDOW_W / 2) - 100.f, 7.f);
    window.draw(scoreBar);

    // ── Level indicator (cyan squares on right) ───────────
    sf::RectangleShape lvlBox({ 14.f, 14.f });
    lvlBox.setFillColor(sf::Color(0, 200, 220));
    for (int i = 0; i < m_level; ++i)
    {
        lvlBox.setPosition(static_cast<float>(WINDOW_W) - 20.f - i * 18.f, 5.f);
        window.draw(lvlBox);
    }
}
