# Vertical Slice - Arcane Ascension

This directory contains the playable vertical slice for the game "Arcane Ascension".

## Current Status (November 7, 2025)
The project is in a stable, compiling state. The initial phase of UI polish for the combat screen is complete, including dynamic stat gauges, spell readiness indicators, monster name display, and a monster attack indicator.

The project is now moving into the first major feature implementation: **Procedural Spire Generation**. Please refer to `NEXT_STEPS.md` for a detailed development log and the current incremental plan.

### Core Features Implemented:
*   **Puzzle Board:** A fully functional 8x8 match-3 board.
*   **Combat:** Players can match skull gems to deal direct damage to a monster.
*   **Mana & Spells:** Matching colored gems grants mana, which can be used to cast spells with varying effects.
*   **Animations:** The game includes animations for gem swaps, matches, and board refills.
*   **Data-Driven Design:** Monster stats and spell definitions are loaded from external JSON files.
*   **Modular Architecture:** The codebase is separated into managers for UI (`UIManager`), data (`DataManager`), and core game logic (`Game`), making it easier to maintain and expand.
*   **UI Feedback:** The UI provides clear visual feedback for player/monster stats, spell readiness, and monster attacks.

## How to Build and Run
1.  **Environment:** Ensure you have a C++ compiler (like g++) and SFML 3.0+ installed and configured.
2.  **Build:** Use the compile command specified in `BUILD_ENVIRONMENT.md`.
3.  **Run:** Execute the generated `game.exe` from within the `VerticalSlice` directory.

## Next Steps
Please refer to `NEXT_STEPS.md` for the detailed development roadmap. The next major focus is on building the core roguelike loop through procedural spire generation.