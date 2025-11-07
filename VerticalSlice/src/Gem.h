#ifndef GEM_H
#define GEM_H

enum class GemType {
    Empty,
    Skull,
    Coin,
    Fire,
    Water,
    Earth,
    Light,
    Air,
    Umbral,
    Life,
    Death,
    Mental,
    Perception,
    Transference,
    Enhancement
};

struct Gem {
    GemType type;
};

#endif // GEM_H