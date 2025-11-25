#include "PCH.h"
#include "BaseGem.h"
#include "Player.h"
#include "Monster.h"
#include "Board.h"

BaseGem::BaseGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture) 

    : m_catalogEntry(catalogEntry), 

      m_sprite(texture),

      m_level(1),

      m_statusEffect(StatusEffect::None),

      m_actionState(ActionState::None),

      m_effectValue(0.0f),

      m_effectMax(0.0f),

      m_periodicActivationValue(0.0f) {
    
    // Scale the sprite to fit the tile size
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = static_cast<float>(TILE_SIZE) / textureSize.x;
    float scaleY = static_cast<float>(TILE_SIZE) / textureSize.y;
    m_sprite.setScale(sf::Vector2f(scaleX, scaleY));
}

GemSubType BaseGem::getSubType() const {
    return static_cast<GemSubType>(m_catalogEntry->id);
}

const GemCatalogEntry* BaseGem::getCatalogEntry() const {
    return m_catalogEntry;
}

void BaseGem::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
}

const sf::Vector2f& BaseGem::getPosition() const {
    return m_position;
}

sf::Sprite& BaseGem::getSprite() {
    return m_sprite;
}

void BaseGem::transform(const GemCatalogEntry* newCatalogEntry, const sf::Texture& newTexture) {
    m_catalogEntry = newCatalogEntry;
    m_sprite.setTexture(newTexture);

    // Rescale the sprite to fit the tile size, in case the new texture has different dimensions
    sf::Vector2u textureSize = newTexture.getSize();
    float scaleX = static_cast<float>(TILE_SIZE) / textureSize.x;
    float scaleY = static_cast<float>(TILE_SIZE) / textureSize.y;
    m_sprite.setScale(sf::Vector2f(scaleX, scaleY));
}

void BaseGem::render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin) {
    m_sprite.setPosition(m_position + boardOrigin);
    window.draw(m_sprite);
}

// --- New Layer Getters/Setters ---

int BaseGem::getLevel() const { return m_level; }
void BaseGem::setLevel(int level) { m_level = level; }

StatusEffect BaseGem::getStatusEffect() const { return m_statusEffect; }
void BaseGem::setStatusEffect(StatusEffect effect) { m_statusEffect = effect; }

ActionState BaseGem::getActionState() const { return m_actionState; }
void BaseGem::setActionState(ActionState state) { m_actionState = state; }

int BaseGem::getCounter() const { return static_cast<int>(m_effectValue); } // Return as int for now, or consider removing if no longer needed
void BaseGem::setCounter(int value) { m_effectValue = static_cast<float>(value); } // Set effectValue for now

float BaseGem::getEffectValue() const { return m_effectValue; }
void BaseGem::setEffectValue(float value) { m_effectValue = value; }

float BaseGem::getEffectMax() const { return m_effectMax; }
void BaseGem::setEffectMax(float value) { m_effectMax = value; }

float BaseGem::getPeriodicActivationValue() const { return m_periodicActivationValue; }
void BaseGem::setPeriodicActivationValue(float value) { m_periodicActivationValue = value; }

void BaseGem::onTurnEnd(Board& board, Player& player, Monster& monster, float speedCost) {
    if (m_statusEffect == StatusEffect::Burning) {
        float oldEffectValue = m_effectValue;
        m_effectValue -= speedCost;

        // Damage Calculation: (Player's Max Mana for Primary Ability * 0.10) * Gem Level
        float damagePerTick = (player.getMaxMana() * 0.10f) * m_level;

        // Check for periodic activation
        if (m_periodicActivationValue > 0) {
            // Calculate how many thresholds were crossed
            int oldTicks = static_cast<int>(oldEffectValue / m_periodicActivationValue);
            int newTicks = static_cast<int>(m_effectValue / m_periodicActivationValue);

            // Trigger damage for each crossed threshold
            for (int i = oldTicks - 1; i >= newTicks; --i) {
                if (i >= 0) { // Ensure we don't trigger for values above max or below 0
                    monster.takeDamage(static_cast<int>(damagePerTick));
                    // TODO: Add damage animation for monster
                }
            }
        }

        // Final Resolution
        if (m_effectValue <= 0) {
            monster.takeDamage(static_cast<int>(damagePerTick)); // Final damage tick
            // TODO: Add damage animation for monster
            m_statusEffect = StatusEffect::None;
            m_effectValue = 0.0f;
            m_effectMax = 0.0f;
            m_periodicActivationValue = 0.0f;
            // TODO: Add effect end animation
        }
    }
    // Add logic for other status effects here
}