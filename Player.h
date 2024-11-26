#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"

class Player {
public:
    Board ownBoard;
    Board targetBoard;
    std::string name;

    Player(const std::string& name);

    void placeShips();
};

#endif // PLAYER_H
