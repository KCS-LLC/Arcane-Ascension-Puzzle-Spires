#include "StringUtils.h"

std::string roomTypeToString(RoomType type) {
    switch (type) {
        case RoomType::Entrance:  return "Entrance";
        case RoomType::Combat:    return "Combat";
        case RoomType::Treasure:  return "Treasure";
        case RoomType::Boss:      return "Boss";
        case RoomType::Sanctuary: return "Sanctuary";
        case RoomType::Special:   return "Special";
        case RoomType::Puzzle:    return "Puzzle";
        case RoomType::Trap:      return "Trap";
        case RoomType::AgilityChallenge: return "AgilityChallenge";
        case RoomType::EnduranceChallenge: return "EnduranceChallenge";
        case RoomType::MagicChallenge: return "MagicChallenge";
        default:                  return "Unknown";
    }
}

std::string doorColorToString(DoorColor color) {
    switch (color) {
        case DoorColor::Red:    return "Red";
        case DoorColor::Orange: return "Orange";
        case DoorColor::Yellow: return "Yellow";
        case DoorColor::Green:  return "Green";
        case DoorColor::Blue:   return "Blue";
        case DoorColor::Indigo: return "Indigo";
        case DoorColor::Violet: return "Violet";
        case DoorColor::Copper: return "Copper";
        case DoorColor::Silver: return "Silver";
        case DoorColor::Gold:   return "Gold";
        case DoorColor::White:  return "White";
        default:                return "Unknown";
    }
}

sf::Color doorColorToSfColor(DoorColor color) {
    switch (color) {
        case DoorColor::Red:    return sf::Color::Red;
        case DoorColor::Orange: return sf::Color(255, 165, 0);
        case DoorColor::Yellow: return sf::Color::Yellow;
        case DoorColor::Green:  return sf::Color::Green;
        case DoorColor::Blue:   return sf::Color::Blue;
        case DoorColor::Indigo: return sf::Color(75, 0, 130);
        case DoorColor::Violet: return sf::Color(238, 130, 238);
        case DoorColor::Copper: return sf::Color(184, 115, 51);
        case DoorColor::Silver: return sf::Color(192, 192, 192);
        case DoorColor::Gold:   return sf::Color(255, 215, 0);
        case DoorColor::White:  return sf::Color::White;
        default:                return sf::Color::Black;
    }
}

GemType stringToGemType(const std::string& s) {
    if (s == "fire") return GemType::Fire;
    if (s == "water") return GemType::Water;
    if (s == "earth") return GemType::Earth;
    if (s == "air") return GemType::Air;
    if (s == "light") return GemType::Light;
    if (s == "umbral") return GemType::Umbral;
    if (s == "enhancement") return GemType::Enhancement;
    if (s == "perception") return GemType::Perception;
    if (s == "transference") return GemType::Transference;
    if (s == "life") return GemType::Life;
    if (s == "death") return GemType::Death;
    if (s == "mental") return GemType::Mental;
    if (s == "raw") return GemType::Raw;
    if (s == "sword") return GemType::Sword;
    if (s == "coin") return GemType::Coin;
    return GemType::Skull; // Default
}

EffectType stringToEffectType(const std::string& s) {
    if (s == "damage") return EffectType::Damage;
    if (s == "heal") return EffectType::Heal;
    if (s == "stun") return EffectType::Stun;
    if (s == "shield") return EffectType::Shield;
    return EffectType::Damage; // Default
}

TargetType stringToTargetType(const std::string& s) {
    if (s == "player") return TargetType::Player;
    if (s == "monster") return TargetType::Monster;
    return TargetType::Monster; // Default
}
