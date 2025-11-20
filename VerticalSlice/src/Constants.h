#pragma once

constexpr int TILE_SIZE = 64;
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

enum class GameState {
    Loading,
    Animating,
    Playing,
    GameOver,
    Judgement_Intro,
    Judgement_Trial,
    Judgement_Summary,
    Judgement_AttunementReveal,
    Exploration,
    Combat,
    Treasure,
    Special,
    Puzzle,
    Trap,
    Sanctuary,
    AgilityChallenge,
    EnduranceChallenge,
    MagicChallenge,
    Exit
};
