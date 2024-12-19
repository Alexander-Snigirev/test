#include "InputHandler.h"
#define COMMANDS_COUNT 5
InputHandler::InputHandler(const std::string& filename) {
        loadCommandsFromFile(filename);
}

bool InputHandler::isCorrectCommands(std::vector<int> hashes){
    std::sort(hashes.begin(), hashes.end());
    // Проверяем соседние элементы
    for (size_t i = 1; i < hashes.size(); ++i) {
        if (hashes[i] == hashes[i - 1]) {
            return false;  // Найден дубликат
        }
    }

    return true;
}

void InputHandler::loadCommandsFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Using default commands." << std::endl;
            setDefaultCommands(); // Загружаем команды по умолчанию, если не удается открыть файл
            return;
        }

        std::string line;
        std::vector<int> hash_vector;
        std::hash<std::string> hasher;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            char key;
            std::string commandType;
            iss >> key >> commandType;
            hash_vector.push_back(hasher(commandType));
            if (commandType == "Attack") {
                commandMap[key] = []() {
                    int x, y;
                    std::cout << "Enter coordinates (x y): ";
                    std::cin >> x >> y;
                    return std::make_unique<AttackCommand>(x, y); // Создаем команду атаки
                };
            }
            else if (commandType == "endTurn"){
                commandMap[key] = []() {
                    return std::make_unique<EndCommand>(); //Создаем команду конца хода по умолчанию
                };
            }
            else if (commandType == "Ability"){
                commandMap[key] = []() {
                    return std::make_unique<AbilityCommand>(); //Создаем команду конца хода по умолчанию
                };
            }
            else if(commandType == "Saving"){
                commandMap[key] = []() {
                    std::string fname;
                    std::cout << "Enter filename for saving: ";
                    std::cin >> fname;
                    return std::make_unique<SaveCommand>(fname); //Создаем команду конца хода по умолчанию
                };
            }
            else if(commandType == "Loading"){
                commandMap[key] = []() {
                    std::string fname;
                    std::cout << "Enter filename for saving: ";
                    std::cin >> fname;
                    return std::make_unique<LoadCommand>(fname); //Создаем команду конца хода по умолчанию
                };
            }     
        }
        if(commandMap.size()!=COMMANDS_COUNT || !isCorrectCommands(hash_vector)){
            std::cout<<"File is ircorrect. Using default commands.\n";
            setDefaultCommands();
        }
    }

std::unique_ptr<Command> InputHandler::getCommand() {
        char input;
        std::cout << "Enter command: ";
        std::cin >> input;

        auto it = commandMap.find(input);
        if (it == commandMap.end()) {
            return nullptr;
        }
        return it->second();
    } 


void InputHandler::setDefaultCommands() {
    commandMap['a'] = []() {
        int x, y;
        std::cout << "Enter coordinates (x y): ";
        std::cin >> x >> y;
        return std::make_unique<AttackCommand>(x, y); // Создаем команду атаки по умолчанию
    };
    commandMap['u'] = []() {
        return std::make_unique<AbilityCommand>(); // Создаем команду способности по умолчанию
    };
    commandMap['x'] = []() {
        return std::make_unique<EndCommand>(); //Создаем команду конца хода по умолчанию
    };
    commandMap['s'] = []() {
        std::string fname;
        std::cout << "Enter filename for saving: ";
        std::cin >> fname;
        return std::make_unique<SaveCommand>(fname);
    };
    commandMap['l'] = []() {
        std::string fname;
        std::cout << "Enter filename: ";
        std::cin>>fname;
        return std::make_unique<LoadCommand>(fname);
    };
}       


std::vector<int> InputHandler::enter_ship_lengths() {
    std::vector<int> shipLengths;
    int numShips;
    std::cout << "Enter the number of ships: ";
    while(1){
        std::cin >> numShips;
        if(numShips>0){
            break;
        }
        std::cout<<"Error. Invalid number of ships"<<std::endl;
    }
    std::cout << "Enter the lengths of " << numShips << " ships (separated by space): ";
    for (int i = 0; i < numShips; ++i) {
        int length;
        std::cin >> length;
        if(length>0 && length<=4){
            shipLengths.push_back(length);
        }
        else{
            std::cout<<"Error. Invalid size of ships"<<std::endl;
            i--;
        }
    }
    return shipLengths;
}

std::unique_ptr<Command> InputHandler::loading(){
    std::string fname;
    std::cout<<"Start Loading... (Enter 'filename' or 'no' )"<<std::endl;
    std::cin>>fname;
    if(fname!="no"){
        return std::make_unique<LoadCommand>(fname);
    }
    return nullptr;
}


void InputHandler::enter_ships_coords(GameField& field, ShipManager& shipManager) {
    std::cout << "Place your ships on the field.\n";
    for (int i = 0; i < shipManager.get_start_length(); ++i) {
        while (true) {
            int x, y;
            char orientation;
            std::cout << "Enter coordinates (x y) and orientation (H/V) for ship of size "
                      << shipManager.get_ship(i)->get_length() << ": ";
            std::cin >> x >> y >> orientation;

            Orientation dir;
            if(orientation=='v' || orientation=='V'){
                dir = Orientation::Vertical;
            }
            else if(orientation=='h' || orientation=='H'){
                dir = Orientation::Horizontal;
            }
            shipManager.set_orientation(i,dir);
            try {
                field.place_ship(shipManager.get_ship(i).get(), x, y, dir);
                break;
            } catch (const ShipPlacementException& e) {
                std::cout << e.what() << " Try again.\n";
            }
        }
    }
}
