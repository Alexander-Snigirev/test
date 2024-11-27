#include <iostream>
#include <vector>

#include "GameField.h"
#include "ShipManager.h"
#include "skills/AbilityManager.h"
#include "GameExceptions.h"
#include "Game.h"

int main()
{
    AbilityManager abl;

    std::vector<int> shipSizes = { 4, 3, 3, 1 };

    ShipManager shipManager(shipSizes);
    GameField gameField(10, 10);

    bool placementSuccess = true;

    placementSuccess &= gameField.place_ship(shipManager.get_ship(0).get(), 0, 0, Orientation::Horizontal);
    placementSuccess &= gameField.place_ship(shipManager.get_ship(1).get(), 0, 2, Orientation::Vertical);
    placementSuccess &= gameField.place_ship(shipManager.get_ship(2).get(), 5, 4, Orientation::Horizontal);

    placementSuccess &= gameField.place_ship(shipManager.get_ship(3).get(), 7, 7, Orientation::Horizontal);

    if (!placementSuccess)
    {
        std::cerr << "Failed to place all ships on the field." << '\n';
        return 1;
    }

    std::cout << "Initial Game Field:" << '\n';
    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            std::cout << gameField.get_field_point(x, y) << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nAttacking ships..." << '\n';
    int len = shipManager.get_ships_count();
    int curr_len;
    for (int x = 0; x < 4; ++x)
    {

        gameField.attack(x, 0, shipManager,DAMAGE);
        gameField.attack(x, 0, shipManager,DAMAGE);
        curr_len=shipManager.get_ships_count();
        if(curr_len<len){
            abl.AddSkill();
        }
    }
    for (int y = 2; y <= 4; ++y)
    {
        gameField.attack(2, y, shipManager,DAMAGE);
        gameField.attack(2, y, shipManager,DAMAGE);
    }

    for (int x = 5; x <= 7; ++x)
    {
        gameField.attack(x, 5, shipManager,DAMAGE);
        gameField.attack(x, 5, shipManager,DAMAGE);
    }

    //gameField.attack(7, 7, shipManager);
    abl.UseSkill(shipManager, gameField);
    abl.UseSkill(shipManager, gameField);
    try{
        gameField.attack(-1, -1, shipManager,DAMAGE);
    }
    catch(const InvalidCoordinatesException& e){
        std::cout<<e.what()<<std::endl;
    }
    //gameField.attack(7,7,shipManager);
    std::cout << "\nGame Field after attacks:" << '\n';
    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            std::cout << gameField.get_field_point(x, y) << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nShips Status:" << '\n';
    for (int i = 0; i < shipManager.get_start_length(); ++i)
    {
        const auto& ship = shipManager.get_ship(i);
        std::cout << "Ship " << i + 1 << " (size " << ship->get_length() << "): ";
        if (ship->is_destroyed())
        {
            std::cout << "Destroyed";
        }
        else
        {
            std::cout << "Not destroyed";
        }
        std::cout << '\n';
    }
    
    std::cout << "\nShips remaining: " << shipManager.get_ships_count() << '\n';


    return 0;
}
