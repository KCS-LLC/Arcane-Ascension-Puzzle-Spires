# Build Environment & Configuration

This document outlines the specific technical environment required to successfully compile and run the Vertical Slice prototype. Adhering to these configurations will prevent common compilation and runtime errors.

---

## 1. Compiler

The project is built using the Clang C++ compiler provided by MSYS2.

-   **Compiler:** `clang++.exe`
-   **Version:** `21.1.5`

---

## 2. SFML Library

The project is linked against a specific version of the SFML library, compiled from source with Clang to ensure compatibility.

-   **SFML Version:** `3.0.2` (compiled with Clang)
-   **SFML Source Path:** `C:/dev/SFML`
-   **SFML Build Path:** `C:/dev/sfml-build-clang`

---

## 3. Asset Locations

All game assets (fonts, sprites, etc.) must be located in a directory named `assets` within the same directory as the game executable.

-   **Relative Path:** `./assets/`
-   **Example:** For the executable at `C:/Users/renga/SteamGame/VerticalSlice/game.exe`, the font must be at `C:/Users/renga/SteamGame/VerticalSlice/assets/OpenSans-Regular.ttf`.

---

## 3. Build Process (CMake)

This project uses CMake to generate build files. The recommended toolchain on Windows is MSYS2 with Clang and Ninja.

1.  **Build SFML from Source (First Time Setup):**
    Since the project uses Clang, SFML must also be compiled with Clang.

    a.  **Create SFML Build Directory:**
        ```bash
        mkdir C:\dev\sfml-build-clang
        ```

    b.  **Configure SFML with CMake:**
        ```bash
        cmake -S C:\dev\SFML -B C:\dev\sfml-build-clang -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=OFF
        ```

    c.  **Build SFML with Ninja:**
        ```bash
        ninja -C C:\dev\sfml-build-clang
        ```
    This will produce Clang-compatible SFML libraries in `C:\dev\sfml-build-clang\lib`.

2.  **Configure Game Project with CMake:**
    From the `VerticalSlice` directory, create and navigate to a build directory, then run CMake to generate the Ninja build files.

    ```bash
    mkdir build
    cd build
    cmake .. -G "Ninja" -DCMAKE_CXX_COMPILER=clang++
    ```
    *Note: `CMAKE_CXX_CLANG_TIDY` is automatically set in `CMakeLists.txt`.*
The final executable, `game.exe`, along with the required `assets` and `data` directories, will be located in the `build` directory.


## Dependencies

*   **Clang C++ Compiler:** Version 21.1.5 or newer, for compilation and PCH generation.
*   **Ninja Build Tool:** For faster incremental builds.
*   **SFML 3.0.2:** The core graphics and windowing library (compiled with Clang).
*   **nlohmann/json:** A header-only library for parsing JSON data. The file `json.hpp` is located in the `src` directory and is required for the `DataManager`.



## Precompiled Headers (PCH)

To significantly speed up compilation times and simplify header management, this project uses precompiled headers. The PCH is generated and utilized by the Clang compiler, ensuring full compatibility with Clang-Tidy.

-   **PCH File:** `src/PCH.h`
-   **Configuration:** The `CMakeLists.txt` file is configured via `target_precompile_headers` to first compile `PCH.h` and then automatically include it in all other source files.

The `PCH.h` file includes common, stable headers that are used throughout the project, such as:
*   SFML library headers
*   C++ Standard Library headers (`<vector>`, `<string>`, `<map>`, etc.)
*   Stable project-specific headers (`Constants.h`, `Structs.h`)

This approach avoids the need to repeatedly include these common headers in every `.cpp` file, making the build process much more efficient. When adding new, widely-used and stable headers, they should be added to `PCH.h`. Headers that are specific to a single `.cpp` file should be included directly in that file.

## Failed Refactoring Attempt (Gem/Tile Data Structure)

An attempt was made to refactor the core gem/tile data structure to support advanced mechanics like Auras and Special Tiles. This involved significant changes to `Gem.h`, `Player.h`, `Player.cpp`, `Board.h`, `Board.cpp`, `Game.h`, `Game.cpp`, `DataManager.h`, `DataManager.cpp`, `StringUtils.h`, and `UIManager.h`.

Despite thorough efforts to correctly implement the new hierarchical type system, manage header dependencies (including converting direct member objects to `std::unique_ptr` where necessary), and resolve numerous compilation errors, the project consistently failed to link. The primary symptom was a series of "undefined reference to" errors for various methods within the `DataManager` class.

**Suspected Causes:**

*   **C++ ABI Mismatch:** Incompatibilities in the Application Binary Interface (ABI) used by different parts of the MinGW/GCC toolchain for standard library types (e.g., `std::string`) or other components.
*   **Subtle Toolchain/Environment Issue:** A deep-seated problem within the build environment that prevented the linker from finding the compiled definitions of `DataManager` methods, even when object files were correctly generated.

**Resolution:**

The project was rolled back to the previous stable commit (before the refactoring attempt) to restore a working build. Further investigation of such linker issues would require advanced debugging tools (e.g., `nm`, `ldd` on Linux/WSL, or a full IDE debugger) and human expertise to diagnose the root cause of the ABI mismatch or toolchain problem.

