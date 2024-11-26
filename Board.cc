#include "Board.h"
#include <limits>

using namespace std;

Board::Board() : shipCount(0) {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            grid[i][j] = '0';

    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            attackHistory[i][j] = false;
}

void Board::display(bool revealShips) const {
    cout << "   ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
        cout << j << " ";
    }
    cout << endl;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] == 'S' && !revealShips) {
                cout << " -";
            } else if (grid[i][j] == 'S') {
                cout << " #";
            } else if (grid[i][j] == '0') {
                cout << " -";
            } else if (grid[i][j] == 'X') {
                cout << " X";
            } else if (grid[i][j] == 'O') {
                cout << " O";
            } else {
                cout << grid[i][j] << " ";
            }
        }
        cout << endl;
    }
}

bool Board::checkAdjacentCells(int row, int col) const {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                if (grid[newRow][newCol] == 'S')
                    return true;
            }
        }
    }
    return false;
}

bool Board::isValidPlacement(int row, int col, int size, bool horizontal) const {
    if (horizontal) {
        if (col + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i)
            if (grid[row][col + i] != '0' || checkAdjacentCells(row, col + i))
                return false;
    } else {
        if (row + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i)
            if (grid[row + i][col] != '0' || checkAdjacentCells(row + i, col))
                return false;
    }
    return true;
}

bool Board::placeShip(int size, int row, int col, bool horizontal) {
    if (shipCount >= MAX_SHIPS) return false;
    if (!isValidPlacement(row, col, size, horizontal))
        return false;

    ships[shipCount] = Ship(size, row, col , horizontal);
    shipCount++;

    if (horizontal) {
        for (int i = 0; i < size; ++i)
            grid[row][col + i] = 'S';
    } else {
        for (int i = 0; i < size; ++i)
            grid[row + i][col] = 'S';
    }

    return true;
}

bool Board::attack(int row, int col) {
    // Sprawdzenie współrzędnych
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        cout << "Invalid coordinates! Try again." << endl;
        return false;
    }

    // Sprawdzenie czy pole już było zaatakowane
    if (attackHistory[row][col]) {
        cout << "This cell has already been attacked! Try again." << endl;
        return false;
    }

    // Oznacz pole jako zaatakowane
    attackHistory[row][col] = true;

    // Trafienie w statek
    if (grid[row][col] == 'S') {
        grid[row][col] = 'X'; 
        for (int i = 0; i < shipCount; ++i) {
            Ship& ship = ships[i];
            if (ship.isHorizontal) {
                if (ship.row == row && col >= ship.col && col < ship.col + ship.size) {
                    ship.hits++;
                    if (ship.isSunk()) {
                        cout << "You sunk a ship!" << endl;
                    } else {
                        cout << "Hit!" << endl;
                    }
                    return true; // kończy funkcje po trafieniu
                }
            } else {
                if (ship.col == col && row >= ship.row && row < ship.row + ship.size) {
                    ship.hits++;
                    if (ship.isSunk()) {
                        cout << "You sunk a ship!" << endl;
                    } else {
                        cout << "Hit!" << endl;
                    }
                    return true; // kończy funkcje po trafieniu
                }
            }
        }
    }

    // Pudło
    if (grid[row][col] == '0') {
        grid[row][col] = 'O'; 
        cout << "Miss!" << endl;
        return true; 
    }

    return true; // Domyślny zwrot w przypadku poprawnego ataku
}



bool Board::allShipsSunk() const {
    for (int i = 0; i < shipCount; ++i) {
        if (!ships[i].isSunk()) return false;
    }
    return true;
}
