#ifndef GEM_H
#define GEM_H

#include "PrimaryGemType.h"
#include "GemSubType.h"

struct Gem {
    PrimaryGemType primaryType = PrimaryGemType::Empty;
    GemSubType subType = GemSubType::Empty;
    int level = 0;

    // Default constructor
    constexpr Gem() = default;

    // Parameterized Constructor
    Gem(GemSubType subType, int level) : subType(subType), level(level) {
        switch (subType) {
            case GemSubType::Fire: 
            case GemSubType::Water: 
            case GemSubType::Earth: 
            case GemSubType::Air: 
            case GemSubType::Light: 
            case GemSubType::Umbral: 
            case GemSubType::Life: 
            case GemSubType::Death: 
            case GemSubType::Mental: 
            case GemSubType::Perception: 
            case GemSubType::Transference: 
            case GemSubType::Enhancement: 
                primaryType = PrimaryGemType::Mana;
                break;
            case GemSubType::Skull:
                primaryType = PrimaryGemType::Attack;
                break;
            case GemSubType::Coin:
                primaryType = PrimaryGemType::Treasure;
                break;
            case GemSubType::Empty:
            default:
                primaryType = PrimaryGemType::Empty;
                break;
        }
    }

    // Add a comparison operator for use in std::set/map if needed later
    bool operator<(const Gem& other) const {
        if (primaryType != other.primaryType) return primaryType < other.primaryType;
        if (subType != other.subType) return subType < other.subType;
        return level < other.level;
    }
};

#endif // GEM_H