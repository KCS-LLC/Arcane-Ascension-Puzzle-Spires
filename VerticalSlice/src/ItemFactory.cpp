#include "PCH.h"
#include "ItemFactory.h"

std::unique_ptr<ItemInstance> ItemFactory::createItemInstance(int instanceId, const ItemBase* base) {
    if (base == nullptr) {
        return nullptr;
    }

    auto item = std::make_unique<ItemInstance>(instanceId, base);

    // TODO: In the future, this is where we would procedurally add stat modifiers,
    // passive abilities, and activated abilities based on the item's desired level/rarity.

    // For now, let's add a simple example stat for testing if it's a sword
    if (base->id == "iron_sword") {
        item->addStatModifier({StatType::Vigor, 5});
    }

    return item;
}
