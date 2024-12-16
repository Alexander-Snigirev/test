#include "Input.h"

std::vector<int> Input::get_ship_lengths() {
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

void Input::place_ships(GameField& field, ShipManager& shipManager) {
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


