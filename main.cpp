#include <iostream>
#include "Player.cc"
#include "Board.cc"
#include "Ship.cc"

using namespace std;

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
                    // Sprawdzamy poprawnoœæ ataku
                    validAttack = (attacker == &player1)
                        ? player2.ownBoard.attack(row, col)
                        : player1.ownBoard.attack(row, col);
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

            cout << "Press Enter to continue " << (attacker == &player1 ? player2.name : player1.name) << "'s turn." << endl;
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
