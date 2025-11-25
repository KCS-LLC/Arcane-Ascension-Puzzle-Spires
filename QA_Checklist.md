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
