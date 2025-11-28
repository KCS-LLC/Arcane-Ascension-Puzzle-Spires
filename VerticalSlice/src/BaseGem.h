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
    virtual float onMatch(Board& board, Player& player, Monster& monster) = 0;

    // Virtual functions for potential future stateful behavior
    virtual void onTurnStart(Board& board) {}

    // Common getters
    GemSubType getSubType() const;
    const GemCatalogEntry* getCatalogEntry() const;

    // Transformation
    virtual void transform(const GemCatalogEntry* newCatalogEntry, const sf::Texture& newTexture);

    // Common data and rendering
    void setPosition(float x, float y);
    const sf::Vector2f& getPosition() const;
    sf::Sprite& getSprite();
    void render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin);

    // New layer getters/setters
    int getLevel() const;
    void setLevel(int level);
    StatusEffect getStatusEffect() const;
    void setStatusEffect(StatusEffect effect);
    ActionState getActionState() const;
    void setActionState(ActionState state);

protected:
    const GemCatalogEntry* m_catalogEntry;
    sf::Sprite m_sprite;
    sf::Vector2f m_position;

    // New layer properties
    int m_level;
    StatusEffect m_statusEffect;
    ActionState m_actionState;
};