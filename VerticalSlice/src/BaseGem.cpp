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

      m_actionState(ActionState::None) {
    
    // Scale the sprite to fit the tile size
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = static_cast<float>(TILE_SIZE) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(TILE_SIZE) / static_cast<float>(textureSize.y);
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
    float scaleX = static_cast<float>(TILE_SIZE) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(TILE_SIZE) / static_cast<float>(textureSize.y);
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

void BaseGem::levelUp() {
    m_level++;
}