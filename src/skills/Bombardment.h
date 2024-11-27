#include "Ability.h"
#ifndef BOMBARDMENT
#define BOMBARDMENT

class Bombardment: public Ability
{
    public:
        void apply(GameField& field, ShipManager& shipManager, int x, int y) override;
};
#endif