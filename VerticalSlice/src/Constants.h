#pragma once

constexpr int TILE_SIZE = 64;
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

enum class GameMode {
    Judgement,
    Exploration,
    Combat
};

enum class GameState {
    Intro,
    Trial,
    Summary,
    AttunementReveal,
    Playing,
    GameOver
};
