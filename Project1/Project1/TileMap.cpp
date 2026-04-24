#include "TileMap.h"

TileMap::TileMap()
{
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(MAP_ROWS * MAP_COLS * 4);

    for (int row = 0; row < MAP_ROWS; ++row)
    {
        for (int col = 0; col < MAP_COLS; ++col)
        {
            bool solid = (LEVEL1_MAP[row][col] == 1);

            Tile t;
            t.bounds = { static_cast<float>(col * TILE_SIZE),
                         static_cast<float>(row * TILE_SIZE),
                         static_cast<float>(TILE_SIZE),
                         static_cast<float>(TILE_SIZE) };
            t.solid = solid;
            m_tiles.push_back(t);

            int   idx = (row * MAP_COLS + col) * 4;
            float x = static_cast<float>(col * TILE_SIZE);
            float y = static_cast<float>(row * TILE_SIZE);
            float s = static_cast<float>(TILE_SIZE);

            // Alternate two shades for visual variety
            sf::Color c = solid
                ? (((row + col) % 2 == 0)
                    ? sf::Color(50, 80, 160)
                    : sf::Color(40, 70, 145))
                : sf::Color(15, 20, 50); // background sky colour

            m_vertices[idx + 0].position = { x,     y };
            m_vertices[idx + 1].position = { x + s, y };
            m_vertices[idx + 2].position = { x + s, y + s };
            m_vertices[idx + 3].position = { x,     y + s };

            for (int v = 0; v < 4; ++v)
                m_vertices[idx + v].color = c;
        }
    }
}

void TileMap::draw(sf::RenderWindow& window) const
{
    window.draw(m_vertices);
}

bool TileMap::isSolid(int col, int row) const
{
    if (col < 0 || col >= MAP_COLS || row < 0 || row >= MAP_ROWS) return true;
    return m_tiles[row * MAP_COLS + col].solid;
}

std::vector<sf::FloatRect> TileMap::getSolidTiles(sf::FloatRect rect) const
{
    std::vector<sf::FloatRect> result;

    int colMin = static_cast<int>(rect.left / TILE_SIZE);
    int colMax = static_cast<int>((rect.left + rect.width) / TILE_SIZE);
    int rowMin = static_cast<int>(rect.top / TILE_SIZE);
    int rowMax = static_cast<int>((rect.top + rect.height) / TILE_SIZE);

    for (int r = rowMin; r <= rowMax; ++r)
        for (int c = colMin; c <= colMax; ++c)
            if (isSolid(c, r))
                result.push_back(getTileBounds(c, r));

    return result;
}