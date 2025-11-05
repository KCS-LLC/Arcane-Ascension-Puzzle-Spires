
# Core Game Mechanics

This document provides a detailed breakdown of the primary game mechanics that govern gameplay in **Arcane Ascension: The Puzzle Spires**.

## 1. The Legacy Loop

The central gameplay loop is generational. The player controls a noble House, not a single character.

1.  **Heir Selection:** The player chooses an heir for the next run, each with potentially unique Traits.
2.  **The Judgement:** The new heir undergoes a trial to determine their starting Attunement, base stats, and equipment. (See `judgement.md`)
3.  **The Manor Phase:** The player prepares at the House Manor, using resources from previous runs to buy equipment, upgrade the Manor, and manage their legacy. This phase is governed by time and financial pressures.
4.  **The Spire Run:** The heir enters the procedurally generated spire to battle monsters and gather loot.
5.  **Success or Failure:** The heir either exits the spire safely (transferring loot to the House) or dies (losing all carried items). The loop then begins again with a new heir.

## 2. Heir Progression

An heir's power grows in several ways during a single run. (See `progression.md`)

*   **Attunement Ranks:** The primary progression path. By increasing their total mana capacity through use, heirs advance through gemstone ranks (Quartz, Carnelian, etc.), unlocking new abilities like the **Shroud** and tertiary mana types.
*   **Mana Pool Growth:** Mana pools for each type grow with use, but overuse can lead to temporary **Mana Burn** or permanent **Mana Scarring**.
*   **Base Stat Growth:** Vigor, Speed, and Wit are increased through rare items and quest rewards. These gains can be partially passed down to future heirs.
*   **Equipment:** Finding and equipping powerful items is the main way to boost an heir's stats and gain new abilities within a run. (See `items.md`)

## 3. Match-3 Board Mechanics

The core of combat is the puzzle board.

*   **Dynamic Gem Spawning:** The board only spawns mana gems relevant to the player's and the current monster's mana affinities.
*   **Action Gems:**
    *   **Skulls:** Deal direct physical damage.
    *   **Coins/Treasure:** Provide in-run currency.
*   **Special Matches:** Matching 4 or 5 gems in a line creates powerful special gems that can clear rows, columns, or gems of a specific color. The exact nature of these special gems can be modified by certain items and abilities.

## 4. Attunements & Magic

The magic system is defined by a character's Attunement. (See `attunement.md`)

*   **Classes:** Attunements function as the game's class system, defining which mana types a character can use and their core abilities. The base game focuses on the eight Serpent Spire Attunements.
*   **Body Locations:** Each Attunement is affixed to a body location (e.g., Hand, Heart, Mind), which can affect its function and carries unique risks, especially concerning overuse.
*   **Spells:** Spells are the primary way to spend mana. They can deal damage, apply status effects, heal, defend, or manipulate the board.
