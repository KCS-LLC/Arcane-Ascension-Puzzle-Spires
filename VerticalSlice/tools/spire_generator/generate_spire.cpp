#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <map>
#include <set>
#include "json.hpp"

using json = nlohmann::json;

// --- Data Structures ---
enum class RoomType { Entrance, Combat, Treasure, Boss, Sanctuary, Special, Puzzle, Trap, Unknown };
enum class DoorColor { Red, Orange, Yellow, Green, Blue, Indigo, Violet, Copper, Silver, Gold, White, Unknown };

struct Teleporter {
    DoorColor color;
    int destinationRoomId;
};

struct Room {
    int id;
    std::string name;
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

// --- Name Generation ---
const std::vector<std::string> ADJECTIVES = {"Whispering", "Forgotten", "Sunken", "Gilded", "Flooded", "Frozen", "Burning", "Shattered", "Hallowed", "Cursed"};
const std::vector<std::string> NOUNS = {"Hall", "Chamber", "Armory", "Library", "Sanctum", "Crypt", "Gallery", "Garden", "Observatory", "Nexus"};

std::string generateRoomName(std::set<std::string>& usedNames) {
    std::string name;
    do {
        name = ADJECTIVES[getRandomInt(0, ADJECTIVES.size() - 1)] + " " + NOUNS[getRandomInt(0, NOUNS.size() - 1)];
    } while (usedNames.count(name));
    usedNames.insert(name);
    return name;
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
        case DoorColor::Indigo: return "Indigo";
        case DoorColor::Violet: return "Violet";
        case DoorColor::Copper: return "Copper";
        case DoorColor::Silver: return "Silver";
        case DoorColor::Gold: return "Gold";
        case DoorColor::White: return "White";
        default: return "Unknown";
    }
}

DoorColor determineTreasureTier(int floorNumber) {
    if (floorNumber < 5) {
        return (getRandomDouble(0.0, 100.0) < 15.0) ? DoorColor::Silver : DoorColor::Copper;
    }
    double goldChance = (floorNumber - 4) * 0.5;
    if (goldChance > 30.0) goldChance = 30.0;
    double silverChance = 15.0 + (floorNumber / 2.0);
    if (silverChance > 50.0) silverChance = 50.0;
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
        case RoomType::Entrance: // Entrance doors are white from the other side
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
    
    int roomCount = getRandomInt(15, 25);
    std::set<std::string> usedNames;
    for (int i = 0; i < roomCount; ++i) {
        floor.rooms.emplace_back();
        floor.rooms.back().id = i;
    }

    // --- Phase 1: Node Creation & Type Assignment ---
    floor.rooms[0].type = RoomType::Entrance;
    floor.rooms[0].name = "Entrance";
    floor.startRoomId = floor.rooms[0].id;

    floor.rooms[1].type = RoomType::Boss;
    floor.rooms[1].name = "Guardian's Antechamber";
    floor.rooms[1].isDeadEndCandidate = true;

    int totalRewardRooms = (floorNumber > 0 && floorNumber % 5 == 0) ? getRandomInt(1, 2) : (getRandomDouble(0.0, 100.0) < (5.0 + (floorNumber / 2.0)) ? 1 : 0);
    
    int currentRoomIdx = 2;
    for (int i = 0; i < totalRewardRooms; ++i) {
        if (currentRoomIdx < roomCount) {
            floor.rooms[currentRoomIdx].type = (getRandomInt(0, 1) == 0) ? RoomType::Treasure : RoomType::Special;
            floor.rooms[currentRoomIdx].isDeadEndCandidate = true;
            currentRoomIdx++;
        }
    }

    while (currentRoomIdx < roomCount) {
        int randType = getRandomInt(1, 3);
        if (randType == 1) floor.rooms[currentRoomIdx].type = RoomType::Combat;
        else if (randType == 2) floor.rooms[currentRoomIdx].type = RoomType::Puzzle;
        else floor.rooms[currentRoomIdx].type = RoomType::Trap;
        currentRoomIdx++;
    }

    std::shuffle(floor.rooms.begin() + 1, floor.rooms.end(), rng);
    
    for(auto& room : floor.rooms) {
        if (room.type != RoomType::Entrance && room.type != RoomType::Boss) {
            room.name = generateRoomName(usedNames);
        }
    }

    std::map<int, int> id_to_idx;
    for(int i = 0; i < floor.rooms.size(); ++i) {
        id_to_idx[floor.rooms[i].id] = i;
    }

    int entranceIdx = id_to_idx.at(floor.startRoomId);
    int bossRoomIdx = -1;
    for(int i = 0; i < floor.rooms.size(); ++i) {
        if (floor.rooms[i].type == RoomType::Boss) {
            bossRoomIdx = i;
            break;
        }
    }
    
    // --- Phase 2 & 3: Build Progression Graph ---
    std::vector<int> connectedIndices = {entranceIdx};
    std::vector<int> unconnectedIndices;
    for(int i = 0; i < floor.rooms.size(); ++i) if(i != entranceIdx) unconnectedIndices.push_back(i);
    
    int mainPathLength = getRandomInt(3, 4);
    int lastRoomIdx = entranceIdx;
    for(int i = 0; i < mainPathLength; ++i) {
        auto it = std::find_if(unconnectedIndices.begin(), unconnectedIndices.end(), [&](int idx){
            return idx != bossRoomIdx && !floor.rooms[idx].isDeadEndCandidate;
        });
        if (it == unconnectedIndices.end()) break;
        
        int nextRoomIdx = *it;
        floor.rooms[lastRoomIdx].connections.push_back({DoorColor::Unknown, floor.rooms[nextRoomIdx].id});
        floor.rooms[nextRoomIdx].connections.push_back({DoorColor::Unknown, floor.rooms[lastRoomIdx].id});
        connectedIndices.push_back(nextRoomIdx);
        unconnectedIndices.erase(it);
        lastRoomIdx = nextRoomIdx;
    }
    
    if (bossRoomIdx != -1) {
        floor.rooms[lastRoomIdx].connections.push_back({DoorColor::Unknown, floor.rooms[bossRoomIdx].id});
        floor.rooms[bossRoomIdx].connections.push_back({DoorColor::Unknown, floor.rooms[lastRoomIdx].id});
        connectedIndices.push_back(bossRoomIdx);
        auto it = std::find(unconnectedIndices.begin(), unconnectedIndices.end(), bossRoomIdx);
        if (it != unconnectedIndices.end()) unconnectedIndices.erase(it);
    }

    while(!unconnectedIndices.empty()) {
        int fromIdx = connectedIndices[getRandomInt(0, connectedIndices.size() - 1)];
        int toIdx = unconnectedIndices.front();
        if (fromIdx == bossRoomIdx) continue;
        floor.rooms[fromIdx].connections.push_back({DoorColor::Unknown, floor.rooms[toIdx].id});
        floor.rooms[toIdx].connections.push_back({DoorColor::Unknown, floor.rooms[fromIdx].id});
        connectedIndices.push_back(toIdx);
        unconnectedIndices.erase(unconnectedIndices.begin());
    }

    // --- Phase 4: Add Loops ---
    int extraConnections = getRandomInt(roomCount / 2, roomCount);
    for(int k = 0; k < extraConnections; ++k) {
        int idx1 = getRandomInt(0, roomCount - 1);
        int idx2 = getRandomInt(0, roomCount - 1);

        if (idx1 == idx2 || floor.rooms[idx1].connections.size() >= 5 || floor.rooms[idx2].connections.size() >= 5) continue;

        bool alreadyConnected = false;
        for(const auto& conn : floor.rooms[idx1].connections) if(conn.destinationRoomId == floor.rooms[idx2].id) alreadyConnected = true;
        if(alreadyConnected) continue;

        DoorColor colorFor1 = colorFromRoomType(floor.rooms[idx2].type, floorNumber);
        DoorColor colorFor2 = colorFromRoomType(floor.rooms[idx1].type, floorNumber);

        bool clash1 = false;
        for(const auto& conn : floor.rooms[idx1].connections) {
            if (colorFromRoomType(floor.rooms[id_to_idx.at(conn.destinationRoomId)].type, floorNumber) == colorFor1) clash1 = true;
        }

        bool clash2 = false;
        for(const auto& conn : floor.rooms[idx2].connections) {
            if (colorFromRoomType(floor.rooms[id_to_idx.at(conn.destinationRoomId)].type, floorNumber) == colorFor2) clash2 = true;
        }

        if (!clash1 && !clash2) {
            floor.rooms[idx1].connections.push_back({DoorColor::Unknown, floor.rooms[idx2].id});
            floor.rooms[idx2].connections.push_back({DoorColor::Unknown, floor.rooms[idx1].id});
        }
    }

    // --- Phase 5: ENTRANCE ROOM FIX ---
    std::vector<RoomType> desiredTypes = {RoomType::Combat, RoomType::Puzzle, RoomType::Trap};
    while (floor.rooms[entranceIdx].connections.size() < 3) {
        bool connectedSomething = false;
        for (RoomType targetType : desiredTypes) {
            DoorColor targetColor = colorFromRoomType(targetType, floorNumber);
            
            bool alreadyHasColor = false;
            for(const auto& conn : floor.rooms[entranceIdx].connections) {
                if (colorFromRoomType(floor.rooms[id_to_idx.at(conn.destinationRoomId)].type, floorNumber) == targetColor) alreadyHasColor = true;
            }
            if (alreadyHasColor) continue;

            for (int i = 0; i < floor.rooms.size(); ++i) {
                if (i == entranceIdx || floor.rooms[i].type != targetType) continue;

                bool alreadyConnected = false;
                for(const auto& conn : floor.rooms[entranceIdx].connections) if(conn.destinationRoomId == floor.rooms[i].id) alreadyConnected = true;
                if(alreadyConnected) continue;

                floor.rooms[entranceIdx].connections.push_back({DoorColor::Unknown, floor.rooms[i].id});
                floor.rooms[i].connections.push_back({DoorColor::Unknown, floor.rooms[entranceIdx].id});
                connectedSomething = true;
                break; 
            }
        }
        if (!connectedSomething) break; // Avoid infinite loop if no valid connections can be made
    }


    // --- Phase 6: Finalize JSON ---
    json floorJson;
    floorJson["floorNumber"] = floor.floorNumber;
    floorJson["startRoomId"] = floor.startRoomId;
    json roomsJson = json::array();

    for (auto& room : floor.rooms) {
        json roomJson;
        roomJson["id"] = room.id;
        roomJson["name"] = room.name;
        roomJson["type"] = roomTypeToString(room.type);
        if (room.type == RoomType::Combat || room.type == RoomType::Boss) {
             roomJson["monsterId"] = "monster_" + std::to_string(room.id);
        }
        
        json connectionsJson = json::array();
        std::sort(room.connections.begin(), room.connections.end(), [](const Teleporter& a, const Teleporter& b){
            return a.destinationRoomId < b.destinationRoomId;
        });
        for (const auto& teleporter : room.connections) {
            int dest_idx = id_to_idx.at(teleporter.destinationRoomId);
            DoorColor color = colorFromRoomType(floor.rooms[dest_idx].type, floorNumber);
            json teleporterJson;
            teleporterJson["color"] = doorColorToString(color);
            teleporterJson["destinationId"] = teleporter.destinationRoomId;
            connectionsJson.push_back(teleporterJson);
        }
        roomJson["connections"] = connectionsJson;
        roomsJson.push_back(roomJson);
    }
    floorJson["rooms"] = roomsJson;

    std::ofstream outFile("../../data/floor" + std::to_string(floorNumber) + ".json");
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
