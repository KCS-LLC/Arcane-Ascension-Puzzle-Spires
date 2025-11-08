#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "SpireData.h"
#include "Structs.h"
#include "GemType.h"
#include <string>
#include <SFML/Graphics.hpp>

std::string roomTypeToString(RoomType type);
std::string doorColorToString(DoorColor color);
sf::Color doorColorToSfColor(DoorColor color);
GemType stringToGemType(const std::string& s);
EffectType stringToEffectType(const std::string& s);
TargetType stringToTargetType(const std::string& s);

#endif // STRING_UTILS_H
