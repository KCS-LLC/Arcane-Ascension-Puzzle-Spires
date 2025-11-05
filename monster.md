
# Monster Stats & Mechanics

This document outlines the core stats and mechanics that define the various monsters and opponents found within the spire.

---

## 1. Core Monster Stats

Every monster in the game is defined by a set of core statistics that determine its behavior and challenge level in combat.

*   **Rank:** A monster's power level, using the gemstone system (Quartz, Carnelian, Sunstone, Citrine, Emerald). This provides a clear, at-a-glance indication of the monster's threat level. A monster's Rank also influences the **Gem Aura System**; higher-rank monsters have a greater chance of causing gems with powerful Auras to appear on the board.
*   **Health (HP):** The amount of damage a monster can sustain. When this reaches zero, the monster is defeated.
*   **Speed:** This stat determines the monster's turn frequency. When the player's cumulative action speed costs equal or exceed the monster's Speed, the monster takes its turn. Higher speed means the monster acts more often.
*   **Vigor:** The monster's physical resilience. This stat influences its base HP, resistance to physical damage (from Skulls), and susceptibility to negative status effects.
*   **Wit:** The monster's cunning and magical aptitude. This stat influences the complexity of its abilities and its spell power. It also determines if a monster meets the **Wit Threshold** to interact directly with the puzzle board.
*   **Mana Affinities:** A list of the mana types the monster can utilize. This is a critical mechanic, as the monster's affinities, combined with the player's, determine which mana gems will appear on the match-3 board during the encounter.
*   **Resistances & Vulnerabilities:** Monsters can have innate strengths and weaknesses. For example, a Fire Elemental might be resistant to Fire Mana damage but vulnerable to Water Mana damage.

---

## 2. Monster Abilities & Actions

On its turn, a monster will perform an action. The available actions depend on the monster's Wit.

*   **Basic Actions (All Monsters):**
    *   **Basic Attack:** A standard physical or magical attack.
    *   **Cast Spell:** Use an ability from its spell list.
    *   **Apply Status Effect:** Use a buff on itself or a debuff on the player.

*   **Advanced Actions (Monsters at or above Wit Threshold):**
    *   **Make a Match:** The monster can choose to make a match on the puzzle board instead of performing a basic action. This allows it to gather mana, deny the player valuable gems, or even trigger cascades. This represents a higher level of intelligence and tactical awareness.
    *   **Board Manipulation:** Abilities that directly affect the match-3 board (e.g., Locking Gems, Spawning Hazards).

---

## 3. Monster Categories

Monsters are classified into different categories based on their role and threat level.

### 3.1. Common Monsters

**Example Monster Template:**

*   **Name:** Spire Goblin
*   **Category:** Common Monster
*   **Rank:** Quartz
*   **Base Stats:**
    *   **HP:** Low
    *   **Speed:** Moderate
    *   **Vigor:** Low
    *   **Wit:** Low (Below Threshold)
*   **Abilities:**
    *   **Shiv:** Deals minor physical damage.
    *   **Rock Toss:** A low-cost spell that deals minor Earth damage.

### 3.2. Spire Guardians

**Example Guardian Template:**

*   **Name:** Sunstone Sentinel
*   **Category:** Spire Guardian
*   **Rank:** Sunstone
*   **Base Stats:**
    *   **HP:** Very High
    *   **Speed:** Moderate
    *   **Vigor:** High
    *   **Wit:** High (Above Threshold)
*   **Abilities:**
    *   **Reflective Shroud:** Reflects the first spell of each turn back at the caster for 50% damage.
    *   **Make a Match:** Can choose to make a match on the board to gather Light or Enhancement mana.
    *   **Purifying Beam:** A powerful spell that deals high Light damage.
