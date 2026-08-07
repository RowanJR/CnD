#ifndef DICE_H_
#define DICE_H_

void InitializeDice();

//returns an int representing a die
int Roll(int sides);

int RollDisadvantage(int sides);

int RollAdvantage(int sides);

#endif // DICE_H_