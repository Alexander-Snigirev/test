#include "ShipManager.h"

#include <stdexcept>

ShipManager::ShipManager(const std::vector<int>& shipSizes) : ships_count(0)
{
    for (int size : shipSizes)
    {
        if (size < MIN_LENGTH || size > MAX_LENGTH)
        {
            throw std::invalid_argument("Invalid ship size.");
        }
        ships.push_back(std::make_unique<Ship>(size, Orientation::Horizontal));
        ship_is_destroy.push_back(false);
        ships_count++;
    }
}

const std::unique_ptr<Ship>& ShipManager::get_ship(int index) const
{
    return ships[index];
}

void ShipManager::change_ship_status(Ship* ship)
{
    for (size_t i = 0; i < ships.size(); ++i)
    {
        if (ships[i].get() == ship)
        {
            if (!ship_is_destroy[i] && ship->is_destroyed())
            {
                ship_is_destroy[i] = true;
                ships_count--;
            }
            break;
        }
    }
}

int ShipManager::get_start_length() const
{
    return ships.size();
}

int ShipManager::get_ships_count() const
{
    return ships_count;
}

bool ShipManager::check_all_ships() const
{
    return ships_count == 0;
}
