#include "PCH.h"
#include "TimeManager.h"
#include "Game.h" // Include Game for the callback
#include <iomanip>
#include <sstream>
#include <algorithm> // For std::sort and std::remove_if

TimeManager::TimeManager() {
    // The GameTime struct's default constructor already initializes time to the start of the game.
}

void TimeManager::advanceTime(int timeUnits, Game& game) {
    if (timeUnits <= 0) {
        return;
    }

    long long targetTime = m_currentTime.totalTimeUnits + timeUnits;

    // Process events that occur during this time block
    processEvents(game);

    m_currentTime.totalTimeUnits += timeUnits;
    m_currentTime.second += timeUnits;

    // Handle rollovers
    m_currentTime.minute += m_currentTime.second / SECONDS_PER_MINUTE;
    m_currentTime.second %= SECONDS_PER_MINUTE;

    m_currentTime.hour += m_currentTime.minute / MINUTES_PER_HOUR;
    m_currentTime.minute %= MINUTES_PER_HOUR;

    m_currentTime.day += m_currentTime.hour / HOURS_PER_DAY;
    m_currentTime.hour %= HOURS_PER_DAY;

    // Our calendar has 50 days per month (5 weeks * 10 days)
    m_currentTime.month += (m_currentTime.day - 1) / (DAYS_PER_WEEK * WEEKS_PER_MONTH);
    m_currentTime.day = ((m_currentTime.day - 1) % (DAYS_PER_WEEK * WEEKS_PER_MONTH)) + 1;

    m_currentTime.year += (m_currentTime.month - 1) / MONTHS_PER_YEAR;
    m_currentTime.month = ((m_currentTime.month - 1) % MONTHS_PER_YEAR) + 1;
}

void TimeManager::addEvent(const TimeEvent& newEvent) {
    m_events.push_back(newEvent);
    // Sort events by trigger time to process them in order
    std::sort(m_events.begin(), m_events.end(), std::greater<TimeEvent>());
}

void TimeManager::processEvents(Game& game) {
    // Process all events that should have fired by the new current time
    while (!m_events.empty() && m_events.back().triggerTime <= m_currentTime.totalTimeUnits) {
        TimeEvent event = m_events.back();
        m_events.pop_back();

        // Handle the event via callback to the Game object
        game.handleTimeEvent(event);
    }
}

const GameTime& TimeManager::getCurrentTime() const {
    return m_currentTime;
}

std::string TimeManager::getDateString() const {
    std::string monthName;
    switch (m_currentTime.month) {
        case 1: monthName = "Primaver"; break;
        case 2: monthName = "Floreal"; break;
        case 3: monthName = "Aestus"; break;
        case 4: monthName = "Caldus"; break;
        case 5: monthName = "Fructus"; break;
        case 6: monthName = "Folium"; break;
        case 7: monthName = "Gelu"; break;
        case 8: monthName = "Nix"; break;
        default: monthName = "Unknown"; break;
    }
    return monthName + " " + std::to_string(m_currentTime.day) + ", AY " + std::to_string(m_currentTime.year);
}

std::string TimeManager::getTimeString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << m_currentTime.hour << ":"
       << std::setfill('0') << std::setw(2) << m_currentTime.minute;
    return ss.str();
}

const std::vector<TimeEvent>& TimeManager::getEvents() const {
    return m_events;
}
