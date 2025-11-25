# Tile Visual Layer System

This document outlines the design for the multi-layered visual representation of gems (also referred to as tiles) on the game board. This system is designed to be extensible and clearly communicate various gameplay states to the player.

## Layer Rendering Order

Each tile on the board is composed of several visual layers, rendered in the following order from back to front. This ensures that gameplay-critical information and player feedback are always visible.

1.  **Background Layer** `(Lowest)`
2.  **Gem Layer**
3.  **Front Effect Layer**
4.  **Counter/Timer Layer**
5.  **Frame Layer** `(Highest)`

---

## Layer Descriptions

### 1. Background Layer
*   **Purpose:** To indicate the base power level or rank of the gem. This level affects the amount of mana or damage generated from a match.
*   **Visuals:** A solid colored rectangle behind the gem sprite.
*   **Progression:**
    *   **Level 1 (Quartz):** Transparent or black (no visible background).
    *   **Level 2:** White/Clear.
    *   **Level 3+:** Follows the ROYGBIV spectrum (Red, Orange, Yellow, Green, Blue, Indigo, Violet).

### 2. Gem Layer
*   **Purpose:** To represent the core type of the gem.
*   **Visuals:** The standard sprite for the gem type (e.g., Fire, Skull, Coin, Water).

### 3. Front Effect Layer
*   **Purpose:** To show a gameplay-altering status effect currently affecting the tile.
*   **Visuals:** A semi-transparent overlay sprite. For example, flames for a "Burning" effect, a sheet of ice for "Frozen," or green bubbles for "Poisoned."

### 4. Counter/Timer Layer
*   **Purpose:** To display a visual representation of a status effect's duration, which is based on the game's "time unit" system.
*   **Visuals:** A two-color horizontal bar across the top of the tile. A background color represents the full duration, and a foreground color represents the remaining time. As the effect's time value decrements, the foreground bar shrinks.
*   **Mechanics:** This is only visible when a `Front Effect` is active. It is not used for base gem multipliers.

### 5. Frame Layer
*   **Purpose:** To provide immediate, non-gameplay feedback to the player based on their actions. This layer is for UI state, not game mechanics.
*   **Visuals:** An outline or border rendered around the edge of the tile.
*   **Examples:**
    *   A solid colored frame for a "Selected" gem.
    *   A pulsing frame for a gem that is part of a "Valid Move Hint" (from the `Insight` ability).