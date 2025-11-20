#include "PCH.h"
#include "StringUtils.h"
#include "SpireData.h"
#include "Structs.h"
#include "GemSubType.h"

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

GemSubType stringToGemSubType(const std::string& s) {
    if (s == "fire") return GemSubType::Fire;
    if (s == "water") return GemSubType::Water;
    if (s == "earth") return GemSubType::Earth;
    if (s == "air") return GemSubType::Air;
    if (s == "light") return GemSubType::Light;
    if (s == "umbral") return GemSubType::Umbral;
    if (s == "enhancement") return GemSubType::Enhancement;
    if (s == "perception") return GemSubType::Perception;
    if (s == "transference") return GemSubType::Transference;
    if (s == "life") return GemSubType::Life;
    if (s == "death") return GemSubType::Death;
    if (s == "mental") return GemSubType::Mental;
    if (s == "raw") return GemSubType::Raw;
    if (s == "sword") return GemSubType::Sword;
    if (s == "coin") return GemSubType::Coin;
    if (s == "skull") return GemSubType::Skull;
    return GemSubType::Generic; // Default
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

std::string primaryGemTypeToString(PrimaryGemType type) {
    switch (type) {
        case PrimaryGemType::Empty: return "Empty";
        case PrimaryGemType::Mana: return "Mana";
        case PrimaryGemType::Attack: return "Attack";
        case PrimaryGemType::Treasure: return "Treasure";
        default: return "Unknown";
    }
}

std::string gemSubtypeToString(GemSubType type) {
    switch (type) {
        case GemSubType::Fire: return "Fire";
        case GemSubType::Water: return "Water";
        case GemSubType::Earth: return "Earth";
        case GemSubType::Air: return "Air";
        case GemSubType::Light: return "Light";
        case GemSubType::Umbral: return "Umbral";
        case GemSubType::Life: return "Life";
        case GemSubType::Death: return "Death";
        case GemSubType::Mental: return "Mental";
        case GemSubType::Perception: return "Perception";
        case GemSubType::Transference: return "Transference";
        case GemSubType::Enhancement: return "Enhancement";
        case GemSubType::Raw: return "Raw";
        case GemSubType::Sword: return "Sword";
        case GemSubType::Coin: return "Coin";
        case GemSubType::Skull: return "Skull";
        case GemSubType::Generic: return "Generic";
        default: return "Unknown";
    }
}
