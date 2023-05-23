#include "Board.hpp"

class Position {
  public:
    Position(Board *chessboard, Move move)
        : echiquier(chessboard), coup(move) {}
    Board *echiquier;
    Move coup;
};