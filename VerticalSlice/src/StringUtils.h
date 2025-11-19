#ifndef STRING_UTILS_H
#define STRING_UTILS_H

// Forward declarations
enum class RoomType;
enum class DoorColor;
enum class GemType;
enum class EffectType;
enum class TargetType;

namespace sf {
    class Color;
}

std::string roomTypeToString(RoomType type);
std::string doorColorToString(DoorColor color);
sf::Color doorColorToSfColor(DoorColor color);
GemType stringToGemType(const std::string& s);
EffectType stringToEffectType(const std::string& s);
TargetType stringToTargetType(const std::string& s);

#endif // STRING_UTILS_H
