# Build Environment & Configuration

This document outlines the specific technical environment required to successfully compile and run the Vertical Slice prototype. Adhering to these configurations will prevent common compilation and runtime errors.

---

## 1. Compiler

The project is built using the G++ compiler provided by MSYS2.

-   **Compiler:** `g++.exe (Rev8, Built by MSYS2 project)`
-   **Version:** `15.2.0`

---

## 2. SFML Library

The project is linked against a specific version of the SFML library. The syntax used in the codebase, particularly for event handling, is specific to this major version.

-   **SFML Version:** `3.0.2`
-   **Include Path:** `C:/Program Files (x86)/SFML/include`
-   **Library Path:** `C:/Program Files (x86)/SFML/lib`

---

## 3. Asset Locations

All game assets (fonts, sprites, etc.) must be located in a directory named `assets` within the same directory as the game executable.

-   **Relative Path:** `./assets/`
-   **Example:** For the executable at `C:/Users/renga/SteamGame/VerticalSlice/game.exe`, the font must be at `C:/Users/renga/SteamGame/VerticalSlice/assets/OpenSans-Regular.ttf`.

---

## 4. Build Command

The following command is used to compile the project from the `VerticalSlice` directory. It includes all necessary flags for static linking.

```bash
g++ -g -DSFML_STATIC "src/main.cpp" "src/Game.cpp" "src/Board.cpp" -o "game.exe" -I "C:/Program Files (x86)/SFML/include" -L "C:/Program Files (x86)/SFML/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -luser32 -lkernel32
```

*Note: When adding new `.cpp` files to the `src` directory, they must be added to this command.*
