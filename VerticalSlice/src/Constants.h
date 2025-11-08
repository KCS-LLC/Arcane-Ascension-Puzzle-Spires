#ifndef CONSTANTS_H
#define CONSTANTS_H

// Game constants
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;
const int TILE_SIZE = 64;

// Enum for the main game states
enum class GameState {
    Judgement,
    Playing,
    Animating,
    GameOver,
    Exploration,
    Treasure,
    Special,
    Puzzle,
    Trap,
    Sanctuary,
    AgilityChallenge,
    EnduranceChallenge,
    MagicChallenge
};

#endif // CONSTANTS_H
