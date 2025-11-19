#ifndef GEM_H
#define GEM_H

#include "PrimaryGemType.h"
#include "GemSubType.h"

struct Gem {
    PrimaryGemType primaryType = PrimaryGemType::Empty;
    GemSubType subType = GemSubType::Empty;
    int level = 0;

    // Add a comparison operator for use in std::set/map if needed later
    bool operator<(const Gem& other) const {
        if (primaryType != other.primaryType) return primaryType < other.primaryType;
        if (subType != other.subType) return subType < other.subType;
        return level < other.level;
    }
};

#endif // GEM_H