#include "Game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

Game::Game()
    : playerField(10, 10), enemyField(10, 10), playerShips(),
      enemyShips({1}), playerAbilities(), isPlayerTurn(true), roundNumber(1) {}

void Game::start_new_game() {
    playerField = GameField(10, 10);
    
    // Пользователь вводит длины кораблей
    auto shipLengths = Input::get_ship_lengths();
    playerShips = ShipManager(shipLengths);
    Input::place_ships(playerField, playerShips);

    // Создание кораблей компьютера

    playerAbilities = AbilityManager();
    isPlayerTurn = true;
    roundNumber = 1;

    std::cout << "New game started!" << std::endl;
    start_round();
}

void imaging_field(GameField& field){
    std::cout<<"  0 1 2 3 4 5 6 7 8 9\n";
    for (int y = 0; y < 10; ++y)
    {
        std::cout<<y<<' ';
        for (int x = 0; x < 10; ++x)
        {
            std::cout << field.get_field_point(x, y) << ' ';
        }
        std::cout << '\n';
    }

}
void Game::initialize_round() {
    enemyField = GameField(10,10);
    enemyShips = ShipManager({1,2,3,4});

    // Случайное размещение кораблей врага
    srand(time(0));
    for (int i = 0; i < enemyShips.get_start_length(); ++i) {
        while (true) {
            int x = rand() % 10;
            int y = rand() % 10;
            Orientation orientation = (rand() % 2 == 0) ? Orientation::Horizontal : Orientation::Vertical;
            try {
                enemyField.place_ship(enemyShips.get_ship(i).get(), x, y, orientation);
                break;
            } catch (const ShipPlacementException&) {
                continue; // Пробуем снова
            }
        }
    }
    std::cout << "All ships placed. Starting the round!" << std::endl;
}

void Game::start_round() {
    initialize_round();
    std::cout << "Round " << roundNumber << " started!" << std::endl;

    while (true) {

        std::cout << "Your Field:" << '\n';
        imaging_field(playerField);
        std::cout << "Enemy Field:" << '\n';
        imaging_field(enemyField);
        std::cout<<"Your abilities: "<<playerAbilities.get_count()<<std::endl;

        if (isPlayerTurn){
            execute_player_turn();
        } else {
            execute_enemy_turn();
        }

        if (check_victory()) {
            std::cout << "You win the round!" << std::endl;
            ++roundNumber;
            break;
        }

        if (check_defeat()) {
            std::cout << "You lose! Starting new game..." << std::endl;
            start_new_game();
            return;
        }

        isPlayerTurn = !isPlayerTurn;
    }
    start_round(); // Переход к следующему раунду
}

void Game::execute_player_turn() {
    std::cout << "Your turn!" << std::endl;
    if(playerAbilities.get_count())
    {
        std::cout<<"Do you want to use ability(Y/N)?\n";
        char answer;
        std::cin>>answer;
        if(answer=='Y')
        {
            try {
                playerAbilities.UseSkill(playerShips, enemyField);
            } catch (const UsingAbilityException&) {
                std::cout << "No abilities available. Proceeding to attack." << std::endl;
            }
        }
    }    
    int ships_num_before = enemyShips.get_ships_count();
    int x, y;
    std::cout << "Enter coordinates for attack (x y): ";
    std::cin >> x >> y;
    enemyField.attack(x, y, enemyShips, DAMAGE);
    int ships_num_after = enemyShips.get_ships_count();
    if (ships_num_after < ships_num_before) {
        playerAbilities.AddSkill();
    }
}

void Game::execute_enemy_turn() {
    std::cout << "Enemy's turn!" << std::endl;

    int x, y;
    srand(time(0));
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (playerField.get_cell_status(x, y) == CellStatus::Miss);

    playerField.attack(x, y, playerShips, DAMAGE);
}

bool Game::check_victory() const {
    return enemyShips.check_all_ships();
}

bool Game::check_defeat() const {
    return playerShips.check_all_ships();
}

void Game::save_game(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error saving game!" << std::endl;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(&roundNumber), sizeof(roundNumber));
    // Сохранить состояние полей, менеджеров кораблей и способностей
    // ...
    outFile.close();
    std::cout << "Game saved successfully!" << std::endl;
}

void Game::load_game(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error loading game!" << std::endl;
        return;
    }
    inFile.read(reinterpret_cast<char*>(&roundNumber), sizeof(roundNumber));
    // Загрузить состояние полей, менеджеров кораблей и способностей
    // ...
    inFile.close();
    std::cout << "Game loaded successfully!" << std::endl;
}
