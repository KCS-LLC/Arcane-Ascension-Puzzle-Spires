# PCH Refactoring Log - Nov 19, 2025

## Objective

To streamline the build process, reduce compilation times, and simplify header management by implementing a precompiled header (PCH) system.

## Summary of Changes

1.  **Created `src/PCH.h`:** A central header file was created to contain common and stable includes, such as the SFML library, the C++ Standard Library, and core project constants.

2.  **Updated `CMakeLists.txt`:** The build configuration was modified to support precompiled headers. This involved using `target_precompile_headers` to instruct CMake to compile `PCH.h` once and then automatically include it for all other source files.

3.  **Refactored Project Files:** All `.h` and `.cpp` files were cleaned up to remove redundant includes that are now handled by `PCH.h`. This simplifies the include directives at the top of each file and makes dependency management more straightforward.

## Outcome

The refactoring was successful. The project now compiles cleanly and benefits from the performance improvements of precompiled headers. This change also makes the codebase easier to maintain, as common headers are managed in a single location.
