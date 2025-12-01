#ifndef PCH_H
#define PCH_H

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

// Third-Party
#include "json.hpp"

// Standard Library
#include <algorithm>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

// Project-specific, but very stable headers
#include "Constants.h"
#include "DataManager.h"

// Custom comparator for sf::Vector2i to be used in std::set
struct Vector2iCompare {
    bool operator() (const sf::Vector2i& a, const sf::Vector2i& b) const {
        if (a.x < b.x) return true;
        if (a.x > b.x) return false;
        return a.y < b.y;
    }
};

#endif // PCH_H