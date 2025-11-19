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

## 3. Build Process (CMake)

This project uses CMake to generate build files. The recommended toolchain on Windows is MSYS2 with MinGW-w64.

1.  **Create a Build Directory:**
    From the `VerticalSlice` directory, create and navigate to a build directory.
    ```bash
    mkdir build
    cd build
    ```

2.  **Configure with CMake:**
    Run CMake to generate the Makefiles.
    ```bash
    cmake .. -G "MinGW Makefiles"
    ```

3.  **Compile the Project:**
    Use `mingw32-make` to compile the executable.
    ```bash
    mingw32-make
    ```
The final executable, `game.exe`, along with the required `assets` and `data` directories, will be located in the `build` directory.


## Dependencies

*   **SFML 3.0.2:** The core graphics and windowing library.
*   **nlohmann/json:** A header-only library for parsing JSON data. The file `json.hpp` is located in the `src` directory and is required for the `DataManager`.

*Note: When adding new `.cpp` files to the `src` directory, they must be added to this command.*
