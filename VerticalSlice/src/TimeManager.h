#pragma once

#include "GameTime.h"
#include <string>
#include <vector>
#include <functional>
#include <SFML/System/Vector2.hpp>

// Forward-declare the Game class for the callback
class Game;

enum class TimeEventType {
    BurningTile_Activation,
    BurningTile_Expire
};

struct TimeEvent {
    long long triggerTime; // The totalTimeUnits value when this event should fire
    TimeEventType type;
    sf::Vector2i coordinates; // e.g., the gem on the board to affect

    // For sorting
    bool operator>(const TimeEvent& other) const {
        return triggerTime > other.triggerTime;
    }
};

class TimeManager {
public:
    TimeManager();

    // Advances the game time by a given number of time units (seconds)
    void advanceTime(int timeUnits, Game& game); // Pass game object for callbacks

    // Adds a new event to the queue
    void addEvent(const TimeEvent& newEvent);
    
    // Processes all events that should have triggered by the current time
    void processEvents(Game& game);

    // Returns the current game time
    const GameTime& getCurrentTime() const;

    // Returns a formatted string for the date (e.g., "Month 1, Day 1, Year 1")
    std::string getDateString() const;

    // Returns a formatted string for the time (e.g., "08:00")
    std::string getTimeString() const;

    // Returns the raw event list (for rendering purposes)
    const std::vector<TimeEvent>& getEvents() const;

private:
    GameTime m_currentTime;
    std::vector<TimeEvent> m_events;

    // Constants for our 8x5x10 calendar
    static const int SECONDS_PER_MINUTE = 60;
    static const int MINUTES_PER_HOUR = 60;
    static const int HOURS_PER_DAY = 24;
    static const int DAYS_PER_WEEK = 10;
    static const int WEEKS_PER_MONTH = 5;
    static const int MONTHS_PER_YEAR = 8;
};
