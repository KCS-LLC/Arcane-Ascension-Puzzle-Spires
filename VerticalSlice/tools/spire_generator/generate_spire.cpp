#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

// --- Data Structures ---
enum class RoomType { Entrance, Combat, Treasure, Boss, Sanctuary, Special, Puzzle, Trap, Unknown };
enum class DoorColor { Red, Orange, Yellow, Green, Blue, Copper, Silver, Gold, White, Unknown };

struct Teleporter {
    DoorColor color;
    int destinationRoomId;
};

struct Room {
    int id;
    RoomType type = RoomType::Unknown;
    std::string monsterId;
    std::vector<Teleporter> connections;
    bool isDeadEndCandidate = false;
};

struct Floor {
    int floorNumber;
    std::vector<Room> rooms;
    int startRoomId;
};

// --- Helper Functions ---
std::mt19937 rng(time(0));

int getRandomInt(int min, int max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

double getRandomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

// Enum to String Converters
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

DoorColor determineTreasureTier(int floorNumber) {
    double goldChance = 1.0;
    double silverChance = 14.0;
    double scalingFactor = getRandomDouble(0.5, 1.5);
    double goldBonus = (floorNumber - 1) * scalingFactor;
    goldChance += goldBonus;
    if (goldChance > 80.0) goldChance = 80.0;
    double copperChance = 100.0 - goldChance - silverChance;
    if (copperChance < 0) {
        silverChance += copperChance;
        copperChance = 0;
    }
    if (silverChance < 0) silverChance = 0;
    double roll = getRandomDouble(0.0, 100.0);
    if (roll < goldChance) return DoorColor::Gold;
    if (roll < goldChance + silverChance) return DoorColor::Silver;
    return DoorColor::Copper;
}

DoorColor colorFromRoomType(RoomType type, int floorNumber) {
    switch (type) {
        case RoomType::Combat: return DoorColor::Red;
        case RoomType::Trap: return DoorColor::Yellow;
        case RoomType::Puzzle: return DoorColor::Blue;
        case RoomType::Treasure:
        case RoomType::Special:
            return determineTreasureTier(floorNumber);
        case RoomType::Sanctuary:
        case RoomType::Boss:
            return DoorColor::White;
        default: return DoorColor::Red;
    }
}

// --- Main Generation Logic ---
int main(int argc, char* argv[]) {
    int floorNumber = 1;
    if (argc > 1) {
        floorNumber = std::stoi(argv[1]);
    }

    Floor floor;
    floor.floorNumber = floorNumber;
    floor.startRoomId = 0;

    // === Phase 1: Node Creation & Strict Type Assignment ===
    int roomCount = getRandomInt(15, 25);
    std::vector<Room> rooms(roomCount);
    for (int i = 0; i < roomCount; ++i) {
        rooms[i].id = i;
    }

    rooms[0].type = RoomType::Entrance;
    rooms[1].type = RoomType::Boss;
    rooms[1].isDeadEndCandidate = true;

    int totalRewardRooms = 0;
    if (floorNumber > 0 && floorNumber % 5 == 0) {
        totalRewardRooms = getRandomInt(1, 2);
    } else {
        double chance = 5.0 + (floorNumber / 2.0);
        if (getRandomDouble(0.0, 100.0) < chance) {
            totalRewardRooms = 1;
        }
    }

    int currentRoomIdx = 2;
    for (int i = 0; i < totalRewardRooms; ++i) {
        if (currentRoomIdx < roomCount) {
            rooms[currentRoomIdx].type = (getRandomInt(0, 1) == 0) ? RoomType::Treasure : RoomType::Special;
            rooms[currentRoomIdx].isDeadEndCandidate = true;
            currentRoomIdx++;
        }
    }

    while (currentRoomIdx < roomCount) {
        rooms[currentRoomIdx].type = RoomType::Combat;
        currentRoomIdx++;
    }

    std::shuffle(rooms.begin() + 1, rooms.end(), rng);
    floor.rooms = rooms;

    std::map<int, int> id_to_idx;
    for(int i = 0; i < floor.rooms.size(); ++i) {
        id_to_idx[floor.rooms[i].id] = i;
    }

    int bossRoomIdx = -1;
    for(int i = 0; i < floor.rooms.size(); ++i) {
        if (floor.rooms[i].type == RoomType::Boss) {
            bossRoomIdx = i;
            break;
        }
    }
    
    // === Phase 2 & 3: Build Progression Graph ===
    std::vector<int> connectedIndices = {0};
    std::vector<int> unconnectedIndices;
    for(int i = 0; i < floor.rooms.size(); ++i) {
        if (i != 0) unconnectedIndices.push_back(i);
    }
    
    int mainPathLength = getRandomInt(3, 4);
    int lastRoomIdx = 0;
    for(int i = 0; i < mainPathLength; ++i) {
        auto it = std::find_if(unconnectedIndices.begin(), unconnectedIndices.end(), [&](int idx){
            return idx != bossRoomIdx && !floor.rooms[idx].isDeadEndCandidate;
        });
        if (it == unconnectedIndices.end()) break;
        
        int nextRoomIdx = *it;
        floor.rooms[lastRoomIdx].connections.push_back({DoorColor::Unknown, floor.rooms[nextRoomIdx].id});
        connectedIndices.push_back(nextRoomIdx);
        unconnectedIndices.erase(it);
        lastRoomIdx = nextRoomIdx;
    }
    
    if (bossRoomIdx != -1) {
        floor.rooms[lastRoomIdx].connections.push_back({DoorColor::Unknown, floor.rooms[bossRoomIdx].id});
        connectedIndices.push_back(bossRoomIdx);
        auto it = std::find(unconnectedIndices.begin(), unconnectedIndices.end(), bossRoomIdx);
        if (it != unconnectedIndices.end()) unconnectedIndices.erase(it);
    }

    while(!unconnectedIndices.empty()) {
        int fromIdx = connectedIndices[getRandomInt(0, connectedIndices.size() - 1)];
        int toIdx = unconnectedIndices.front();
        if (fromIdx == bossRoomIdx) continue;
        floor.rooms[fromIdx].connections.push_back({DoorColor::Unknown, floor.rooms[toIdx].id});
        connectedIndices.push_back(toIdx);
        unconnectedIndices.erase(unconnectedIndices.begin());
    }

    // === Phase 4: Ensure No Invalid Dead Ends ===
    for(int i = 0; i < floor.rooms.size(); ++i) {
        if(floor.rooms[i].connections.empty() && !floor.rooms[i].isDeadEndCandidate) {
            int targetIdx = getRandomInt(0, roomCount - 1);
            while(targetIdx == i) targetIdx = getRandomInt(0, roomCount - 1);
            floor.rooms[i].connections.push_back({DoorColor::Unknown, floor.rooms[targetIdx].id});
        }
    }

    // === Phase 5: Add Loops and Backtracking ===
    for(int i = 0; i < floor.rooms.size(); ++i) {
        int extraConnections = getRandomInt(0, 5 - (int)floor.rooms[i].connections.size());
        for(int j = 0; j < extraConnections; ++j) {
            int targetIdx = getRandomInt(0, roomCount - 1);
            if(targetIdx == i) continue;
            bool exists = false;
            for(const auto& conn : floor.rooms[i].connections) if(conn.destinationRoomId == floor.rooms[targetIdx].id) exists = true;
            if(!exists) {
                floor.rooms[i].connections.push_back({DoorColor::Unknown, floor.rooms[targetIdx].id});
            }
        }
    }
    for(int i = 0; i < floor.rooms.size(); ++i) {
        for(const auto& conn : floor.rooms[i].connections) {
            int dest_idx = id_to_idx[conn.destinationRoomId];
            bool backExists = false;
            for(const auto& backConn : floor.rooms[dest_idx].connections) {
                if(backConn.destinationRoomId == floor.rooms[i].id) backExists = true;
            }
            if(!backExists) {
                 floor.rooms[dest_idx].connections.push_back({DoorColor::Unknown, floor.rooms[i].id});
            }
        }
    }

    // === Phase 6: Assign Colors & Finalize JSON ===
    json floorJson;
    floorJson["floorNumber"] = floor.floorNumber;
    floorJson["startRoomId"] = 0; // Always 0
    json roomsJson = json::array();

    for (const auto& room : floor.rooms) {
        json roomJson;
        roomJson["id"] = room.id;
        roomJson["type"] = roomTypeToString(room.type);
        if (room.type == RoomType::Combat || room.type == RoomType::Boss) {
             roomJson["monsterId"] = "monster_" + std::to_string(room.id);
        }
        
        json connectionsJson = json::array();
        for (const auto& teleporter : room.connections) {
            int dest_idx = id_to_idx.at(teleporter.destinationRoomId);
            json teleporterJson;
            teleporterJson["color"] = doorColorToString(colorFromRoomType(floor.rooms[dest_idx].type, floorNumber));
            teleporterJson["destinationId"] = teleporter.destinationRoomId;
            connectionsJson.push_back(teleporterJson);
        }
        roomJson["connections"] = connectionsJson;
        roomsJson.push_back(roomJson);
    }
    floorJson["rooms"] = roomsJson;

    std::ofstream outFile("floor" + std::to_string(floorNumber) + ".json");
    if (outFile.is_open()) {
        outFile << floorJson.dump(4);
        outFile.close();
        std::cout << "Successfully generated floor" << floorNumber << ".json" << std::endl;
    } else {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    return 0;
}
