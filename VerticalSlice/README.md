# Vertical Slice - Arcane Ascension

This directory contains the playable vertical slice for the game "Arcane Ascension".

## Current Status (November 7, 2025)
The project has been successfully reverted to a stable, compiling state after encountering significant build issues. All previous compilation errors have been resolved. The core features and asset integration described below are fully functional. Please refer to `NEXT_STEPS.md` for a detailed development log and the current incremental plan.

### Core Features Implemented:
*   **Puzzle Board:** A fully functional 8x8 match-3 board.
*   **Combat:** Players can match skull gems to deal direct damage to a monster.
*   **Mana & Spells:** Matching colored gems grants mana, which can be used to cast spells with varying effects.
*   **Animations:** The game includes animations for gem swaps, matches, and board refills.
*   **Data-Driven Design:** Monster stats and spell definitions are loaded from external JSON files.
*   **Modular Architecture:** The codebase is separated into managers for UI (`UIManager`), data (`DataManager`), and core game logic (`Game`), making it easier to maintain and expand.

## How to Build and Run
1.  **Environment:** Ensure you have a C++ compiler (like g++) and SFML 3.0+ installed and configured.
2.  **Build:** Use the compile command specified in `BUILD_ENVIRONMENT.md`.
3.  **Run:** Execute the generated `game.exe` from within the `VerticalSlice` directory.

## Next Steps
Please refer to `NEXT_STEPS.md` for the detailed development roadmap. The next major focus is on enhancing the player experience and visual polish.