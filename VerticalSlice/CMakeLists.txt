# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(ArcaneAscensionVerticalSlice VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Manual SFML Configuration ---
# We bypass the problematic SFMLConfig.cmake by finding everything ourselves.

# 1. Define the path to our compiled SFML installation
set(SFML_ROOT "C:/SFML-3.0.2-mingw")

# 2. Define Include Directories
include_directories(${SFML_ROOT}/include)

# 3. Define Link Directories (where to find .a files)
link_directories(${SFML_ROOT}/lib)

# 4. Define all the libraries we need to link, based on the working g++ command
# For SFML, we use the static libraries (ending in -s)
set(SFML_LIBRARIES
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    opengl32
    freetype
    winmm
    gdi32
    user32
    kernel32
)

# --- Project Setup ---

# Add an executable target named "game"
add_executable(game "")

set(SOURCE_FILES
    src/Board.cpp
    src/DataManager.cpp
    src/Game.cpp
    src/Monster.cpp
    src/Player.cpp
    src/StringUtils.cpp
    src/UIManager.cpp
    src/main.cpp
    src/PCH.h
    src/Judgement.h
)
target_sources(game PRIVATE ${SOURCE_FILES})

# Enable Precompiled Headers
target_precompile_headers(game PRIVATE src/PCH.h)

# Add the SFML_STATIC definition, which is required
target_compile_definitions(game PRIVATE SFML_STATIC)

# Link the executable against all the libraries we defined
target_link_libraries(game PRIVATE ${SFML_LIBRARIES})

# --- Post-Build Steps ---
# Automatically copy assets and data to the build directory for convenience.
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_CURRENT_BINARY_DIR}/assets"
)
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/data"
    "${CMAKE_CURRENT_BINARY_DIR}/data"
)

message(STATUS "Project configured to build 'game.exe' with manually found SFML libraries.")