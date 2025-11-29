#pragma once

#include "Structs.h"

// Forward declarations
class Player;
class Monster;
class Board;
#include <map>
#include <SFML/Graphics/Texture.hpp>

// Forward declarations
class GemFactory;
class TimeManager;
class Game;

class EffectProcessor {
public:
    std::vector<sf::Vector2i> processEffect(const Spell& spell, const Effect& effect, Game& game);
};
