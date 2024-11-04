#include "Ability.h"
#ifndef SCANNER
#define SCANNER

class Scanner: public Ability
{
    public:
        void apply(GameField& field, ShipManager& shipManager, int x, int y) override;
};

#endif