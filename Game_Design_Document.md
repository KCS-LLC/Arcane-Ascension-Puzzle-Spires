# Game Design Document: Arcane Ascension - The Puzzle Spires

## 1. Overview

**Title:** Arcane Ascension: The Puzzle Spires
**Genre:** Roguelike-legacy Match-3 RPG
**Platform:** PC, Mobile
**Target Audience:** Fans of puzzle RPGs, roguelikes (especially Rogue Legacy), and the Arcane Ascension series.
**Core Concept:** A single-player game where players control a lineage of adventurers from a noble house. Each generation, an heir must enter a procedurally generated spire to earn wealth and power, battling monsters through strategic match-3 combat. The gameplay revolves around matching colored mana gems to power spells, mirroring the magic system of the books, while managing the long-term progression of the player's house.

## 2. The House and Legacy System

The player does not control a single character, but rather a noble house with a long line of adventurers.

*   **The Heirs:** At the start of each run, the player chooses from a selection of potential heirs. Each heir may have unique traits (e.g., "Strong Constitution" - +10% HP, "Mana Affinity" - starts with extra mana, "Clumsy" - occasionally drops gold).
*   **The Judgement:** Before a run, the chosen heir undergoes a "Judgement," a short series of puzzle challenges. Success in these challenges determines the heir's starting Attunement, base stats, and a unique starting treasure, making each new character distinct.
*   **The Hub World - The Manor:** Between runs, the player returns to their family manor. Here they can:
    *   **Manage the Vault:** Store powerful equipment and treasures for future heirs.
    *   **Visit Merchants:** Buy and sell equipment.
    *   **Attend Classes:** Spend time and resources to permanently upgrade certain stats or unlock new spells for the entire lineage.
    *   **Train with Family:** Speak with surviving (retired) house members for temporary buffs or to learn new skills.

## 3. Core Gameplay Loop

The player's goal is to manage their house's legacy by sending heirs into the spire to gain resources.

1.  **Heir Selection:** Choose an heir for the next generation.
2.  **The Judgement:** Complete the Judgement to determine the heir's starting loadout. The judgment will be a modified match 3 puzzle initially to set mana types and attunement. This will be followed by a modified first floor spire run. This will set initial treasure and items.
3.  **Manor Phase:** Prepare for the run. Buy/sell equipment, manage the vault, and attend training. This phase can take as long as the player wants, but each action will take in-game time, that will deplete the house, treasury, and resources.
4.  **Enter Spire:** The player starts a new "run" by entering the spire.
5.  **Puzzle Combat & Encounters:** The core match-3 gameplay remains the same. The player moves through rooms, battles monsters, and gathers rewards.
6.  **Exiting the Spire:** At regular intervals (e.g., after defeating a boss), the player has the option to safely exit the spire with all the loot they have gathered. This resets the spire for the next run. Later in the progression, the player will be able to acquire items that will allow for early exit from the tower. They will be single-use and expensive.
7.  **Defeat & Legacy:** If an heir dies in the spire, all wealth and items they were carrying are lost forever. The player then returns to the Manor to select the next heir, continuing the cycle, if the house has heirs and can afford the process. The game will be a loss if the house suffers financial ruin, or runs out of heirs.
## 4. Time and Urgency

*   **Financial Obligations:** The house has recurring financial obligations (taxes, upkeep, etc.) that must be paid. A timer is always present during the Manor phase.
*   **Urgency:** This system creates a sense of urgency, forcing the player to balance preparation time with the need to enter the spire and earn money. Failure to meet obligations could result in penalties for the next run (e.g., starting with less gold, a negative trait on the next heir).
*   **Legacy:**  The family will begin with a fixed size and set number of heirs. As time progresses, some will be allowed to marry and produce more heirs. This will be decided based on choices and actions done inside and outside of the spire.
## 5. Game Mechanics (Combat)

### 5.1. The Match-3 Board

The board is the core of combat. It is filled with different types of gems. A key mechanic is that **only the mana types usable by the player and the current opponent(s) will populate the board during an encounter.**

*   **Mana Gems:**
    *   **Grey:** Raw Mana
    *   **Air Mana**
    *   **Fire Mana**
    *   **Earth Mana**
    *   **Water Mana**
    *   **Umbral/Shadow Mana**
    *   **Light Mana**
    *   **Life Mana**
    *   **Death Mana**
    *   **Mental Mana**
    *   **Perception Mana**
    *   **Transference Mana**
    *   **Enhancement Mana**
*   **Action Gems:**
    *   **Skulls:** Deal direct physical damage to the opponent when matched.
    *   **Coins/Treasure:** Provide currency (Marks) when matched. A deeper system will allow for matching and upgrading these symbols to create higher-tier treasure that yields greater rewards.
	*   **Special Ability Tiles:** Some abilities and equipment can provide additional special tiles generated on the field. These will have different effects based on current character, equipment, and abilities. Initial examples are:
		- Equipment that enables the generation of special tiles when matches of four or five of the same tile in a row and T-shaped or X-shaped matches of same tiles.

*Note on Shields: Defensive abilities like shields are no longer tied to a specific gem. Instead, they are cast as spells or granted by items, fueled by the player's collected mana pool.*

Special tiles (like Spears or Bombs) are not created by default. An heir must have an item equipped or a passive ability active that specifically enables the creation of one or more types of special tiles. This makes abilities that unlock special tiles highly valuable.

### 5.2. Mana & Spells

*   Matching mana gems adds that color of mana to the player's mana pool.
*   The player has a list of spells determined by their Attunement(s).
*   Each spell has a mana cost (e.g., **Fireball** costs 10 Fire Mana).
*   Spells can deal damage, apply status effects (positive or negative), heal, defend, or manipulate the board.
*   **Mana Dynamics:** The "Like deflects like, opposites negate" principle is a key mechanic.
    *   **Deflection:** If an enemy casts a spell that puts special "spell gems" on the board, matching them with the same color can deflect the effect back at them.
    *   **Negation:** Matching opposite mana types (Fire/Water, Air/Earth, Light/Umbral) near each other can cause a "mana burn" explosion, clearing a small area of the board and dealing neutral damage to both combatants.

### 5.3. Attunements

The player's "class" is their Attunement.

*   **Starting Attunements:** The Judgement determines the starting Attunement. Initially available Attunements include:
    *   **Elementalist:** Access to Fire, Water, Air, Earth mana. Focus on direct damage spells.
    *   **Guardian:** Access to Earth and Enhancement mana. Focus on defense and self-buffs.
    *   **Shaper:** Access to Metal and Wood mana. Focus on creating constructs (special gems that provide passive effects).
    *   **Diviner:** Access to Perception and Mental mana. Focus on board manipulation and debuffing the enemy.
*   **Gaining Attunements:** During a run, players can find "Attunement Shards" after defeating powerful enemies, allowing them to gain a new, restricted Attunement and access to its mana types and spells. This allows for powerful build customization.

## 6. Roguelike & Legacy Elements

*   **Procedural Generation:** Each spire run is unique.
*   **Permadeath of the Heir:** When an heir dies, their carried items and wealth are lost. The run is over.
*   **Meta-Progression (The House):** The true progression is in the house itself. Currency spent on manor upgrades, new items unlocked for merchants, and stored items in the vault persist between generations. This is the primary way the player grows stronger over time.

## 7. Player Progression

*   **In-Run (The Heir):** Improving abilities, finding items, and gaining temporary power within a single run based on performance, mana gained, and items/treasure looted. A key progression milestone is reaching the **Carnelian** rank with an Attunement, which unlocks the **Shroud**, a personal defensive shield.
*   **Meta-Game (The House):** Unlocking new Attunements for future heirs, upgrading the manor, adding new items to the merchant pools, and building a powerful collection of items in the vault.

## 8. Enemies

Enemies are drawn from the lore of Arcane Ascension, each with its own health, abilities, and mana affinities. The spire contains a wide variety of threats.

*   **Common Monsters:** These form the bulk of the encounters in the spire. Examples include:
    *   Golems
    *   Elementals
    *   Slimes
    *   Goblins
    *   Kobolds
    *   Imps
    *   Gargoyles & Urgoyles
    *   Karvensi
    *   Delsys
    *   Ogres
    *   Serpents
*   **Random Encounters:** Players will face a variety of special encounters. This could include enchanted traps, merchants with rare goods, powerful wandering monsters, or even brief narrative events that offer risk and reward.
*   **Spire Guardians:** At the end of each major spire floor, the player faces a Spire Guardian. These are powerful, unique foes, often upgraded versions of normal monsters with special abilities or significantly higher power levels. The most intelligent of these guardians can even interact directly with the puzzle board, making matches to gather mana or deny resources. Examples could include a Hydra, a Dragon, or a powerful Tavare. These guardians serve as the primary skill checks for the player's progress. Passing them results in a large reward and access to the next floor. Some floors will also offer the option to exit the spire after the Guardian is defeated. (probably multiples of 5, initially)

## 9. Art & Sound

*   **Art Style:** A clean, anime-inspired art style that matches the book covers. Gem designs should be clear and distinct. Spell effects should be flashy and satisfying.

*   **Sound:** Magical and orchestral soundtrack. Satisfying sound effects for gem matches, spell casts, and impacts.



## 10. Heir and Family Management System



This system introduces a long-term strategic and economic layer to the game, making the management of the house as important as the adventures in the spire.



*   **Living Family Members:** The house is not just an abstract entity but is composed of living family members. The player will track all relatives, each of whom contributes to or drains the family's resources.

*   **Careers and Income:** Each non-adventuring family member will have a job or career (e.g., merchant, politician, scholar). These careers will generate a steady stream of income for the house, which is crucial for paying upkeep and funding spire runs.

*   **Upkeep Costs:** Every family member, including the active heir, will have an upkeep cost. This creates a constant financial pressure that must be managed through income from careers and loot from the spire.

*   **Heir Retirement:** When an heir successfully completes their adventuring career (either by choice after a number of successful runs or by reaching a certain age), they retire.

*   **Post-Adventure Careers:** Upon retirement, the player can choose a new career for the former heir. The available careers and their effectiveness will be determined by the skills and attunements the heir gained while adventuring.

    *   *Example:* An heir who mastered the Elementalist and Diviner attunements might become a powerful "Magical Forecaster," generating significant income. An heir who focused on defense and physical combat might become a "City Guard Captain," providing a smaller income but reducing the house's overall upkeep costs.

*   **Strategic Depth:** This system encourages players to think about the long-term consequences of their heir's development, balancing short-term gains in the spire with the long-term economic prosperity of their family.

## 11. Game Calendar and Time Progression

To create a unified strategic layer and a tangible sense of urgency, the game will feature a global calendar. This system tracks the passage of time across all phases of gameplay, making time itself a critical resource to be managed.

*   **Unified Time:** The calendar advances based on player actions, whether in the Manor or the Spire. This replaces abstract timers with a concrete timeline.
    *   **Manor Actions:** Every significant action in the Manor (e.g., training, crafting, attending class) will cost a specific number of days.
    *   **Spire Expeditions:** A run into the Spire is not instantaneous. It costs time to enter, and additional days will pass for every major floor cleared or Guardian defeated. A deep, successful run might take weeks of in-game time.

*   **The Calendar Structure:** Each in-game year will consist of a procedurally generated calendar, unique to each new legacy. A suggested structure is: 10 months per year, 4 weeks per month, and 10 days per week.

*   **Scheduled and Procedural Events:** The calendar will be populated with various events:
    *   **Financial Obligations:** Tax deadlines and upkeep payments are now fixed dates on the calendar, allowing the player to plan for them.
    *   **Holidays:** Each calendar will have procedurally generated holidays. These events can provide temporary global effects (e.g., bonuses to certain mana types, increased luck, unique merchant stock).
    *   **Scheduled Story Events:** Major world events, such as tournaments, political summits, or celestial alignments, can be scheduled on the calendar, giving the player long-term goals to prepare for.

*   **Strategic Impact:** This system forces meaningful strategic choices. A player must constantly weigh the time cost of their actions against upcoming deadlines and opportunities. Spending a month training an heir might make them powerful, but it could also mean missing a lucrative holiday bonus or failing to gather enough resources before taxes are due. This directly ties the micro-level puzzle gameplay to the macro-level generational strategy.
