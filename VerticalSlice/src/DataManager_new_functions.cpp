#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include "DataManager.h"
#include "json.hpp"

using json = nlohmann::json;

// (Other constructors and functions will be here)

bool DataManager::loadPrimaryGemTypes(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open primary gem types file: " << path << std::endl;
        return false;
    }
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            m_primaryGemTypes[static_cast<PrimaryGemType>(item.at("id").get<int>())] = item.at("name").get<std::string>();
        }
    } catch (json::exception& e) {
        std::cerr << "JSON error in primary gem types file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool DataManager::loadSecondaryGemTypes(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open secondary gem types file: " << path << std::endl;
        return false;
    }
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            SecondaryGemTypeData sgtd = item.get<SecondaryGemTypeData>();
            m_secondaryGemTypes[sgtd.id] = sgtd;
        }
    } catch (json::exception& e) {
        std::cerr << "JSON error in secondary gem types file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool DataManager::loadGemCatalog(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open gem catalog file: " << path << std::endl;
        return false;
    }
    try {
        json data = json::parse(f);
        for (const auto& item : data) {
            GemCatalogEntry gce = item.get<GemCatalogEntry>();
            m_gemCatalog[static_cast<GemSubType>(gce.id)] = gce;
        }
    } catch (json::exception& e) {
        std::cerr << "JSON error in gem catalog file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

// (Rest of the DataManager.cpp file will go here)
