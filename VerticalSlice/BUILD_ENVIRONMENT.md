# Build Environment & Configuration (Clang)

This document outlines the specific technical environment required to successfully compile, run, and debug the Vertical Slice prototype using the Clang toolchain on Windows with MSYS2. Adhering to these configurations is critical to prevent compilation, linking, and runtime errors.

---

## 1. Toolchain: MSYS2 with MinGW-w64 and Clang

The project is built using the Clang C++ compiler provided by the **`mingw-w64`** toolchain within MSYS2. It is essential to use this specific toolchain for compilers, linkers, libraries, and debuggers to ensure ABI compatibility.

-   **Compiler:** `clang++.exe` (from the `mingw-w64` toolchain)
-   **Linker:** `lld.exe` (from the `mingw-w64` toolchain)
-   **Debugger:** `gdb.exe` (from the `mingw-w64` toolchain)

### Initial Setup

Install the required toolchain components via the MSYS2 terminal:

```bash
# Installs Clang, LLD, and other core build tools for the mingw-w64 environment
pacman -S mingw-w64-x86_64-clang mingw-w64-x86_64-lld

# Installs the Ninja build tool
pacman -S mingw-w64-x86_64-ninja

# Installs the GDB debugger for the mingw-w64 environment
pacman -S mingw-w64-x86_64-gdb
```

---

## 2. SFML Library (Build from Source)

The project must be linked against a version of SFML compiled from source with the exact same `mingw-w64` Clang toolchain to ensure compatibility. A **Debug** build is required for development.

-   **SFML Version:** `3.0.2` or compatible
-   **SFML Source Path:** `C:/dev/SFML`
-   **SFML Build Path:** `C:/dev/sfml-build-clang`

### SFML Build Process

1.  **Create SFML Build Directory:**
    ```powershell
    # Ensure the directory is clean before configuring
    Remove-Item -Path C:\dev\sfml-build-clang -Recurse -Force
    New-Item -Path C:\dev\sfml-build-clang -ItemType Directory
    ```

2.  **Configure SFML with CMake:**
    Run this command from the project's root directory. It is configured to create a static, debug build of SFML.
    ```bash
    cmake -S C:/dev/SFML -B C:/dev/sfml-build-clang -G "Ninja" `
      -DCMAKE_BUILD_TYPE=Debug `
      -DBUILD_SHARED_LIBS=OFF `
      -DCMAKE_C_COMPILER=clang `
      -DCMAKE_CXX_COMPILER=clang++ `
      -DSFML_USE_STATIC_STD_LIBS=ON
    ```

3.  **Build SFML with Ninja:**
    ```bash
    ninja -C C:\dev\sfml-build-clang
    ```
    This will produce Clang-compatible SFML debug libraries (e.g., `libsfml-graphics-s-d.a`) in `C:\dev\sfml-build-clang\lib`.

---

## 3. Game Build Process (CMake)

The game project itself is also built with CMake and Ninja.

1.  **Configure Game Project with CMake:**
    From the project's root directory, run the following command:
    ```bash
    cmake -S VerticalSlice -B VerticalSlice/build -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
    ```

2.  **Build Game with Ninja:**
    ```bash
    ninja -C VerticalSlice/build
    ```
    The final executable, `game.exe`, will be located in the `VerticalSlice/build` directory.

---

## 4. Runtime & Debugging

### Runtime Dependencies (DLLs)

To run `game.exe` outside of the MSYS2 terminal (e.g., by double-clicking or from a debugger), essential runtime DLLs from the `mingw64` toolchain must be present in the same directory as the executable.

Copy the following files from `C:/msys64/mingw64/bin` to `VerticalSlice/build`:
-   `libstdc++-6.dll`
-   `libgcc_s_seh-1.dll`
-   `libwinpthread-1.dll`

### VS Code Debugger Setup (`.vscode/`)

To debug the application in Visual Studio Code, create two files in a `.vscode` directory at the project root.

#### `launch.json`
This file configures the debugger to launch the game, correctly pointing to the `mingw64` GDB and setting the `PATH` to find the runtime DLLs.

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch Game",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/VerticalSlice/build/game.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}/VerticalSlice/build",
            "environment": [
                {
                    "name": "PATH",
                    "value": "C:/msys64/mingw64/bin;${env:PATH}"
                }
            ],
            "externalConsole": true,
            "MIMode": "gdb",
            "miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

#### `tasks.json`
This file configures a default build task so you can compile the project by pressing `Ctrl+Shift+B`.

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build-game",
            "type": "shell",
            "command": "ninja",
            "args": [
                "-C",
                "${workspaceFolder}/VerticalSlice/build"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        }
    ]
}
```