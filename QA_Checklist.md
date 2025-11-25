# Quality Assurance Checklist

This checklist is to verify the functionality of all recently added features, including the dynamic ability system, UI changes, and temporary testing adjustments.

## I. Attunement Selection & UI
- [ ] **1.1 Attunement UI Layout:** Does the "Choose Your Attunement" screen display the 8 attunement buttons in a 2x4 grid below the title text?
- [ ] **1.2 Attunement Selection:** Can you successfully click on and select each of the 8 attunements?
- [ ] **1.3 Transition to Combat:** After selecting an attunement, does the game correctly transition to the first combat room?

## II. Combat & Abilities
- [ ] **2.1 Dynamic Mana Generation:**
    - [ ] When combat starts, does the board contain mana gems corresponding to your chosen attunement? (e.g., if you chose Pyromancer, are there Fire and Air gems?)
    - [ ] Does the board also contain mana gems corresponding to the monster's affinities (Earth and Fire for the Goblin)?
    - [ ] Does the board contain Skull gems?
- [ ] **2.2 Ability UI:**
    - [ ] Are the correct two starting abilities for your chosen attunement displayed in the UI?
    - [ ] Is the correct Mana Cost and Speed Cost displayed for each ability?
- [ ] **2.3 Spell Casting:**
    - [ ] Does casting a spell correctly consume the required amount of mana?
    - [ ] Does the monster's turn gauge (the yellow bar) increase after you cast a spell?
    - [ ] Does the monster's turn gauge increase after you make a gem match?
- [ ] **2.4 Specific Ability Effects:**
    - [ ] **Minor Heal (Hydromancer):** Does casting this spell restore player HP?
    - [ ] **Shadow Strike (Umbralist):** Does casting this spell deal damage to the monster?
    - [ ] **Gust of Wind (Aeromancer):** Does casting this spell rotate the top row of the board to the right?
    - [ ] **Stone Shield (Geomancer):** Does this spell grant a temporary shield/damage reduction? (Note: Effect is not yet visible, check for no crashes).
    - [ ] **Wild Growth (Biomancer):** Does this spell transform a random gem into a Life gem?

## III. Treasure Round & Data
- [ ] **3.1 Treasure Round Turns:** Does the treasure round now correctly end after 5 moves instead of 20?
- [ ] **3.2 Treasure Merging:**
    - [ ] Do 3 `Coin` gems merge into a `CoinPile`?
    - [ ] Do 3 `CoinPile` gems merge into a `CoinBag`?
    - [ ] Do 3 `CoinBag` gems merge into a `CoinBagBundle`?
    - [ ] Do 3 `CoinBagBundle` gems merge into a `TreasureChest`?

## IV. General
- [ ] **4.1 No Crashes:** Did you encounter any crashes or freezes during the test?
- [ ] **4.2 Console Output:** Were there any new or unusual error messages printed to the console?

## V. Specific Ability Functionality

### Diviner
- [ ] **Insight:** Does casting this spell highlight a random valid move on the board?
- [ ] **Mana Surge:** Does casting this spell temporarily increase mana gain/damage from all matches? (Requires observation of mana/damage values before and after cast)

### Elementalist
- [ ] **Create Burning Tile:** Does casting this spell create a new 'Burning Tile' on the board?
- [ ] **Gust of Wind:** Does casting this spell rotate a targeted row/column of gems?

### Enchanter
- [ ] **Quick Swap:** Can you swap two non-matching adjacent gems after casting this spell?
- [ ] **Insight:** Does casting this spell highlight a random valid move on the board?

### Guardian
- [ ] **Empower Self:** Does casting this spell temporarily increase the player's Vigor (defense)? (Requires observation of defense stat or damage taken)
- [ ] **Minor Heal:** Does casting this spell restore player HP?

### Mender
- [ ] **Minor Heal:** Does casting this spell restore player HP?
- [ ] **Chaotic Shift:** Does casting this spell randomly remove up to 3 tiles, causing a cascade?

### Shadow
- [ ] **Shadow Strike:** Does casting this spell deal direct damage to the monster?
- [ ] **Empower Weapon:** Does casting this spell temporarily increase the damage dealt by Skull matches? (Requires observation of Skull match damage)

### Shaper
- [ ] **Create Weapon:** Does casting this spell randomly convert up to 3 tiles into Attack tiles?
- [ ] **Sharpen Weapon:** Does casting this spell increase the damage value of a single Attack tile? (Requires observation of Attack tile damage)

### Summoner
- [ ] **Gust of Wind:** Does casting this spell rotate a targeted row/column of gems?
- [ ] **Convert Mana:** Does casting this spell convert a small amount of one mana type to another? (Requires observation of mana pools before and after cast)