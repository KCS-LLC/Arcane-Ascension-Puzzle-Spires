#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "GemSubType.h" // For gem types
#include <functional>

// Defines what kind of board animation is playing.
enum class BoardAnimationType {
    Swap,
    Destroy,
    Fall,
    Transform,
    RotateRow,
    RotateColumn
};

// A self-contained structure holding all data for one animation instance.
struct BoardAnimation {
    BoardAnimationType type;
    sf::Clock clock;
    sf::Time duration;

    // --- Data for specific animation types ---
    // Used by Swap, Fall, Move
    sf::Vector2i startPos;
    sf::Vector2i endPos;

    // Used by Destroy
    sf::Vector2i position;
    GemSubType destroyedGemType = GemSubType::None;
    
    // Used by Transform
    GemSubType fromType;
    GemSubType toType;

    // Used by RotateRow/RotateColumn
    int index;      // Row or column index
    int direction;  // 1 for right/down, -1 for left/up

    // Callback to execute when the animation completes.
    std::function<void()> onComplete;
};
