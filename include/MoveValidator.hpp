#pragma once

#include "ChessBoard.hpp"

extern const Position directions[8];
extern const Position l_shape_directions[8];

class MoveValidator {
  private:
    ChessBoard& board_;
    std::vector<Position> moveable_into_positions_;
    
    bool check(Position selection);
    Position sumPositions(const Position& a, const Position& b, const Position& c = {0,0});
    void transferToBoard();

  public:
    MoveValidator(ChessBoard& board);
    void setMoveableIntoPositions(InputPosition& input);
    std::vector<Position> getMoveableIntoPositions();
    void clearMoveableIntoPositions();
};