#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include "Structs.h" // Include the pure data definitions

// --- Function Declarations ---

std::string roomTypeToString(RoomType type);
std::string primaryGemTypeToString(PrimaryGemType type);
std::string gemSubtypeToString(GemSubType type);
std::string doorColorToString(DoorColor color);
sf::Color doorColorToSfColor(DoorColor color);
GemSubType stringToGemSubType(const std::string& s);
EffectType stringToEffectType(const std::string& s);
TargetType stringToTargetType(const std::string& s);
std::string wordWrap(const std::string& text, unsigned int lineLength);
DoorColor stringToDoorColor(const std::string& s);
RoomType stringToRoomType(const std::string& s);
