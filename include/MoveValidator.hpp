#pragma once

#include "ChessBoard.hpp"

class MoveValidator {
  private:
    const ChessBoard& board_;
    std::vector<Position> moveable_into_positions_;
    
    bool check(Position selection);
  public:
    MoveValidator(const ChessBoard& board);
    void setMoveableIntoPositions(Position selection);
    std::vector<Position> getMoveableIntoPositions();
};