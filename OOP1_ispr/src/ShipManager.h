#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include <memory>
#include <vector>

#include "Ship.h"

class ShipManager
{
public:
    ShipManager(const std::vector<int>& shipSizes);
    const std::unique_ptr<Ship>& get_ship(int index) const;
    void change_ship_status(Ship* ship);
    int get_start_length() const;
    int get_ships_count() const;
    bool check_all_ships() const;

private:
    const int MIN_LENGTH = 1;
    const int MAX_LENGTH = 4;
    std::vector<std::unique_ptr<Ship>> ships;
    std::vector<bool> ship_is_destroy;
    int ships_count;

    ShipManager(const ShipManager&) = delete;
    ShipManager& operator=(const ShipManager&) = delete;
};

#endif
