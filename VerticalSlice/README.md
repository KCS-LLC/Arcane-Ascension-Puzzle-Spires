# Arcane Ascension - Vertical Slice

This project is a vertical slice of a match-3 combat RPG, "Arcane Ascension." It demonstrates the core gameplay loop, including the puzzle board, combat mechanics, spell casting, and a complete user interface.

## Features

*   **Match-3 Combat:** Match colored gems to gain mana and skull gems to deal direct damage.
*   **Spell Casting:** Use the mana you've collected to cast powerful damage-dealing spells.
*   **Speed-Based Turn System:** Player actions have a "speed cost." When the player's cumulative speed costs exceed the monster's Speed stat, the monster takes its turn.
*   **Dynamic UI:** A three-panel UI displays player stats, the game board, and monster stats, including a real-time gauge for the monster's next turn.
*   **Full Animation System:** All player actions, including gem swaps, destruction, and board refills, are fully animated.
*   **Robust Board Logic:** The game board can detect when no more moves are possible and will automatically reshuffle itself.

## How to Compile

To compile this project, you will need a C++ compiler (like G++) and the SFML library. For detailed instructions on setting up the environment and the exact compiler command, please refer to the `BUILD_ENVIRONMENT.md` file.

The general command is:
```bash
g++ -g -DSFML_STATIC "src/main.cpp" "src/Game.cpp" "src/Board.cpp" -o "game.exe" -I "path/to/sfml/include" -L "path/to/sfml/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -luser32 -lkernel32
```

## How to Play



*   **Swap Gems:** Click and drag a gem to swap it with an adjacent gem. Alternatively, click one gem and then click an adjacent gem.

*   **Cast Spells:** When you have enough mana, click the spell buttons in the left-hand panel to cast them.

*   **Objective:** Defeat the monster before it defeats you!



## Developer Notes



The codebase is currently in a stable, functional state. However, it is not yet optimized for future development. The `Game.cpp` file is overly large, and much of the game's data (spells, monster stats) is hardcoded directly into the source.



A major architectural refactoring is planned to address these issues. The goal is to separate concerns into more focused classes (like a `UIManager` and `DataManager`) and to move all game data into external JSON files. For a detailed history of this effort and the revised plan, please see the `NEXT_STEPS.md` file.
