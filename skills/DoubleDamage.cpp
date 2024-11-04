#include "DoubleDamage.h"

void DoubleDamage::apply(GameField& field, ShipManager& shipManager, int x, int y)
{
    field.attack(x,y,shipManager);
    field.attack(x,y,shipManager);
}