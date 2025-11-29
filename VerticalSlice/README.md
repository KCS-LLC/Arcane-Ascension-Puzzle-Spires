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

### **CRITICAL: Build Environment Note**
**This project requires a specific Clang-based toolchain on Windows (MSYS2 `mingw-w64`) to compile, run, and debug correctly. Using a different compiler, linker, or runtime environment (like standard G++ or the `ucrt64` MSYS2 toolchain) will result in compilation or runtime errors.**

Before proceeding, you **must** follow the detailed setup instructions in the `BUILD_ENVIRONMENT.md` file. This document covers:
1.  Installing the correct MSYS2 toolchain components (`clang`, `lld`, `gdb`).
2.  Building SFML from source with the correct configuration.
3.  Configuring the game project and the VS Code debugger.

Adhering to these steps is essential for a working build.

1.  **Environment:** Ensure you have the MSYS2 environment with the Clang C++ compiler and Ninja build tool installed.
2.  **Compile SFML:** This project requires a specific version of the SFML library compiled with Clang. Follow the detailed steps in `BUILD_ENVIRONMENT.md` to compile SFML from source.
3.  **Build & Run:** Use the build commands specified in `BUILD_ENVIRONMENT.md` to compile and run the game. The final executable will be located in the `build` directory.

## Next Steps
The foundational exploration and combat loop is complete. The next major focus will be on fleshing out the non-combat room encounters (Treasure, Puzzle, Trap, and the new Challenge rooms) and implementing a greater variety of monsters.