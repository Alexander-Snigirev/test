#include "AbilityManager.h"


AbilityManager::AbilityManager():rng(std::random_device{}())
{
    abilities.push(createRandomAbility());
}


std::unique_ptr<Ability> AbilityManager::createRandomAbility() {
    std::uniform_int_distribution<int> dist(1, 3);
    auto skillType = static_cast<SkillType>(dist(rng));
    return CreateAbility(skillType);
}
std::unique_ptr<Ability> AbilityManager::CreateAbility(SkillType skill)
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

void AbilityManager::AddSkill()
{
    abilities.push(createRandomAbility());
}

int AbilityManager::get_count(){
    return abilities.size();
}


void AbilityManager::UseSkill(ShipManager& manager, GameField& field)
{
    if(abilities.empty()){
        throw UsingAbilityException();
    }
    auto& skill = abilities.front();
    int x=-1;
    int y=-1;
    if (auto dmg = dynamic_cast<DoubleDamage*>(skill.get())) {
        std::cout << "Enter coordinates to double-damage attack: ";
        std::cin >> x >> y;
    }
    else if (auto scn = dynamic_cast<Scanner*>(skill.get())) {
        std::cout << "Enter coordinates of top-left corner of the square: ";
        std::cin >> x >> y;
    }
    skill->apply(field,manager,x,y);
    abilities.pop();
}