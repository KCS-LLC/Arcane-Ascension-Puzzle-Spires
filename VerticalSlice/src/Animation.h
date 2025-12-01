#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

// Defines *what* UI element or area is being targeted.
enum class AnimationTarget {
    PlayerHpBar,
    MonsterHpBar
};

// Defines *what kind* of animation to play.
enum class AnimationType {
    HpSweep
};

// A self-contained structure holding all data for one animation instance.
struct Animation {
    AnimationType type;
    AnimationTarget target;
    sf::Time lifetime;      // How long the animation lasts
    sf::Clock clock;        // Tracks the animation's age
    sf::Color color;        // The primary color of the effect
    float startValue;       // The state before the change (e.g., old HP)
    float endValue;         // The state after the change (e.g., new HP)
    float maxValue;         // The maximum value for percentage calculations (e.g., max HP)
};
