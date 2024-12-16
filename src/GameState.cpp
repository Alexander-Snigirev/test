#include "GameState.h"
/*void GameState::save(int round, GameField& playerField, GameField& enemyField,
              ShipManager& playerShips, ShipManager& enemyShips,AbilityManager& playerAbilities, std::ostream& out) const{
    out.write(reinterpret_cast<const char*>(&roundNumber), sizeof(roundNumber));
    playerShips.save(out);
    playerField.save(out, pShips);
    enemyShips.save(out);
    enemyField.save(out, eShips);
    playerAbilities.save(out);

}*/
void GameState::load(int round, GameField& playerField, GameField& enemyField,
              ShipManager& playerShips, ShipManager& enemyShips,AbilityManager& playerAbilities, std::istream& in){
    in.read(reinterpret_cast<char*>(&roundNumber), sizeof(roundNumber));
    std::cout << "Round Number loaded: " << roundNumber << std::endl;

    playerShips.load(in);
    playerField.load(in, playerShips);
    enemyShips.load(in);
    enemyField.load(in, enemyShips);
    playerAbilities.load(in);
}


void GameState::save(std::ostream& out, const ShipManager& playerShips, const ShipManager& enemyShips){
    out.write(reinterpret_cast<const char*>(&roundNumber), sizeof(roundNumber));
    playerShips.save(out);
    pField.save(out, playerShips);
    enemyShips.save(out);
    eField.save(out, enemyShips);
    int abilityCount = static_cast<int>(pAbilities.size());
    out.write(reinterpret_cast<const char*>(&abilityCount), sizeof(abilityCount)); // Сохраняем количество способностей

    for (char type : pAbilities) {
        out.write(&type, sizeof(type)); // Сохраняем тип каждой способности
    }
}


GameState::GameState(const int round, const GameField& playerField, const GameField& enemyField, std::vector<char> playerAbilities){
    roundNumber = round;
    pField = playerField;
    eField = enemyField;
    pAbilities = playerAbilities;
}


std::ostream& operator<<(std::ostream& out, const GameState& state){
    out.write(reinterpret_cast<const char*>(&state.roundNumber), sizeof(state.roundNumber));
    state.pShips.save(out);
    state.pField.save(out, state.pShips);
    state.eShips.save(out);
    state.eField.save(out, state.eShips);
    std::cout<<"OK"<<std::endl;
    
    std::vector<char> sym_abilities = state.pAbilities; 
    int abilityCount = static_cast<int>(sym_abilities.size());
    out.write(reinterpret_cast<const char*>(&abilityCount), sizeof(abilityCount)); // Сохраняем количество способностей

    for (char type : sym_abilities) {
        out.write(&type, sizeof(type)); // Сохраняем тип каждой способности
    }
}

std::istream& operator>>(std::istream& in, GameState& state){
    in.read(reinterpret_cast<char*>(&state.roundNumber), sizeof(state.roundNumber));
    std::cout << "Round Number loaded: " << state.roundNumber << std::endl;

    state.pShips.load(in);
    state.pField.load(in, state.pShips);
    state.eShips.load(in);
    state.eField.load(in, state.eShips);
    //state.pAbilities.load(in);
}



