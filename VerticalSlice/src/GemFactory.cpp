#include "GemFactory.h"
#include "ManaGem.h"
#include "AttackGem.h"
#include "TreasureGem.h"
// Include other gem subclasses as they are created

GemFactory::GemFactory(const DataManager& dataManager) : m_dataManager(dataManager) {}

std::unique_ptr<BaseGem> GemFactory::createGem(GemSubType subType, const sf::Texture& texture) {
    const GemCatalogEntry* catalogEntry = m_dataManager.getGemCatalogEntry(subType);
    if (catalogEntry == nullptr) {
        return nullptr;
    }

    PrimaryGemType primaryType = m_dataManager.getPrimaryGemType(subType);

    switch (primaryType) {
        case PrimaryGemType::Mana:
            return std::make_unique<ManaGem>(catalogEntry, texture);
        case PrimaryGemType::Attack:
            return std::make_unique<AttackGem>(catalogEntry, texture);
        case PrimaryGemType::Treasure:
            return std::make_unique<TreasureGem>(catalogEntry, texture);
        default:
            return nullptr;
    }
}
