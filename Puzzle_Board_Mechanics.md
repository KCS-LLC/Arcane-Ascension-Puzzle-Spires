# Puzzle Board Mechanics

This document details the specific rules and interactions of the match-3 puzzle board, which is the core of the combat system. It defines how special tiles are created, their individual effects, and how they can be combined for powerful strategic plays.

---

## 1. The Gem Aura System

The quality of gems on the board is directly tied to the power level of the combatants. Higher-ranking characters and monsters will naturally cause gems with powerful Auras to appear, making them more valuable.

### 1.1. Aura Effect
An Aura acts as a **resource multiplier** for any gem it's attached to.

| Aura Rank | Multiplier |
| :--- | :--- |
| **None (Quartz)** | 1x (Base Value) |
| **Carnelian** | 2x |
| **Sunstone** | 3x |
| **Citrine** | 4x |
| **Emerald** | 5x |

This multiplier applies to mana gained, Skull damage dealt, and Coins collected.

### 1.2. Aura Generation
When a new gem is generated, the game performs a hierarchical check based on the character's Attunement Rank to determine if an Aura is applied. It always checks for the highest possible rank first.

*   **Generation Chance:** The chance for an Aura to manifest is based on the character's rank (5% at E-Rank, scaling up to 25% at A-Rank for each gemstone tier).
*   **Hierarchical Check:** The game checks for the highest rank first. If that fails, it checks for the next lowest, and so on.
*   **Example:** A **Sunstone C-Rank (15%)** character will first have a 15% chance to generate a Sunstone Aura. If that fails, they will then have a 25% chance (the max for Carnelian) to generate a Carnelian Aura. If both fail, a standard gem is created.

---

## 2. The Prerequisite for Special Tiles

A core rule of this system is that **special tiles are not created by default**. An heir must have an item equipped or a passive ability active that specifically enables the creation of one or more types of special tiles.

---

## 3. Creating Special Tiles

Assuming the heir has the requisite ability, special tiles are created by matching more than three gems at once.

*   **Match 4 Gems (in a line):** Creates a **Spear** gem.
*   **Match 5+ Gems (in an L or T shape):** Creates a **Bomb** gem.
*   **Match 5 Gems (in a line):** Creates a **Warp Gem**.
*   **Match 4 Gems (in a 2x2 square):** Creates a **Bullseye** gem. (Requires a very rare ability to unlock).

---

## 4. Special Tile Basic Effects

*   **Spear:** Clears its designated row or column.
*   **Bomb:** Detonates and clears a 3x3 area.
*   **Warp Gem:** When swapped with a normal gem, destroys all other gems of that color.
*   **Bullseye:** Prompts the player to select any single gem on the board to be destroyed.

---

## 5. The Interaction Matrix

| Action: Swap... | With a **Spear** | With a **Bomb** | With a **Warp Gem** | With a **Bullseye** |
| :--- | :--- | :--- | :--- | :--- |
| **Spear** | **Cross Spear:** Clears one full row AND one full column. | **Mega Spear:** Clears three full rows AND three full columns. | **Spear Storm:** All gems of a random color become Spears and activate. | **Targeted Spear:** Prompts the player to select a row or column to be cleared. |
| **Bomb** | **Mega Spear:** Clears three full rows AND three full columns. | **Mega Bomb:** Detonates in a large 5x5 area. | **Bombardment:** All gems of a random color become Bombs and detonate. | **Targeted Bomb:** Prompts the player to select a 3x3 area to be destroyed. |
| **Warp Gem** | **Spear Storm:** All gems of a random color become Spears and activate. | **Bombardment:** All gems of a random color become Bombs and detonate. | **Total Annihilation:** Clears the entire board. Applies **Mana Shock**. | **Targeted Warp:** Prompts the player to select a color of gem to be cleared. |
| **Bullseye** | **Targeted Spear:** Prompts to select a location; a Spear is created there and activates. | **Targeted Bomb:** Prompts to select a location; a Bomb is created there and detonates. | **Targeted Warp:** Acts as if the Warp Gem was swapped with the Bullseye's color. | **Double Bullseye:** Prompts the player to select two gems to be destroyed. |

*   **Note on Total Annihilation & Mana Shock:** Clearing the board this way yields no Coins/Treasure and applies the **Mana Shock** debuff (40% reduced mana gain for the current and next turn).

---

## 6. Attunement & Item Influence

### 6.1. Item Influence
Items are the primary way that the core puzzle rules are modified.
*   **Unlocking Creation:** The most fundamental interaction is having an item or ability that allows for the creation of a specific special tile (e.g., "The Ring of Blasting allows the creation of Bomb gems from L/T matches").
*   **Modifying Effects:** Higher-tier items can alter the effects of standard special tiles (e.g., "All Spears you create are now Lightning Javelins").

### 6.2. Attunement Influence (Static Effect Tiles)
Most Attunements exert their influence on the board by creating their own unique **Static Effect Tiles**. These are special, persistent tiles that apply effects to the board or to the enemy and are independent of the Spear/Bomb/Warp/Bullseye system. This makes their abilities a core, reliable part of any strategy.

*   **Creation:** These tiles are typically created by casting a specific spell.
*   **Mechanics:** They often have a countdown timer and apply effects over time or when matched.

**Example Static Effect Tiles:**

*   **Burning Tile (Elementalist):** A tile wreathed in flame that deals damage over time.
*   **Poison Tile (Shadow):** A tile that applies a stacking Vigor debuff to the enemy.
*   **Growth Tile (Mender):** A tile that spreads and provides healing when matched.

### 6.3. Special Interactions (Shaper & Enchanter)
As masters of creation and modification, the Shaper and Enchanter Attunements are a specific exception to the above rule. In addition to their own Static Effect Tiles, they have unique abilities that allow them to directly interact with and create the standard special tiles.

*   **Shaper (Creation):** Shapers can have abilities that directly **create** standard special tiles on the board.
    *   *Example Ability:* "Construct Bomb" - Pay a high mana cost to create a Bomb gem in a target empty space or replace a normal gem. This provides immense tactical control for setting up powerful combinations.

*   **Enchanter (Transformation):** Enchanters can have abilities that **transform** existing gems into special tiles.
    *   *Example Ability:* "Imbue Spear" - Pay a moderate mana cost to enchant a target normal gem, turning it into a Spear. This is less direct than the Shaper's creation but can be used to turn a poor board state into an advantageous one.