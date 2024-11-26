#include "Player.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    this_thread::sleep_for(chrono::milliseconds(500));
}

Player::Player(const string& name) : name(name) {}

void Player::placeShips() {
    int shipSizes[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    for (int i = 0; i < MAX_SHIPS; ++i) {
        clearScreen();
        ownBoard.display(true);
        cout << name << ", place a ship of size " << shipSizes[i] << endl;

        int row, col, direction;
        bool placed = false;
        while (!placed) {
            cout << "Enter the starting coordinates of the ship (column and row): ";
            cin >> col >> row;

            if (cin.fail() || col < 0 || col >= BOARD_SIZE || row < 0 || row >= BOARD_SIZE) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid coordinates! Try again." << endl;
                continue;
            }

            cout << "Is it horizontal (1) or vertical (0)? ";
            cin >> direction;

            if (direction != 0 && direction != 1) {
                cout << "Invalid choice. Try again." << endl;
                continue;
            }

            placed = ownBoard.placeShip(shipSizes[i], row, col, direction == 1);
            if (!placed) cout << "Invalid coordinates or space occupied. Try again." << endl;
        }
    }
    clearScreen();
}
