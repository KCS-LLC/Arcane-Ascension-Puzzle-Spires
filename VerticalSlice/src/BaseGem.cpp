#include "BaseGem.h"

BaseGem::BaseGem(const GemCatalogEntry* catalogEntry, const sf::Texture& texture) 
    : m_catalogEntry(catalogEntry), m_sprite(texture) {}

GemSubType BaseGem::getSubType() const {
    return static_cast<GemSubType>(m_catalogEntry->id);
}

const GemCatalogEntry* BaseGem::getCatalogEntry() const {
    return m_catalogEntry;
}

void BaseGem::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    m_sprite.setPosition(m_position);
}

const sf::Vector2f& BaseGem::getPosition() const {
    return m_position;
}

sf::Sprite& BaseGem::getSprite() {
    return m_sprite;
}

void BaseGem::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
