#include "Ship.h"

Ship::Ship() : size(0), row(0), col(0), isHorizontal(true), hits(0) {}

Ship::Ship(int size, int row, int col, bool isHorizontal)
    : size(size), row(row), col(col), isHorizontal(isHorizontal), hits(0) {}

bool Ship::isSunk() const {
    return hits == size;
}
