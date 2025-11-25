# Quality Assurance Checklist (2025-11-26)

This checklist is for the next round of testing. Please verify the fixes for recently resolved bugs and test the functionality of features that are still pending.

## I. Re-Verification of Core Mechanic Fixes
- [ ] **1.1 Mana Refill:** When matches are made in combat, do the new gems that fall match the player's and monster's mana affinities?
- [ ] **1.2 Mana Bar Display:** Upon starting combat, do the player's mana bars correctly display the mana types for the chosen attunement, including numerical values (e.g., "0/100")?
- [ ] **1.3 Spell Cost Clarity:** Do the ability buttons in the UI clearly label the Mana and Speed costs (e.g., "M:15 S:20")?
- [ ] **1.4 Chaotic Shift:** Does the Mender's `Chaotic Shift` ability now correctly remove 3 gems and cause the board to cascade and refill the empty spaces?

## II. High Priority Bugs to Test
- [ ] **2.1 Summoner's Second Ability:** When selecting the Summoner attunement, is the second ability `Quick Swap`?
- [ ] **2.2 Treasure Chest Merging:** (To be tested when move count is restored) Do 3 or more `TreasureChest` gems merge correctly and add to the score?

## III. Medium Priority Bugs & Untested Abilities
- [X] **3.1 Insight Visuals (Diviner/Enchanter):** Does the `Insight` ability now provide a clear visual indicator on the two gems that form a valid move?
- [X] **3.2 Create Burning Tile Visuals (Elementalist):** Does the `Create Burning Tile` ability now provide a clear visual effect on the targeted tile to indicate that it is "burning"?
- [ ] **3.3 Minor Heal Feedback (Guardian):** Does the Guardian's `Minor Heal` ability now have a clear visual effect (e.g., floating green numbers, animation) to confirm that healing occurred?
- [ ] **3.4 Buff Indicators (Guardian/Shadow):**
    - [ ] Is there a UI element that appears when `Empower Self` or `Empower Weapon` is active?
    - [ ] Does this indicator show the remaining duration or expire correctly?
- [X] **3.5 Monster HP Display:** Does the monster's HP bar now include a numerical display (e.g., "125/150")?
- [ ] **3.6 Gust of Wind Functionality (Elementalist/Summoner):**
    - [ ] Does the `Gust of Wind` ability now require player input to select a row/column and direction?
    - [ ] Does the ability correctly rotate the selected row/column?
    - [ ] Is the rotation now animated smoothly?
- [ ] **3.7 Untested Abilities:**
    - [ ] **Mana Surge (Diviner):** Does this ability increase mana/damage from matches?
    - [ ] **Sharpen Weapon (Shaper):** Does this ability increase the damage of an Attack tile?

## IV. General
- [ ] **4.1 No Crashes:** Did you encounter any new crashes or freezes during the test?
- [ ] **4.2 Console Output:** Were there any new or unusual error messages printed to the console?