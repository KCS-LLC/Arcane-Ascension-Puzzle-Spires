#pragma once

struct GameTime {
    int year = 1;
    int month = 1;
    int day = 1;
    int hour = 8;
    int minute = 0;
    int second = 0;

    // Total time units elapsed since the start of the game.
    // Can be used for calculating event triggers without repeated calendar conversions.
    long long totalTimeUnits = 0;
};
