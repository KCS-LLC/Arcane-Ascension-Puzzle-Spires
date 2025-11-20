#pragma once

#include <SFML/Graphics.hpp>
#include "Structs.h"

// Forward-declarations to avoid circular dependencies
class Board;
class Player;
class Monster;

class BaseGem {
public:
    BaseGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture);
    virtual ~BaseGem() = default;

    // Pure virtual function makes this an abstract class
    // This is the core of the polymorphic behavior
    virtual void onMatch(Board& board, Player& player, Monster& monster) = 0;

    // Virtual functions for potential future stateful behavior
    virtual void onTurnStart(Board& board) {}
    virtual void onTurnEnd(Board& board) {}

    // Common getters
    GemSubType getSubType() const;
    const GemCatalogEntry* getCatalogEntry() const;

    // Common data and rendering
    void setPosition(float x, float y);
    const sf::Vector2f& getPosition() const;
    sf::Sprite& getSprite();
    void render(sf::RenderWindow& window);

protected:
    const GemCatalogEntry* m_catalogEntry;
    sf::Sprite m_sprite;
    sf::Vector2f m_position;
};
