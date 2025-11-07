# Vertical Slice - Arcane Ascension

This directory contains the playable vertical slice for the game "Arcane Ascension".

## Current Status (November 7, 2025)
The project is in a stable, compiling state. The core gameplay loop, combining procedural dungeon exploration with match-3 combat, is now implemented.

### Core Features Implemented:
*   **Procedural Spire Generation:** The game creates a unique, multi-room floor layout for each run, governed by a set of robust rules. This includes an expanded variety of room types (Combat, Puzzle, Trap, Agility, Endurance, Magic, etc.) and guarantees unique, thematic door connections for each room.
*   **Exploration Gameplay:** Players navigate between uniquely named rooms using a dedicated exploration screen. The system tracks visited and cleared rooms, preventing events from re-triggering and updating door labels to show the names of explored destinations.
*   **Puzzle Board:** A fully functional 8x8 match-3 board.
*   **Combat:** Players can match skull gems to deal direct damage to a monster.
*   **Mana & Spells:** Matching colored gems grants mana, which can be used to cast spells with varying effects.
*   **Animations:** The game includes animations for gem swaps, matches, and board refills.
*   **Data-Driven Design:** Monster stats, spell definitions, and the entire floor layout are loaded from external JSON files.
*   **Modular Architecture:** The codebase is separated into managers for UI (`UIManager`), data (`DataManager`), and core game logic (`Game`), making it easier to maintain and expand.
*   **UI Feedback:** The UI provides clear visual feedback for player/monster stats, spell readiness, and monster attacks, with distinct screens for combat and exploration.

## How to Build and Run
1.  **Environment:** Ensure you have a C++ compiler (like g++) and SFML 3.0+ installed and configured.
2.  **Build:** Use the compile command specified in `BUILD_ENVIRONMENT.md`.
3.  **Run:** Execute the generated `game.exe` from within the `VerticalSlice` directory.

## Next Steps
The foundational exploration and combat loop is complete. The next major focus will be on fleshing out the non-combat room encounters (Treasure, Puzzle, Trap, and the new Challenge rooms) and implementing a greater variety of monsters.