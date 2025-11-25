#pragma once

#include "Structs.h"

// Forward declarations
class Player;
class Monster;
class Board;
#include <map>
#include <SFML/Graphics/Texture.hpp>

// Forward declarations
class Player;
class Monster;
class Board;
class GemFactory;

class EffectProcessor {
public:
    bool processEffect(const Effect& effect, Player& player, Monster& monster, Board& board, GemFactory& gemFactory, const std::map<GemSubType, sf::Texture>& gemTextures);
};
