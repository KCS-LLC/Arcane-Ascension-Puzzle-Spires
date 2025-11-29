# UI and Resolution Refactor Plan

This document outlines the architectural plan for improving the UI, handling different screen resolutions, and managing player-facing text.

## 1. Unified Targeting System (In Progress)

- **Goal:** Create a single, scalable system for handling all abilities that require player input on the game board.
- **Status:** The core system is implemented (`PlayMode::Targeting`, `TargetingRequest` struct). "Gust of Wind" is functional.
- **Next Steps:**
    - [x] Move targeting prompt text into `spells.json`.
    - [ ] Refactor "Quick Swap" to use the new system.
    - [x] Refine UI feedback for targeting (e.g., highlighting the selected row/column).

## 2. Centralized UI Text

- **Goal:** Store all player-facing UI text, especially ability-related prompts, in the JSON data files instead of hardcoding them in C++.
- **Implementation:**
    - **`spells.json`:** Add a `"targeting_prompt"` field to spells that require it.
    - **`DataManager`:** Load this new field into the `Spell` struct.
    - **`UIManager`:** Use the `targeting_prompt` from the active ability's `Spell` data to display instructions.
- **Benefits:** Scalability, consistency, easier localization in the future.

## 3. Resolution and Aspect Ratio Handling

- **Goal:** Make the game adaptable to different window sizes and display modes (windowed, fullscreen, borderless) while maintaining a consistent aspect ratio for the core gameplay area.
- **Implementation Plan:**
    1.  **`config.json`:** Create a configuration file to store user settings (`width`, `height`, `display_mode`). The game will read this on startup and update it when settings change.
    2.  **Dynamic Layout:** Refactor all UI positioning code in `UIManager` to calculate positions and sizes relative to the current window size (`m_window.getSize()`) instead of hardcoded constants.
    3.  **Enforce Aspect Ratio:** Use an `sf::View` to manage the rendering viewport. This will create a "letterbox" or "pillarbox" effect if the window's aspect ratio does not match the game's design aspect ratio (e.g., 16:9), preventing visual distortion.
    4.  **UI Scaling:** Investigate a scaling factor for UI elements (fonts, buttons) based on the difference between a "base resolution" (e.g., 1280x720) and the user's current resolution to maintain readability.

## 4. Dedicated Message Panel

- **Goal:** Create a dedicated, persistent UI panel at the bottom of the screen for prompts, messages, and other contextual information.
- **Implementation:**
    - This should be done *after* the resolution refactor.
    - The screen layout will be redesigned to reserve a fixed-height area (e.g., the bottom 100 pixels) for this panel.
    - The main gameplay area (board, side panels) will be resized and repositioned to fit within the remaining space.
    - All targeting prompts and other player-facing messages will be rendered in this new panel.
- **Benefits:** Guarantees readability, provides a consistent focal point for player information, and offers a flexible space for future UI needs (narrative, hints, etc.).
