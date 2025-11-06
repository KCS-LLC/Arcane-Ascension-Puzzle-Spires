
#ifndef MONSTER_H
#define MONSTER_H

class Monster {
public:
    // Constructor
    Monster(int maxHp, int speed) : maxHp(maxHp), currentHp(maxHp), speed(speed), actionCounter(0) {}

    // Public Methods
    void takeDamage(int damage) {
        currentHp -= damage;
        if (currentHp < 0) {
            currentHp = 0;
        }
    }

    // This function will be called by the game loop.
    // It returns true if the monster is ready to take its turn.
    bool isTurnReady(int playerActionCost) {
        actionCounter += playerActionCost;
        if (actionCounter >= speed) {
            actionCounter -= speed; // Reset for the next turn
            return true;
        }
        return false;
    }

    // Getters
    int getCurrentHp() const { return currentHp; }
    int getMaxHp() const { return maxHp; }
    int getSpeed() const { return speed; }
    int getActionCounter() const { return actionCounter; }

private:
    int maxHp;
    int currentHp;
    int speed; // The threshold for the monster to take an action
    int actionCounter;
};

#endif // MONSTER_H
