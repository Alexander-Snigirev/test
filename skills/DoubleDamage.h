#include "Ability.h"
#ifndef DOUBLE_DAMAGE
#define DOUBLE_DAMAGE

class DoubleDamage: public Ability
{
    public:
        void apply(GameField& field, ShipManager& shipManager, int x, int y) override;
};
#endif