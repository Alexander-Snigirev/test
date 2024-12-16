#ifndef ABILITY
#define ABILITY
#include "../GameField.h"
#include "../ShipManager.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cstring>


class Ability {
public:
    virtual ~Ability() = default;
    virtual void apply(GameField& field, ShipManager& shipManager, int x=-1, int y=-1) = 0;
};
#endif