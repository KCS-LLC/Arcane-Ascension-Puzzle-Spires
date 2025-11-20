#pragma once

#include <memory>
#include "BaseGem.h"
#include "DataManager.h"

class GemFactory {
public:
    GemFactory(const DataManager& dataManager);

    std::unique_ptr<BaseGem> createGem(GemSubType subType, const sf::Texture& texture);

private:
    const DataManager& m_dataManager;
};
