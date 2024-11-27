#ifndef SHIP_H
#define SHIP_H
#define DAMAGE 1
#define D_DAMAGE 2
#include <vector>

enum class Orientation
{
    Horizontal,
    Vertical
};

enum class SegmentStatus
{
    Intact,
    Damaged,
    Destroyed
};

enum class DamageLevel
{
    Intact,
    Damaged,
    Destroyed
};

class Ship
{
public:
    Ship(int length, Orientation orientation);
    ~Ship();

    int get_length() const;
    Orientation get_orientation() const;
    SegmentStatus get_seg_status(int index) const;
    void decrease_health(int index, int damage);
    bool is_destroyed() const;
    Ship(const Ship&);
    Ship& operator=(const Ship&);

private:
    const int MIN_LENGTH = 1;
    const int MAX_LENGTH = 4;
    int length;
    Orientation orientation;
    std::vector<DamageLevel> segment_arr;

    
};

#endif
