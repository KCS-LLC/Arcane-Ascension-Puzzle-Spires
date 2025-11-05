
# Arcane Ascension: The Puzzle Spires - Vertical Slice Prototype

This project is a C++ prototype for the game "Arcane Ascension: The Puzzle Spires". The goal of this vertical slice is to implement and test the core match-3 combat loop.

## 1. Core Features to Implement
- A functional 8x8 match-3 board.
- Player and Monster entities with HP.
- Turn-based combat governed by a Speed mechanic.
- Basic combat actions:
    - Matching **Skull** gems deals damage to the monster.
    - Matching **Mana** gems adds resources to the player.
    - The monster attacks after a set number of player actions.
- A win/loss condition (HP reaching zero).

## 2. Dependencies

This project requires the **SFML (Simple and Fast Multimedia Library)** for graphics, windowing, and input.

### SFML Setup Instructions:

1.  **Download SFML:** Go to the official SFML website (`https://www.sfml-dev.org/download.php`) and download the latest stable version for your compiler (e.g., Visual C++ 17 (2022), GCC). Make sure to get the correct 32-bit or 64-bit version that matches your compiler.

2.  **Extract the Library:** Extract the downloaded archive to a stable location on your computer (e.g., `C:\libs\SFML`).

3.  **Configure Your Project:** You will need to configure your compiler's include and library paths to point to the SFML directory.
    *   **Include Path:** Point this to the `SFML\include` directory (e.g., `C:\libs\SFML\include`).
    *   **Library Path:** Point this to the `SFML\lib` directory (e.g., `C:\libs\SFML\lib`).

4.  **Link Libraries:** You will need to link the SFML libraries. For a basic project, you'll typically need to link:
    *   `sfml-graphics-d.lib` (for debug builds)
    *   `sfml-window-d.lib` (for debug builds)
    *   `sfml-system-d.lib` (for debug builds)
    *   (Use the non `-d` versions for release builds, e.g., `sfml-graphics.lib`)

5.  **DLLs:** Copy the `.dll` files from the `SFML\bin` directory to the same directory as your final executable.

## 3. How to Compile (Example using g++)

This is a sample command. You will need to adjust the paths to match your SFML installation.

```bash
g++ -g src/*.cpp -o game.exe -I"C:/libs/SFML/include" -L"C:/libs/SFML/lib" -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
```

After compiling, ensure the SFML DLLs are in the same folder as `game.exe`, then run the executable.
