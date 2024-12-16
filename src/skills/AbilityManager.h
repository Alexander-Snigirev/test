#ifndef ABILITY_MANAGER_H
#define ABILITY_MANAGER_H
#include "Scanner.h"
#include "Bombardment.h"
#include "DoubleDamage.h"
#include <queue>
#include "../GameExceptions.h"
//#include "AbilityFactory.h"
enum class SkillType{
    DoubleDmg=1,
    Scan=2,
    RandomAttack=3
};
class AbilityManager
{
    private:
        std::queue<std::unique_ptr<Ability>> abilities;
        std::vector<char> sym_abilities;
        std::mt19937 rng;
        std::unique_ptr<Ability> CreateAbility(SkillType skill);
        std::unique_ptr<Ability> createRandomAbility();
        
    public:
        AbilityManager();
        void AddSkill();
        void UseSkill(ShipManager& manager, GameField& field);
        int get_count();
        std::vector<char> get_abilities_as_symbols();
        void save(std::ostream& out) const;
        void load(std::istream& in);
};
#endif