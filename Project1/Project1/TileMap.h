#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "LevelMap.h"
#include <vector>

struct Tile
{
    sf::FloatRect bounds;
    bool          solid;
};

// ─── Tile map renderer + collision query ─────────────────
class TileMap
{
public:
    TileMap();

    void draw(sf::RenderWindow& window) const;

    // Returns all solid tiles that intersect rect
    std::vector<sf::FloatRect> getSolidTiles(sf::FloatRect rect) const;

    bool isSolid(int col, int row) const;

    sf::FloatRect getTileBounds(int col, int row) const
    {
        return { static_cast<float>(col * TILE_SIZE),
                 static_cast<float>(row * TILE_SIZE),
                 static_cast<float>(TILE_SIZE),
                 static_cast<float>(TILE_SIZE) };
    }

private:
    // Pre-built vertex array for fast drawing
    sf::VertexArray m_vertices;
    std::vector<Tile> m_tiles;
};