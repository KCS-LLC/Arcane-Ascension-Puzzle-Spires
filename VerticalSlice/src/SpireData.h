#ifndef SPIRE_DATA_H
#define SPIRE_DATA_H

#include <vector>
#include <string>
#include "json.hpp"

// --- Spire Data Structures ---

enum class RoomType {
    Entrance,
    Combat,
    Treasure,
    Boss,
    Sanctuary,
    Special,
    Puzzle,
    Trap,
    Unknown
};

enum class DoorColor { Red, Orange, Yellow, Green, Blue, Indigo, Violet, Copper, Silver, Gold, White, Unknown };

struct Teleporter {
    DoorColor color;
    int destinationRoomId;
};

struct Room {
    int id;
    std::string name;
    RoomType type;
    std::string monsterId;
    std::vector<Teleporter> connections;
};

struct Floor {
    int floorNumber;
    std::vector<Room> rooms;
    int startRoomId;
};

// --- JSON Parsing Logic ---

// Helper to convert strings from JSON to enums
inline RoomType stringToRoomType(const std::string& s) {
    if (s == "Entrance")  return RoomType::Entrance;
    if (s == "Combat")    return RoomType::Combat;
    if (s == "Treasure")  return RoomType::Treasure;
    if (s == "Boss")      return RoomType::Boss;
    if (s == "Sanctuary") return RoomType::Sanctuary;
    if (s == "Special")   return RoomType::Special;
    if (s == "Puzzle")    return RoomType::Puzzle;
    if (s == "Trap")      return RoomType::Trap;
    return RoomType::Unknown;
}

inline DoorColor stringToDoorColor(const std::string& s) {
    if (s == "Red")    return DoorColor::Red;
    if (s == "Orange") return DoorColor::Orange;
    if (s == "Yellow") return DoorColor::Yellow;
    if (s == "Green")  return DoorColor::Green;
    if (s == "Blue")   return DoorColor::Blue;
    if (s == "Indigo") return DoorColor::Indigo;
    if (s == "Violet") return DoorColor::Violet;
    if (s == "Copper") return DoorColor::Copper;
    if (s == "Silver") return DoorColor::Silver;
    if (s == "Gold")   return DoorColor::Gold;
    if (s == "White")  return DoorColor::White;
    return DoorColor::Unknown;
}

// nlohmann_json specializations for parsing
inline void from_json(const nlohmann::json& j, Teleporter& t) {
    t.color = stringToDoorColor(j.at("color").get<std::string>());
    j.at("destinationId").get_to(t.destinationRoomId);
}

inline void from_json(const nlohmann::json& j, Room& r) {
    j.at("id").get_to(r.id);
    j.at("name").get_to(r.name);
    r.type = stringToRoomType(j.at("type").get<std::string>());
    if (j.contains("monsterId")) {
        j.at("monsterId").get_to(r.monsterId);
    }
    j.at("connections").get_to(r.connections);
}

inline void from_json(const nlohmann::json& j, Floor& f) {
    j.at("floorNumber").get_to(f.floorNumber);
    j.at("startRoomId").get_to(f.startRoomId);
    j.at("rooms").get_to(f.rooms);
}

#endif // SPIRE_DATA_H