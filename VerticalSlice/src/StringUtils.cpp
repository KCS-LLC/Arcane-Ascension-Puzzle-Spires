#include "PCH.h"
#include "StringUtils.h"
#include "SpireData.h"
#include "Structs.h"

// ... (existing string conversion functions) ...

std::string roomTypeToString(RoomType type) {
    switch (type) {
        case RoomType::Entrance: return "Entrance";
        case RoomType::Combat: return "Combat";
        case RoomType::Treasure: return "Treasure";
        case RoomType::Boss: return "Boss";
        case RoomType::Sanctuary: return "Sanctuary";
        case RoomType::Special: return "Special";
        case RoomType::Puzzle: return "Puzzle";
        case RoomType::Trap: return "Trap";
        case RoomType::AgilityChallenge: return "Agility Challenge";
        case RoomType::EnduranceChallenge: return "Endurance Challenge";
        case RoomType::MagicChallenge: return "Magic Challenge";
        default: return "Unknown";
    }
}

std::string primaryGemTypeToString(PrimaryGemType type) {
    switch (type) {
        case PrimaryGemType::Attack: return "Attack";
        case PrimaryGemType::Mana: return "Mana";
        case PrimaryGemType::Treasure: return "Treasure";
        default: return "Empty";
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
        case GemSubType::Skull: return "Skull";
        default: return "Unknown";
    }
}

std::string doorColorToString(DoorColor color) {
    switch (color) {
        case DoorColor::Red: return "Red";
        case DoorColor::Orange: return "Orange";
        case DoorColor::Yellow: return "Yellow";
        case DoorColor::Green: return "Green";
        case DoorColor::Blue: return "Blue";
        case DoorColor::Indigo: return "Indigo";
        case DoorColor::Violet: return "Violet";
        case DoorColor::Copper: return "Copper";
        case DoorColor::Silver: return "Silver";
        case DoorColor::Gold: return "Gold";
        case DoorColor::White: return "White";
        default: return "Unknown";
    }
}

sf::Color doorColorToSfColor(DoorColor color) {
    switch (color) {
        case DoorColor::Red: return sf::Color::Red;
        case DoorColor::Orange: return sf::Color(255, 165, 0);
        case DoorColor::Yellow: return sf::Color::Yellow;
        case DoorColor::Green: return sf::Color::Green;
        case DoorColor::Blue: return sf::Color::Blue;
        case DoorColor::Indigo: return sf::Color(75, 0, 130);
        case DoorColor::Violet: return sf::Color(238, 130, 238);
        case DoorColor::Copper: return sf::Color(184, 115, 51);
        case DoorColor::Silver: return sf::Color(192, 192, 192);
        case DoorColor::Gold: return sf::Color(255, 215, 0);
        case DoorColor::White: return sf::Color::White;
        default: return sf::Color::Black;
    }
}

GemSubType stringToGemSubType(const std::string& s) {
    if (s == "Fire") return GemSubType::Fire;
    if (s == "Water") return GemSubType::Water;
    if (s == "Earth") return GemSubType::Earth;
    if (s == "Air") return GemSubType::Air;
    if (s == "Light") return GemSubType::Light;
    if (s == "Umbral") return GemSubType::Umbral;
    if (s == "Skull") return GemSubType::Skull;
    return GemSubType::Generic;
}

EffectType stringToEffectType(const std::string& s) {
    if (s == "Damage") return EffectType::Damage;
    if (s == "Heal") return EffectType::Heal;
    if (s == "Stun") return EffectType::Stun;
    if (s == "Shield") return EffectType::Shield;
    return EffectType::Damage; // Default
}

TargetType stringToTargetType(const std::string& s) {
    if (s == "Player") return TargetType::Player;
    if (s == "Monster") return TargetType::Monster;
    return TargetType::Monster; // Default
}

std::string wordWrap(const std::string& text, unsigned int lineLength) {
    std::stringstream ss(text);
    std::string line;
    std::string result;
    std::string word;

    while (ss >> word) {
        if (line.length() + word.length() + 1 > lineLength) {
            result += line + '\n';
            line = "";
        }
        if (!line.empty()) {
            line += " ";
        }
        line += word;
    }
    result += line;
    return result;
}