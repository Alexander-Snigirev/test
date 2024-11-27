#include "Input.h"

std::vector<int> Input::get_ship_lengths() {
    std::vector<int> shipLengths;
    int numShips;
    std::cout << "Enter the number of ships: ";
    std::cin >> numShips;

    std::cout << "Enter the lengths of " << numShips << " ships (separated by space): ";
    for (int i = 0; i < numShips; ++i) {
        int length;
        std::cin >> length;
        shipLengths.push_back(length);
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

            Orientation dir = (orientation == 'H' || orientation == 'h') ? Orientation::Horizontal : Orientation::Vertical;
            try {
                field.place_ship(shipManager.get_ship(i).get(), x, y, dir);
                break;
            } catch (const ShipPlacementException& e) {
                std::cout << e.what() << " Try again.\n";
            }
        }
    }
}

std::pair<int, int> Input::get_attack_coordinates() {
    int x, y;
    std::cout << "Enter coordinates for attack (x y): ";
    std::cin >> x >> y;
    return {x, y};
}

void Input::use_ability(AbilityManager& abilityManager, GameField& field, ShipManager& shipManager) {
    try {
        abilityManager.UseSkill(shipManager, field);
    } catch (const UsingAbilityException& e) {
        std::cout << e.what() << std::endl;
    }
}
