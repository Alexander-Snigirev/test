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
ShipManager::ShipManager() : ships_count(0){}

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

ShipManager::ShipManager(const ShipManager& other) : ships_count(other.ships_count) {
    for (const auto& ship : other.ships) {
        ships.push_back(std::make_unique<Ship>(*ship));
    }
    ship_is_destroy = other.ship_is_destroy;
}

// Оператор присваивания
ShipManager& ShipManager::operator=(const ShipManager& other) {
    if (this == &other) {
        return *this; // Самоприсваивание
    }

    // Очистка текущего состояния
    ships.clear();
    ship_is_destroy.clear();

    // Копирование состояния из другого объекта
    ships_count = other.ships_count;
    for (const auto& ship : other.ships) {
        ships.push_back(std::make_unique<Ship>(*ship));
    }
    ship_is_destroy = other.ship_is_destroy;

    return *this;
}

int ShipManager::get_vector_size(){
    return ships.size();
}

void ShipManager::save(std::ostream& out) const {
    int vectorSize = static_cast<int>(ships.size());

    out.write(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize)); // Сохраняем количество кораблей
    out.write(reinterpret_cast<const char*>(&ships_count), sizeof(ships_count)); // Сохраняем количество целых кораблей

    for (size_t i = 0; i < ships.size(); ++i) {
        int id = static_cast<int>(i); // Уникальный ID корабля
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));

        // Сохраняем корабль
        const Ship& ship = *ships[i];
        int length = ship.get_length();
        int orientation = static_cast<int>(ship.get_orientation());
        std::cout<<orientation<<std::endl;


        out.write(reinterpret_cast<const char*>(&length), sizeof(length));
        out.write(reinterpret_cast<const char*>(&orientation), sizeof(orientation));

        // Сохраняем состояние сегментов
        //const auto& segments = ship.get_segments();
        for (int i=0;i<ship.get_length();i++) {
            int status = static_cast<int>(ship.get_seg_status(i));
            out.write(reinterpret_cast<const char*>(&status), sizeof(status));
        }
    }
}

void ShipManager::load(std::istream& in) {
    int vectorSize;
    int countShips;
    in.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));
    in.read(reinterpret_cast<char*>(&countShips), sizeof(countShips));
    if (!in) {
        throw std::runtime_error("Error loading ship count.");
    }
    ships_count = countShips;
    ships.clear();
    for (int i = 0; i < vectorSize; ++i) {
        int id, length, orientation;
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        in.read(reinterpret_cast<char*>(&length), sizeof(length));
        in.read(reinterpret_cast<char*>(&orientation), sizeof(orientation));


        // Создаем новый корабль
        auto ship = std::make_unique<Ship>(length, static_cast<Orientation>(orientation));

        // Загружаем состояние сегментов корабля
        for (int j = 0; j < length; ++j) {
            int status;
            in.read(reinterpret_cast<char*>(&status), sizeof(status));
            ship->set_segment_status(j, static_cast<DamageLevel>(status));
        }

        ships.push_back(std::move(ship));
        ship_is_destroy.push_back(false); // Предполагаем, что нужно добавить флаг "не уничтожен"
    }
}

void ShipManager::set_orientation(size_t idx, Orientation orient){
    ships[idx].get()->set_orientation(orient);
}


int ShipManager::get_ship_index(const Ship* ship) const {
    for (size_t i = 0; i < ships.size(); ++i) {
        if (ships[i].get() == ship) {
            return static_cast<int>(i);
        }
    }
    return -1; // Корабль не найден
}


