#include "Ship.h"

#include <stdexcept>

Ship::Ship(int length, Orientation orientation): length(length), orientation(orientation)
{
    if (length < MIN_LENGTH || length > MAX_LENGTH)
    {
        throw std::invalid_argument("Ship length must be between 1 and 4.");
    }
    segment_arr.resize(length, DamageLevel::Intact);
}

Ship::~Ship()
{
    // No dynamic memory allocation; nothing to clean up
}
Ship::Ship(const Ship& other) : length(other.length), orientation(other.orientation), segment_arr(other.segment_arr) {
    // Копируем длину, ориентацию и состояние сегментов
}

// Оператор присваивания
Ship& Ship::operator=(const Ship& other) {
    if (this == &other) {
        return *this; // Проверка на самоприсваивание
    }

    length = other.length;
    orientation = other.orientation;
    segment_arr = other.segment_arr; // Используем копирующее присваивание std::vector

    return *this;
}

int Ship::get_length() const
{
    return length;
}

Orientation Ship::get_orientation() const
{
    return orientation;
}

SegmentStatus Ship::get_seg_status(int index) const
{
    if (index < 0 || index >= length)
    {
        throw std::out_of_range("Segment index out of range.");
    }
    switch (segment_arr[index])
    {
        case DamageLevel::Destroyed:
            return SegmentStatus::Destroyed;
        case DamageLevel::Damaged:
            return SegmentStatus::Damaged;
        case DamageLevel::Intact:
        default:
            return SegmentStatus::Intact;
    }
}

void Ship::decrease_health(int index, int damage)
{
    if (index < 0 || index >= length)
    {
        throw std::out_of_range("Segment index out of range.");
    }
    if (damage < 0)
    {
        throw std::invalid_argument("Damage cannot be negative.");
    }

    int currentDamage = static_cast<int>(segment_arr[index]) + damage;

    if (currentDamage >= static_cast<int>(DamageLevel::Destroyed))
    {
        segment_arr[index] = DamageLevel::Destroyed;
    }
    else if (currentDamage >= static_cast<int>(DamageLevel::Damaged))
    {
        segment_arr[index] = DamageLevel::Damaged;
    }
}

bool Ship::is_destroyed() const
{
    for (const auto& dmg : segment_arr)
    {
        if (dmg != DamageLevel::Destroyed)
        {
            return false;
        }
    }
    return true;
}
