# Quality Assurance Checklist

This checklist is to verify the functionality of all recently added features, including the dynamic ability system, UI changes, and temporary testing adjustments.

## I. Attunement Selection & UI
- [X] **1.1 Attunement UI Layout:** Does the "Choose Your Attunement" screen display the 8 attunement buttons in a 2x4 grid below the title text?
- [X] **1.2 Attunement Selection:** Can you successfully click on and select each of the 8 attunements?
- [X] **1.3 Transition to Combat:** After selecting an attunement, does the game correctly transition to the first combat room?

## II. Combat & Abilities
- [ ] **2.1 Dynamic Mana Generation:**
    - [0] When combat starts, does the board contain mana gems corresponding to your chosen attunement? - **User Note:** The board has the correct mana types on initial fill, but does not refill from the correct types. Also the player mana bars do not reflect the correct mana pools.
    - [X] Does the board also contain mana gems corresponding to the monster's affinities (Earth and Fire for the Goblin)?
    - [X] Does the board contain Skull gems?
- [ ] **2.2 Ability UI:**
    - [X] Are the correct two starting abilities for your chosen attunement displayed in the UI?
    - [0] Is the correct Mana Cost and Speed Cost displayed for each ability? - **User Note:** the #/# is confusing. Which is mana, which is speed?
- [ ] **2.3 Spell Casting:**
    - [0] Does casting a spell correctly consume the required amount of mana? - **User Note:** unknown, mana bars do not have numbers. when you use the skill, the bar goes down, so yes?
    - [X] Does the monster's turn gauge (the yellow bar) increase after you cast a spell?
    - [X] Does the monster's turn gauge increase after you make a gem match?

## III. Treasure Round & Data
- [X] **3.1 Treasure Round Turns:** Does the treasure round now correctly end after 5 moves instead of 20?
- [X] **3.2 Treasure Merging:**
    - [X] Do 3 `Coin` gems merge into a `CoinPile`?
    - [X] Do 3 `CoinPile` gems merge into a `CoinBag`?
    - [X] Do 3 `CoinBag` gems merge into a `CoinBagBundle`?
    - [X] Do 3 `CoinBagBundle` gems merge into a `TreasureChest`?
	- [ ] Do 3 or more `TreasureChest` gems merge and add to the Treasure Score? 

## IV. General
- [X] **4.1 No Crashes:** Did you encounter any crashes or freezes during the test?
- [X] **4.2 Console Output:** Were there any new or unusual error messages printed to the console?

## V. Specific Ability Functionality

### Diviner
- [0] **Insight:** Does casting this spell highlight a random valid move on the board? - **User Note:** No visual indicator appears when the ability is activated.
- [ ] **Mana Surge:** Does casting this spell temporarily increase mana gain/damage from all matches? (Requires observation of mana/damage values before and after cast)

### Elementalist
- [0] **Create Burning Tile:** Does casting this spell create a new 'Burning Tile' on the board? - **User Note:** no tile is visually indicated that it's burning.
- [0] **Gust of Wind:** Does casting this spell rotate a targeted row/column of gems? - **User Note:** only the top row rotates. This should take an input of 2 adjacent tiles. The first is the target, the second is the direction. Also this needs to be animated.

### Enchanter
- [X] **Quick Swap:** Can you swap two non-matching adjacent gems after casting this spell?
- [0] **Insight:** Does casting this spell highlight a random valid move on the board? - **User Note:** No visual indicator appears when the ability is activated.

### Guardian
- [0] **Empower Self:** Does casting this spell temporarily increase the player's Vigor (defense)? (Requires observation of defense stat or damage taken) - need visual stat indicator to test
- [0] **Minor Heal:** Does casting this spell restore player HP? - clicked and no HP bar change. Could have happened at same time as monster attack, so they offset? Maybe animate the healing so its more obvious?

### Mender
- [X] **Minor Heal:** Does casting this spell restore player HP? - 
- [X] **Chaotic Shift:** Does casting this spell randomly remove up to 3 tiles, causing a cascade?

### Shadow
- [X] **Shadow Strike:** Does casting this spell deal direct damage to the monster?
- [0] **Empower Weapon:** Does casting this spell temporarily increase the damage dealt by Skull matches? (Requires observation of Skull match damage) - add numbers to monster HP for diagnostics. - need visual indicator when this is active/expires. 

### Shaper
- [X] **Create Weapon:** Does casting this spell randomly convert up to 3 tiles into Attack tiles?
- [ ] **Sharpen Weapon:** Does casting this spell increase the damage value of a single Attack tile? (Requires observation of Attack tile damage)

### Summoner
- [0] **Gust of Wind:** Does casting this spell rotate a targeted row/column of gems? - **User Note:** only the top row rotates. This should take an input of 2 adjacent tiles. The first is the target, the second is the direction. Also this needs to be animated.
- [0] **Convert Mana:** Does casting this spell convert a small amount of one mana type to another? (Requires observation of mana pools before and after cast) - this is the wrong ability. it shoudl be the same as the enchanter transfernce ability.
