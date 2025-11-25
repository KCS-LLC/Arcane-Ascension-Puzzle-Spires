# Game Time and Turn Mechanics

This document outlines the core concepts of how time, turns, and actions are handled in the game. The system is designed to be turn-based from the player's perspective, but it uses a more granular "time unit" system under the hood to allow for varied action costs and effect durations.

---

## Core Concepts

### Time Units
*   The fundamental unit of in-game time. Every action taken by the player has a `speedCost` measured in time units.
*   Making a standard gem swap is the baseline action and has a `BASE_SWAP_SPEED` (e.g., 20 time units).
*   Casting spells or using abilities can have different `speedCost` values, making some actions "faster" or "slower" than others.

### Player Actions
*   The game's timeline is driven exclusively by player actions. The game state only advances when the player makes a move (swaps a gem, casts a spell, etc.).
*   After each player action, the `speedCost` of that action is applied, and both monster and tile effects are processed.

### Monster Actions
*   Monsters have a "speed" stat, which represents the number of time units they must accumulate before they can take an action.
*   After each player action, the `speedCost` of the player's action is added to the monster's internal "action counter."
*   If the monster's action counter is greater than or equal to its speed stat, the monster immediately takes its turn. Its action counter is then reduced by its speed value.
*   This allows a player to potentially take multiple "fast" actions before a "slow" monster can react.

---

## Status Effect Timing System

Status effects on gems (like `Burning`, `Frozen`) are not based on a simple turn counter. They use the time unit system to determine their duration and when they trigger.

Each gem with a status effect has three time-related properties:

*   **`m_effectValue` (float):** The current remaining "time" on an effect. It is decremented by the `speedCost` of each player action.
*   **`m_effectMax` (float):** The starting value of `m_effectValue`. This is used to calculate the percentage for visual feedback (e.g., the duration bar).
*   **`m_periodicActivationValue` (float):** The "time unit" threshold for periodic effects.

### Example: Burning Tile
1.  **Activation:** A spell applies `Burning` to a gem.
    *   `m_periodicActivationValue` is set to `BASE_SWAP_SPEED` (e.g., 20.0f).
    *   `m_effectValue` and `m_effectMax` are set to `3 * BASE_SWAP_SPEED` (e.g., 60.0f).

2.  **Processing (after each player action):**
    *   Let's say the player makes a swap (cost: 20). The gem's `m_effectValue` is reduced from 60 to 40.
    *   The game checks if the `m_effectValue` has crossed a multiple of the `m_periodicActivationValue`. It has (it crossed the `60 - 20 = 40` threshold), so the `Burning` damage **triggers once**.
    *   Next, the player casts a quick spell (cost: 10). The `m_effectValue` is reduced from 40 to 30.
    *   The next threshold is `40 - 20 = 20`. Since 30 is not less than or equal to 20, the effect does not trigger.
    *   The player makes another swap (cost: 20). The `m_effectValue` is reduced from 30 to 10.
    *   The next threshold was 20. Since 10 is less than or equal to 20, the `Burning` damage **triggers once**.

3.  **Resolution:**
    *   When `m_effectValue` is depleted to 0 or less, the effect may have a final resolution action, and then it is cleared from the gem.
