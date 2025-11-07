#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "SpireData.h"
#include <string>
#include <SFML/Graphics.hpp>

std::string roomTypeToString(RoomType type);
std::string doorColorToString(DoorColor color);
sf::Color doorColorToSfColor(DoorColor color);

#endif // STRING_UTILS_H
