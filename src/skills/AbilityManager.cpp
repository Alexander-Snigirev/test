#include "AbilityManager.h"


AbilityManager::AbilityManager():rng(std::random_device{}())
{
    AddSkill();
    
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

    auto& skill = abilities.back();
    if (auto dmg = dynamic_cast<DoubleDamage*>(skill.get())) {
        sym_abilities.push_back('D');
    }
    else if (auto scn = dynamic_cast<Scanner*>(skill.get())) {
        sym_abilities.push_back('S');
    }
    else if(auto scn = dynamic_cast<Bombardment*>(skill.get())){
        sym_abilities.push_back('R');
    }
     
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
    sym_abilities.pop_back();
}

void AbilityManager::save(std::ostream& out) const {
    int abilityCount = static_cast<int>(sym_abilities.size());
    out.write(reinterpret_cast<const char*>(&abilityCount), sizeof(abilityCount)); // Сохраняем количество способностей

    for (char type : sym_abilities) {
        out.write(&type, sizeof(type)); // Сохраняем тип каждой способности
    }
}

void AbilityManager::load(std::istream& in) {
    int abilityCount;
    in.read(reinterpret_cast<char*>(&abilityCount), sizeof(abilityCount));
    if (!in) {
        throw std::runtime_error("Error loading ability count.");
    }

    // Очищаем способности
    sym_abilities.clear();
    while (!abilities.empty()) {
        abilities.pop();
    }

    for (int i = 0; i < abilityCount; ++i) {
        char type;
        in.read(&type, sizeof(type));
        if (!in) {
            throw std::runtime_error("Error reading ability type.");
        }

        sym_abilities.push_back(type);
        if (type == 'D') {
            abilities.push(std::make_unique<DoubleDamage>());
        } else if (type == 'S') {
            abilities.push(std::make_unique<Scanner>());
        } else {
            abilities.push(createRandomAbility());
        }
    }
}
std::vector<char> AbilityManager::get_abilities_as_symbols(){
    return sym_abilities;
}





