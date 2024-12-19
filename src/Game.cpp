#include "Game.h"


Game::Game()
    : playerField(10, 10), enemyField(10, 10), playerShips(),
      enemyShips({1}), playerAbilities(), isPlayerTurn(true), roundNumber(1), acounter(0), ucounter(0), endflag(0) {}

void Game::start_new_game() {
    InputHandler startInput("commands.txt");
    GameController<Game, InputHandler> startController(*this, startInput);
    bool ld = startController.start_loading();
    if(ld){ 
        start_round();
        return;
    }
    /*char ans;
    std::cout<<"Do you want to load save?(y/n)"<<std::endl;
    std::cin>>ans;
    if(ans=='y'){
        loading();
        start_round();
        return;
    }*/
    playerField = GameField(10, 10);
    
    auto shipLengths = startInput.enter_ship_lengths();
    playerShips = ShipManager(shipLengths);
    startInput.enter_ships_coords(playerField, playerShips);


    playerAbilities = AbilityManager();
    isPlayerTurn = true;
    roundNumber = 1;

    initialize_round();
    start_round();
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
}



void Game::start_round() {
    bool defeat = false;
    InputHandler inputHandler("control/commands.txt");
    FieldRenderer fieldRenderer;

    // Создаем контроллер и рендерер
    GameController<Game, InputHandler> controller(*this, inputHandler);
    GameRenderer renderer(fieldRenderer);

    while (true) {
        if (isPlayerTurn){
            endflag=0;
            acounter=0;
            ucounter=0;
            while(1){
                renderer.render(playerField, enemyField, playerAbilities);
                controller.processInput();
                if(endflag) break;
            }
        } else {
            execute_enemy_turn();
        }

        if (check_victory()) {
            ++roundNumber;
            break;
        }

        if (check_defeat()) {
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

/*void Game::loading(){
    std::string file_name;
    std::cout<<"Enter the filename\n";
    std::cin>>file_name;
    try{
        load_game(file_name);
    } catch(std::runtime_error& e){
        std::cout<<e.what()<<std::endl;
        loading();
    }
}*/


void Game::setEndFlag(){
    endflag = 1;
}

void Game::UseAbility(){
    if(ucounter){
        throw CommandException();
    }
    int ships_num_before = enemyShips.get_ships_count();
    playerAbilities.UseSkill(playerShips, enemyField);
    ucounter=1;

    int ships_num_after = enemyShips.get_ships_count();
    if (ships_num_after < ships_num_before) {
        playerAbilities.AddSkill();
    }
}

void Game::execute_player_turn(int x, int y) {
       
    int ships_num_before = enemyShips.get_ships_count();
    if(acounter){
        throw CommandException();
    }
    enemyField.attack(x, y, enemyShips, DAMAGE);
    acounter=1;
    int ships_num_after = enemyShips.get_ships_count();
    if (ships_num_after < ships_num_before) {
        playerAbilities.AddSkill();
    }
}

void Game::execute_enemy_turn() {
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
}











