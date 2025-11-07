#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

// --- Reusable Data Structures ---

enum class RoomType {
    Entrance, Combat, Treasure, Boss, Sanctuary, Special, Puzzle, Trap, Unknown
};

enum class DoorColor {
    Red, Orange, Yellow, Green, Blue, Copper, Silver, Gold, White, Unknown
};

struct Teleporter {
    DoorColor color;
    int destinationRoomId;
};

struct Room {
    int id;
    RoomType type;
    std::string monsterId;
    std::vector<Teleporter> connections;
};

struct Floor {
    int floorNumber;
    std::vector<Room> rooms;
    int startRoomId;
};

// --- Reusable Parsing Logic ---

RoomType stringToRoomType(const std::string& s) {
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
        default: return "Unknown";
    }
}

DoorColor stringToDoorColor(const std::string& s) {
    if (s == "Red") return DoorColor::Red;
    if (s == "Orange") return DoorColor::Orange;
    if (s == "Yellow") return DoorColor::Yellow;
    if (s == "Green") return DoorColor::Green;
    if (s == "Blue") return DoorColor::Blue;
    if (s == "Copper") return DoorColor::Copper;
    if (s == "Silver") return DoorColor::Silver;
    if (s == "Gold") return DoorColor::Gold;
    if (s == "White") return DoorColor::White;
    return DoorColor::Unknown;
}

std::string doorColorToString(DoorColor color) {
    switch (color) {
        case DoorColor::Red: return "Red";
        case DoorColor::Orange: return "Orange";
        case DoorColor::Yellow: return "Yellow";
        case DoorColor::Green: return "Green";
        case DoorColor::Blue: return "Blue";
        case DoorColor::Copper: return "Copper";
        case DoorColor::Silver: return "Silver";
        case DoorColor::Gold: return "Gold";
        case DoorColor::White: return "White";
        default: return "Unknown";
    }
}

void from_json(const json& j, Teleporter& t) {
    t.color = stringToDoorColor(j.at("color").get<std::string>());
    j.at("destinationId").get_to(t.destinationRoomId);
}

void from_json(const json& j, Room& r) {
    j.at("id").get_to(r.id);
    r.type = stringToRoomType(j.at("type").get<std::string>());
    if (j.contains("monsterId")) {
        j.at("monsterId").get_to(r.monsterId);
    }
    j.at("connections").get_to(r.connections);
}

void from_json(const json& j, Floor& f) {
    j.at("floorNumber").get_to(f.floorNumber);
    j.at("startRoomId").get_to(f.startRoomId);
    j.at("rooms").get_to(f.rooms);
}

// --- SpireReader Application Logic ---

void printFloorSummary(const Floor& floor) {
    std::cout << "--- Floor " << floor.floorNumber << " Summary ---" << std::endl;
    std::cout << "Start Room ID: " << floor.startRoomId << std::endl;
    std::cout << "Total Rooms: " << floor.rooms.size() << std::endl;
    std::cout << "--------------------" << std::endl;

    for (const auto& room : floor.rooms) {
        std::cout << "Room " << room.id << " (" << roomTypeToString(room.type) << "):" << std::endl;
        if (room.connections.empty()) {
            std::cout << "  -> [End of a branch]" << std::endl;
        } else {
            for (const auto& teleporter : room.connections) {
                std::cout << "  -> " << doorColorToString(teleporter.color) 
                          << " door to Room " << teleporter.destinationRoomId << std::endl;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_floor_json>" << std::endl;
        return 1;
    }

    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        std::cerr << "Error: Could not open " << argv[1] << " for reading." << std::endl;
        return 1;
    }

    try {
        json data = json::parse(f);
        Floor floor = data.get<Floor>();
        printFloorSummary(floor);
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return 1;
    } catch (json::exception& e) {
        std::cerr << "JSON data error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}