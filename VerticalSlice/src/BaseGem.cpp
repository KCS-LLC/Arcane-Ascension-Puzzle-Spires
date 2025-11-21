#include "PCH.h"
#include "BaseGem.h"

BaseGem::BaseGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture) 
    : m_catalogEntry(catalogEntry), m_sprite(texture) {
    
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

void BaseGem::render(sf::RenderWindow& window, const sf::Vector2f& boardOrigin) {
    m_sprite.setPosition(m_position + boardOrigin);
    window.draw(m_sprite);
}