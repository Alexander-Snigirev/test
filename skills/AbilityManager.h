#include "Scanner.h"
#include "Bombardment.h"
#include "DoubleDamage.h"
#include <queue>
#include "../GameExceptions.h"
#include "AbilityFactory.h"

class AbilityManager
{
    private:
        std::queue<std::unique_ptr<Ability>> abilities;
        std::mt19937 rng;
    public:
        AbilityManager();
        void AddSkill();
        void UseSkill(ShipManager& manager, GameField& field);
        std::unique_ptr<Ability> createRandomAbility();
};