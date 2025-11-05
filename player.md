
# Player Stats & Mechanics

This document outlines the core stats and mechanics that define a player's progression. The system is divided into two distinct parts: **The Heir**, representing the current character on a single run, and **The House**, representing the permanent, generational progression.

---

## 1. The Heir (In-Run Progression)

The Heir's stats and inventory are temporary and are **lost upon death**. Successfully exiting the spire allows some of these assets (currency, items) to be transferred to the House.

### Core Stats
*   **Health (HP):** The amount of damage an heir can sustain. Reaches zero, the heir dies. Determined by the Judgement, traits, and in-run items.
*   **Mana Capacity:** Each mana type (Fire, Water, etc.) has its own pool. The maximum amount of each mana an heir can hold is determined by their Attunement, items, and temporary upgrades. Each mana pool will be assigned to a body location. As they progress, more types will be unlocked. 
*   **Vigor:** Base Stat for how physically healthy/strong the character is. Affects healing, max HP, resistances, etc
*   **Speed:** Base stat for how quick the player is, mentally and physically. In match-3 combat, each action (matching gems, casting spells) has a 'speed cost'. When the total speed cost of player actions equals or exceeds the opponent's speed stat, the opponent takes their turn. A higher Speed stat for the heir reduces the speed cost of their actions, potentially allowing them to perform multiple actions before the enemy.
*   **Wit:** Base stat for how cunning/smart the player is. Will be used to provide help during puzzles, and for learning skills/spells, enchanting, dialog options, and training/teaching.

### Combat & Magic
*   **Shroud:** A personal magical barrier that acts as a regenerating shield over the heir's HP. The Shroud is automatically unlocked when the heir's primary Attunement reaches the **Carnelian** rank through in-run progression. The Shroud has its own HP pool and will absorb damage before the heir's main HP. It regenerates slowly over time between combat encounters. Players can spend mana to imbue their Shroud with a specific mana type, granting it special defensive properties (e.g., a Fire Shroud may damage attackers, a Water Shroud may have faster regeneration).
*   **Attunements:** The heir's "class," granted during the Judgement. Determines which mana types they can use and their initial spell list. The initial game will only permit one Attunement per character. When granted, the attunement will affix to one of the following body locations:
    *   **Left Hand:** Ideal for attunements that enhance weapon usage or require accurately directing spells.
    *   **Right Hand:** Ideal for attunements that enhance weapon usage or require accurately directing spells.
    *   **Left Leg:** Suited for attunements focused on movement, speed, or delivering spells through touch.
    *   **Right Leg:** Suited for attunements focused on movement, speed, or delivering spells through touch.
    *   **Lungs:** Related to area-of-effect abilities, stamina, or spells cast via incantations.
    *   **Heart:** Channels powerful magic through the entire body. Provides significant bonuses but overuse can be extremely dangerous, causing self-damage or even death.
    *   **Mind:** The primary location for mental, perception, and illusionary magic. Overuse carries a high risk of mana scarring or death.
    *   *Note on Limb Loss: If an heir loses a limb, any attunement affixed to that limb becomes unusable, and its associated mana pool is inaccessible.*
The chosen location impacts the function and potential risks of the attunement. Damage to an area with an attunement could temporarily weaken its mana regeneration.
*   **Spells:** A list of active abilities the heir can cast using mana. New spells can be learned or existing ones upgraded during a run by finding grimoires or other treasures. Additionally, the player can learn abilities from training outside of the Spire between runs. 
*   **Traits:** Unique positive and/or negative modifiers assigned to an heir at creation (e.g., "Mana Affinity: Fire" - +10% Fire Mana from matches, "Brittle Bones" - +15% damage from Skull matches).

### Loot & Equipment
*   **Equipment Slots:**
    *   **Weapon:** (e.g., Wand, Sword) - Often enhances Skull damage or provides a unique combat spell.
	*   **Off-Hand:** (e.g., Shield, second weapon) - Cannot be used if weapon is 2-handed or oversized
    *   **Head:** (e.g., Helmet, Circlet)
    *   **Body:** (e.g., Armor, Robes)
	*   **Legs:** (e.g., Pants, Leggings, Skirts)
	*   **Feet:** (e.g., Boots, Shoes, Slippers)
	*   **Hands:** (e.g., Gloves, Gauntlets)
    *   **Ring 1:** (Right Hand ring) 
	*   **Ring 2:** (Left Hand ring)
*   **Carried Coins:** The primary in-run currency, gathered from matching Coin/Treasure gems and defeating enemies. Lost on death. Can be used at merchants found outside the spire. Occasionaly used for puzzles and random encounters in the Spire. 
*   **Inventory:** A temporary bag for holding single-use items (e.g., potions, escape scrolls), Attunement Shards, and other treasures found during the run.

---

## 2. The House (Meta-Progression)

The House represents the player's permanent progress. These stats and unlocks persist through generations, making future runs more manageable.

### The Manor & Treasury
*   **House Treasury:** The permanent wealth of the house, measured in **Coins**. This is funded by heirs successfully exiting the spire with their carried currency. Used to pay for financial obligations and Manor upgrades.
*   **Manor Upgrades:** The primary meta-progression system. The player can spend from the House Treasury to:
    *   **Unlock/Upgrade Merchants:** Improve the quality and variety of items available for purchase between runs.
    *   **Build Classrooms:** Unlock new Attunements that can be granted during the Judgement for future heirs.
    *   **Expand the Library:** Unlock new spells that can be found or learned in future runs.
    *   **Reinforce the Vault:** Increase the storage capacity for shared items.

### Legacy & Heirs
*   **The Vault:** A shared, permanent storage space. Players can deposit powerful items here after a successful run, allowing future heirs to equip them before starting their own journey.
*   **Generational Knowledge:** If a member of the family becomes powerful or successful enough, they will gain access to generational knowledge that will immediately be passed upon all future heirs. This knowledge can be acquired through various means, including successful random encounters, reaching significant milestones within the spire (e.g., defeating a powerful Spire Guardian for the first time), or discovering specific ancient tomes and unique treasures. This knowledge will include things that increase starting mana pools, faster mana growth, better starting spells and abilities, and other permanent benefits. Later in the game, House progression will allow players to fund research initiatives within the Manor to actively accelerate the growth and acquisition of new Generational Knowledge.
*   **Heir Pool:** The number of available heirs for the next generation. Starts as a fixed number and can be increased through in-game choices and events, securing the family's future.
*   **Global Unlocks:** A record of all Attunements, spells, and items that have been discovered. Once unlocked via Manor Upgrades or found in the spire, they are permanently added to the pool of things that can appear in subsequent runs.
*   **House Legacy:** A reputation or ranking. Making certain choices, defeating powerful guardians, and accumulating wealth can improve the House's standing, leading to unique events, special heir traits, and better opportunities.
