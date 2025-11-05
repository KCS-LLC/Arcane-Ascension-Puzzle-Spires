
# Formulas & Effects

This document serves as the master rulebook for the game's underlying calculations, status effects, and ability structures. It provides the hard numbers that translate conceptual stats into tangible gameplay mechanics.

---

## 1. Core Combat Formulas

These formulas govern the primary calculations for health, damage, and action speed.

### 1.1. Health & Shroud
*   **Max HP Calculation:**
    `Max HP = 100 + (Vigor * 10)`
    *   *Example:* An heir with 15 Vigor has `100 + (15 * 10) = 250 Max HP`.

*   **Shroud Capacity Calculation:**
    `Shroud HP = (Vigor * 5) + (Wit * 5)`
    *   *Example:* An heir with 15 Vigor and 10 Wit has a Shroud with `(15 * 5) + (10 * 5) = 125 HP`.

*   **Shroud Regeneration (Out of Combat):**
    `Regen Rate = 10% of Max Shroud HP per turn`

### 1.2. Damage
*   **Skull Match Damage:**
    `Damage = (Weapon's Base Attack) + (Vigor Tier Bonus)`
    *   If no weapon is equipped, `Weapon's Base Attack` is **1**.
    *   **Vigor Tier Bonus Table:**
        | Vigor Stat | Bonus Damage |
        | :--- | :--- |
        | 0 - 24 | +0 |
        | 25 - 59 | +1 |
        | 60 - 99 | +2 |
        | 100+ | +3 |

*   **Spell Damage Calculation:**
    Spell effects are based on the mana spent, modified by a Power Tier which reflects the heir's mastery over that mana type.
    `Base Spell Damage = (Mana Cost of Spell) * (Power Tier Multiplier)`

    *   **Power Tier Table (based on Max Mana of Spell's Type):**
        | Tier Name | Max Mana Requirement | Power Multiplier |
        | :--- | :--- | :--- |
        | Quartz | 0 - 59 | 1.0x |
        | Carnelian | 60 - 199 | 1.2x |
        | Sunstone | 200 - 499 | 1.5x |
        | Citrine | 500 - 999 | 2.0x |
        | Emerald | 1000+ | 2.5x |
    *   *Example:* An heir with a max Fire Mana of **250** (Sunstone Tier) casts a spell that costs **30** Fire Mana. The Base Damage is `30 * 1.5 = 45`.

*   **Spell Critical Hits (Wit-Based):**
    *   **Crit Chance %:** `5 + (Wit / (Wit + 50))`
    *   **Crit Damage Multiplier:** `1.5 + (Wit / 100)`
    *   *Continuing the example:* The 45 base damage spell has a chance to critically hit for `45 * (1.5 + (Wit/100))` damage.

### 1.4. Gem Aura System
*   **Resource Calculation:**
    `Final Resource Gain = (Base Value) * (Aura Multiplier)`
    *   **Base Value:** The default resource gain from a gem (e.g., 1 Mana).
    *   **Aura Multiplier:** The multiplier granted by the gem's Aura (e.g., 1x for Quartz, 2x for Carnelian, etc.).

---

## 2. Status Effects Library

This is a list of all standard buffs and debuffs. Specific abilities may have unique effects not listed here.

### 2.1. Stacking Rules
Unless specified otherwise, status effects follow a consistent stacking rule with diminishing returns.

*   **Initial Application:** The effect applies with its full base intensity and duration.
*   **Second Application:** Adds 80% of the base intensity and 50% of the base duration.
*   **Third Application:** Adds 64% (80% of 80%) of the base intensity and 25% of the base duration.
*   **Subsequent Applications:** Continue this pattern, with each new stack adding 80% of the *previous* stack's intensity and 50% of the *previous* stack's duration.

**Example: "Vigor Up" (+5 Vigor, 3 Turns)**
*   **1st Cast:** +5 Vigor, 3 turns remaining.
*   **2nd Cast:** +4 Vigor (total +9), +1 turn (total 4 turns remaining).
*   **3rd Cast:** +3 Vigor (total +12), +0 turns (total 4 turns remaining).

### 2.2. Effect List

| Effect Name | Type | Effect Description | Standard Duration |
| :--- | :--- | :--- | :--- |
| **Regeneration** | Buff | Heals for 5% of Max HP at the start of the character's turn. | 3 Turns |
| **Haste** | Buff | Reduces the Final Speed Cost of all actions by 20%. | 3 Turns |
| **Vigor Up** | Buff | Temporarily increases Vigor stat by 5. | 3 Turns |
| **Wit Up** | Buff | Temporarily increases Wit stat by 5. | 3 Turns |
| **Poison** | Debuff | Deals 5% of Max HP as damage at the start of the character's turn. | 3 Turns |
| **Slow** | Debuff | Increases the Final Speed Cost of all actions by 20%. | 3 Turns |
| **Stun** | Debuff | The character cannot act. The debuff is removed when the character's turn would have started. | 1 Turn |
| **Mana Burn** | Debuff | Reduces the max capacity of a specific mana pool by 25%. | 5 Encounters |
| **Mana Scar** | Debuff | Permanently reduces the max capacity of a specific mana pool by 10% for the rest of the run. | Permanent |

---

## 3. Ability & Spell Template

This standardized template should be used for defining all new spells and abilities to ensure consistency.

*   **Name:** The ability's name.
*   **Rank Unlocked:** The Attunement Rank required to learn/use this ability (e.g., Quartz, Carnelian).
*   **Mana Cost:** The amount and type of mana required (e.g., 20 Fire Mana). This value is the spell's base power before multipliers.
*   **Speed Cost:** The base speed cost of using the ability.
*   **Target:** Who the ability affects (e.g., Self, Single Enemy, All Enemies, Board).
*   **Effect:** A clear description of the mechanical outcome.
    *   *Example:* "Deals Fire damage and applies the **Poison** debuff for 2 turns."
*   **Rank Scaling:** How the ability improves at higher Attunement Ranks.
    *   *Sunstone Bonus:* "Mana Cost increases to 30 (improving base power), and the Poison debuff now also applies **Slow**."
    *   *Emerald Bonus:* "The spell now hits a second target for 50% damage."
