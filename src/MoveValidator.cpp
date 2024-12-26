#include "MoveValidator.hpp"

MoveValidator::MoveValidator(const ChessBoard& board) : board_(board) {}

void MoveValidator::setMoveableIntoPositions(Position selection) {
  moveable_into_positions_.clear();
  auto& tiles = board_.getChessTiles();
  ChessTile tile = tiles[selection.y][selection.x];
  
}

std::vector<Position> MoveValidator::getMoveableIntoPositions() {
  return moveable_into_positions_;
}

bool MoveValidator::check(Position selection) {
  const auto& tiles = board_.getChessTiles();
  const auto& tile = tiles[selection.y][selection.x];
  if (!tile.tile_type.is_occupied) return true;
  else return false;
}

