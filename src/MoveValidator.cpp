#include "MoveValidator.hpp"

const Position directions[8] = {
  {1,0},   // RIGHT
  {-1,0},  // LEFT
  {0,-1},  // UP
  {0,1},   // DOWN
  {1,-1},  // UPPER RIGHT
  {-1,-1}, // UPPER LEFT
  {-1,1},  // LOWER LEFT
  {1,1}    // LOWER RIGHT
};

const Position l_shape_directions[8] = {
  {1,2},   // DOWN L FACING RIGHT
  {-1,2},  // DOWN L FACING LEFT
  {1,-2},  // UP L FACING RIGHT
  {-1,-2}, // UP L FACING LEFT
  {2,-1},  // RIGHT L FACING UP
  {2,1},   // RIGHT L FACING DOWN
  {-2,-1}, // LEFT L FACING UP
  {-2,1}   // LEFT L FACING DOWN 
}; 

MoveValidator::MoveValidator(ChessBoard& board) : board_(board) {}

void MoveValidator::setMoveableIntoPositions(InputPosition& input) {

  Position selection = convertToPosition(input);

  moveable_into_positions_.clear();

  const auto& tiles = board_.getChessTiles();
  const auto& tile = tiles[selection.y][selection.x];
  const auto& rules = tile.movement;
  const Position original = selection;
  int i;
  if (rules.forward) { // CHECK VERTICAL
    const Position& up = directions[2]; // UP
    selection = sumPositions(selection, up);
    i = rules.forward - 1;
    while (check(selection) && i) {
      moveable_into_positions_.push_back(selection);
      selection = sumPositions(selection, up);
      i--;
    }
    selection = original;
    i = rules.forward - 1;
    const Position& down = directions[3]; // DOWN
    selection = sumPositions(selection, down);
    while (check(selection) && i) {
      moveable_into_positions_.push_back(selection);
      selection = sumPositions(selection, down);
      i--;
    }
    selection = original;
  }
  if (rules.sideways) { // CHECK HORIZONTAL
    const Position& right = directions[0]; // RIGHT
    selection = sumPositions(selection, right);
    i = rules.sideways - 1;
    while (check(selection) && i) {
      moveable_into_positions_.push_back(selection);
      selection = sumPositions(selection, right);
      i--;
    }
    selection = original;
    i = rules.forward - 1;
    const Position& left = directions[3]; // LEFT
    selection = sumPositions(selection, left);
    while (check(selection) && i) {
      moveable_into_positions_.push_back(selection);
      selection = sumPositions(selection, left);
      i--;
    }
    selection = original;
  }
  transferToBoard();
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

Position MoveValidator::sumPositions(const Position& a, const Position& b, const Position& c) {
  Position result;
  result.x = a.x + b.x + c.x;
  result.y = a.y + b.y + c.y;
  return result;
}

void MoveValidator::transferToBoard() {
  auto& tiles = board_.getChessTiles();
  for (const auto& position : moveable_into_positions_) {
    tiles[position.y][position.x].tile_type.moveable_into_tile = true;
    std::cout << position.x << position.y << std::endl;
  }
}

void MoveValidator::clearMoveableIntoPositions() {
  moveable_into_positions_.clear();
}