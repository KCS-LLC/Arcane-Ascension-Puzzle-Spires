#pragma once

enum class GemType {
    Empty,
    Skull,
    // Primary Mana
    Fire,
    Water,
    Earth,
    Air,
    Light,
    Umbral,
    // Secondary Mana
    Enhancement,
    Perception,
    Transference,
    Life,
    Death,
    Mental,
    Raw,
    // Action Gems
    Sword,
    Coin,

    // For random generation range
    FirstMana = Fire,
    LastMana = Raw
};
