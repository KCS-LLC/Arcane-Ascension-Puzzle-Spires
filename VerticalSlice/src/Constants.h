#pragma once

constexpr int TILE_SIZE = 64;
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

enum class GameMode {
    JUDGEMENT,
    TOWER_CLIMB,
    GAME_OVER
};

enum class GameState {
    Intro,
    Trial,
    Summary,
    Judgement_TreasureRound,
    AttunementReveal,
    Exploration,
    Playing,
    GameOver,
    Treasure,
    Special,
    Puzzle,
    Trap,
    Sanctuary,
    AgilityChallenge,
    EnduranceChallenge,
    MagicChallenge
};
