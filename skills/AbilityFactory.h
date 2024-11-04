#ifndef ABILITY_FACTORY_H
#define ABILITY_FACTORY_H

#include <memory>
#include "Ability.h"
#include "DoubleDamage.h"
#include "Scanner.h"
#include "Bombardment.h"

enum class SkillType{
    DoubleDmg=1,
    Scan=2,
    RandomAttack=3
};

class AbilityFactory
{
    public:
        static std::unique_ptr<Ability> CreateAbility(SkillType skill)
        {
            switch (skill) 
            {
                case SkillType::DoubleDmg:
                    return std::make_unique<DoubleDamage>();
                case SkillType::Scan:
                    return std::make_unique<Scanner>();
                case SkillType::RandomAttack:
                    return std::make_unique<Bombardment>();
                default:
                    throw std::invalid_argument("Unknown skill type");
            }
        }
};

#endif