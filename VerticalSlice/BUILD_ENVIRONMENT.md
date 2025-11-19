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

## Compiler Command

The command below includes all current source files.

```bash
g++ -g -DSFML_STATIC "src/main.cpp" "src/Game.cpp" "src/Board.cpp" "src/Player.cpp" "src/Monster.cpp" "src/DataManager.cpp" "src/UIManager.cpp" "src/StringUtils.cpp" -o "game.exe" -I "C:/SFML-3.0.2-mingw/include" -L "C:/SFML-3.0.2-mingw/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -luser32 -lkernel32
```

## Dependencies

*   **SFML 3.0.2:** The core graphics and windowing library.
*   **nlohmann/json:** A header-only library for parsing JSON data. The file `json.hpp` is located in the `src` directory and is required for the `DataManager`.

*Note: When adding new `.cpp` files to the `src` directory, they must be added to this command.*
