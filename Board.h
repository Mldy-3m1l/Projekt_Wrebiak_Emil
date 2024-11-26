#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "Ship.h"

const int BOARD_SIZE = 10;
const int MAX_SHIPS = 10;

class Board {
private:
    char grid[BOARD_SIZE][BOARD_SIZE];
    Ship ships[MAX_SHIPS];
    int shipCount;
    bool attackHistory[BOARD_SIZE][BOARD_SIZE];

    bool checkAdjacentCells(int row, int col) const;

public:
    Board();

    void display(bool revealShips = false) const;
    bool isValidPlacement(int row, int col, int size, bool horizontal) const;
    bool placeShip(int size, int row, int col, bool horizontal);
    bool attack(int row, int col);
    bool allShipsSunk() const;
};

#endif // BOARD_H
