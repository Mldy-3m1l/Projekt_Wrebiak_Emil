#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

const int BOARD_SIZE = 10;
const int MAX_SHIPS = 10;


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    this_thread::sleep_for(chrono::milliseconds(500));
}

class Ship {
public:
    int size;
    int row, col;
    bool isHorizontal;
    int hits;

    Ship() : size(0), row(0), col(0), isHorizontal(true), hits(0) {}

    Ship(int size, int row, int col, bool isHorizontal)
        : size(size), row(row), col(col), isHorizontal(isHorizontal), hits(0) {}

    bool isSunk() const {
        return hits == size;
    }
};

class Board {
private:
    char grid[BOARD_SIZE][BOARD_SIZE];
    Ship ships[MAX_SHIPS];
    int shipCount;
    bool attackHistory[BOARD_SIZE][BOARD_SIZE];

public:
    Board() : shipCount(0) {
        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j)
                grid[i][j] = '0';

        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j)
                attackHistory[i][j] = false;
    }

    void display(bool revealShips = false) const {
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

    bool isValidPlacement(int row, int col, int size, bool horizontal) const {
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

    bool checkAdjacentCells(int row, int col) const {
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

    bool placeShip(int size, int row, int col, bool horizontal) {
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

    bool attack(int row, int col) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            cout << "Invalid coordinates! Try again." << endl;
            return false;
        }

        if (attackHistory[row][col]) {
            cout << "This cell has already been attacked! Try again." << endl;
            return false;
        }

        attackHistory[row][col] = true;

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
                        return true;
                    }
                } else {
                    if (ship.col == col && row >= ship.row && row < ship.row + ship.size) {
                        ship.hits++;
                        if (ship.isSunk()) {
                            cout << "You sunk a ship!" << endl;
                        } else {
                            cout << "Hit!" << endl;
                        }
                        return true;
                    }
                }
            }
        }

        if (grid[row][col] == '0') {
            grid[row][col] = 'O';
            cout << "Miss!" << endl;
        }
        return true;
    }

    bool allShipsSunk() const {
        for (int i = 0; i < shipCount; ++i) {
            if (!ships[i].isSunk()) return false;
        }
        return true;
    }
};

class Player {
public:
    Board ownBoard;
    Board targetBoard;
    string name;

    Player(const string& name) : name(name) {}

    void placeShips() {
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
};

void gameLoop(Player& player1, Player& player2) {
    bool gameOn = true;
    while (gameOn) {
        for (Player* attacker : {&player1, &player2}) {
            clearScreen();
            cout << attacker->name << ", your board:" << endl;
            attacker->ownBoard.display(true);
            cout << "Target board:" << endl;
            attacker->targetBoard.display();

            cout << attacker->name << ", your turn to attack!" << endl;
            int row, col;
            bool validAttack = false;

            while (!validAttack) {
                cout << "Enter coordinates to hit (column and row): ";
                cin >> col >> row;

                if (cin.fail() || col < 0 || col >= BOARD_SIZE || row < 0 || row >= BOARD_SIZE) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong coordinates, please try again." << endl;
                } else {
                    validAttack = (attacker == &player1) ? player2.ownBoard.attack(row, col) : player1.ownBoard.attack(row, col);
                    if (!validAttack) {
                        cout << "This cell has already been attacked or is invalid. Please try again." << endl;
                    }
                }
            }

            if (validAttack) {
                attacker->targetBoard.attack(row, col);
            }

            this_thread::sleep_for(chrono::seconds(1));


            if ((attacker == &player1 && player2.ownBoard.allShipsSunk()) ||
                (attacker == &player2 && player1.ownBoard.allShipsSunk())) {
                cout << attacker->name << " wins!" << endl;
                gameOn = false;
                break;
            }

            clearScreen();

            cout << "Press Enter to continue " << (attacker == &player1 ? player2.name : player1.name) << "'s turn." << endl;       \

            cin.ignore();
            cin.get();
        }
    }
}

int main() {
    cout << R"(
 __      _____________.____   _________  ________      _____  ___________       ___________________
/  \    /  \_   _____/|    |  \_   ___ \ \_____  \    /     \ \_   _____/       \__    ___/\_____  \
\   \/\/   /|    __)_ |    |  /    \  \/  /   |   \  /  \ /  \ |    __)_          |    |    /   |   \
 \        / |        \|    |__\     \____/    |    \/    Y    \|        \         |    |   /    |    \
  \__/\  / /_______  /|_______ \______  /\_______  /\____|__  /_______  /         |____|   \_______  /
       \/          \/         \/      \/         \/         \/        \/                           \/
       __________    ___________________________.____     ___________ _________ ___ ___ ._____________  _________
       \______   \  /  _  \__    ___/\__    ___/|    |    \_   _____//   _____//   |   \|   \______   \/   _____/
        |    |  _/ /  /_\  \|    |     |    |   |    |     |    __)_ \_____  \/    ~    \   ||     ___/\_____  \
        |    |   \/    |    \    |     |    |   |    |___  |        \/        \    Y    /   ||    |    /        \
        |______  /\____|__  /____|     |____|   |_______ \/_______  /_______  /\___|_  /|___||____|   /_______  /
               \/         \/                            \/        \/        \/       \/                       \/
    )" << endl;
    string name1, name2;
    cout << "Enter Player 1's name: ";
    getline(cin, name1);
    cout << "Enter Player 2's name: ";
    getline(cin, name2);

    Player player1(name1);
    Player player2(name2);

    player1.placeShips();
    player2.placeShips();

    gameLoop(player1, player2);

    return 0;
}
