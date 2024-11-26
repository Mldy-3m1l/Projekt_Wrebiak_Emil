#ifndef SHIP_H
#define SHIP_H

class Ship {
public:
    int size;
    int row, col;
    bool isHorizontal;
    int hits;

    Ship();
    Ship(int size, int row, int col, bool isHorizontal);

    bool isSunk() const;
};

#endif // SHIP_H
