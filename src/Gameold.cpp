/*#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

Game::Game()
    : playerField(10, 10), enemyField(10, 10), playerShips(),
      enemyShips({1}), playerAbilities(), isPlayerTurn(true), roundNumber(1) {}

void Game::start_new_game() {
    char ans;
    std::cout<<"Do you want to load save?(y/n)"<<std::endl;
    std::cin>>ans;
    if(ans=='y'){
        loading();
        start_round();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        return;
    }
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
    initialize_round();
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
    enemyShips = ShipManager({1,1});

    // Случайное размещение кораблей врага
    srand(time(0));
    for (int i = 0; i < enemyShips.get_start_length(); ++i) {
        Orientation orientation;
        while (true) {
            int x = rand() % 10;
            int y = rand() % 10;
            orientation = (rand() % 2 == 0) ? Orientation::Horizontal : Orientation::Vertical;
            try {
                enemyField.place_ship(enemyShips.get_ship(i).get(), x, y, orientation);
                break;
            } catch (const ShipPlacementException&) {
                continue; // Пробуем снова
            }
        }
        enemyShips.set_orientation(i, orientation);
    }
    std::cout << "All ships placed. Starting the round!" << std::endl;
}



void Game::start_round() {
    std::cout << "Round " << roundNumber << " started!" << std::endl;
    bool defeat = false;
    InputHandler inputHandler("commands.txt");
    FieldRenderer fieldRenderer;

    // Создаем контроллер и рендерер
    GameController<Game, InputHandler> controller(*this, inputHandler);
    GameRenderer<Game, FieldRenderer> renderer(fieldRenderer);

    while (true) {
        std::cout << "Your Field:" << '\n';
        imaging_field(playerField);
        std::cout << "Enemy Field:" << '\n';
        imaging_field(enemyField);
        std::cout<<"Your abilities: "<<playerAbilities.get_count()<<std::endl;

        if (isPlayerTurn){
            std::cout << "Your turn!" << std::endl;
            UseCommand();
            UseAbility();
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
            defeat = true;
            break;
        }

        isPlayerTurn = !isPlayerTurn;
    }
    if(defeat){
        start_new_game();
        return;
    }
    initialize_round();
    start_round(); // Переход к следующему раунду
}

void Game::loading(){
    std::string file_name;
    std::cout<<"Enter the filename\n";
    std::cin>>file_name;
    try{
        load_game(file_name);

        std::cout << "Your Field:" << '\n';
        imaging_field(playerField);
        std::cout << "Enemy Field:" << '\n';
        imaging_field(enemyField);
        std::cout<<"Your abilities: "<<playerAbilities.get_count()<<std::endl;

    } catch(std::runtime_error& e){
        std::cout<<e.what()<<std::endl;
        loading();
    }
}

void Game::UseCommand(){
    std::cout<<"Enter command"<<std::endl;
    std::string command, file_name;
    std::cin>>command;
    if(command=="save"){
        std::cout<<"Enter the filename\n";
        std::cin>>file_name;
        save_game(file_name);
    }
    else if(command=="load"){
        loading();
    }
}

void Game::UseAbility(){
    if(playerAbilities.get_count())
    {
        std::cout<<"Do you want to use ability(Y/N)?\n";
        char answer;
        std::cin>>answer;
        if(answer=='Y')
        {
            int ships_num_before = enemyShips.get_ships_count();
            try {
                playerAbilities.UseSkill(playerShips, enemyField);
            } catch (const UsingAbilityException&) {
                std::cout << "No abilities available. Proceeding to attack." << std::endl;
            } catch(const InvalidCoordinatesException& e){
                std::cout<<e.what()<<std::endl;
                UseAbility();
            }
            int ships_num_after = enemyShips.get_ships_count();
            if (ships_num_after < ships_num_before) {
                playerAbilities.AddSkill();
            }
        }
    } 
}

void Game::execute_player_turn() {
       
    int ships_num_before = enemyShips.get_ships_count();
    int x, y;
    std::cout << "Enter coordinates for attack (x y): ";
    std::cin >> x >> y;
    try{
        enemyField.attack(x, y, enemyShips, DAMAGE);
    }
    catch (const InvalidCoordinatesException& e){
        std::cout<<e.what()<<std::endl;
        execute_player_turn();
    }
    int ships_num_after = enemyShips.get_ships_count();
    std::cout<<ships_num_after<<std::endl;
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


char* get_save_info(){
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    return asctime(timeinfo);
}

void Game::save_game(const std::string& filename){
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile) {
        throw std::runtime_error("Unable to open file for saving.");
    }
    std::vector<char> sym_abilities = playerAbilities.get_abilities_as_symbols();
    GameState gameState(roundNumber, playerField, enemyField, sym_abilities);
    gameState.save(outFile, playerShips, enemyShips);

    outFile.close();
    std::cout << "Game successfully saved to " << filename << std::endl;
}


void Game::load_game(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open save file for loading.");
    }
    std::vector<char> sym_abilities = playerAbilities.get_abilities_as_symbols();
    GameState gameState;

    gameState.load(roundNumber, playerField, enemyField, playerShips, enemyShips, playerAbilities, inFile);

    inFile.close();
    std::cout << "Game successfully loaded from " << filename << std::endl;
}

*/









