#pragma once

#include "ItemData.h"
#include <memory>

class ItemFactory {
public:
    // For now, this is a placeholder. Later, this will take data from JSON.
    // A more complex procedural generation function will also be added.
    static std::unique_ptr<ItemInstance> createItemInstance(int instanceId, const ItemBase* base);
};
