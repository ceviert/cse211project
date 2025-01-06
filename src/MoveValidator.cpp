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

void MoveValidator::setPositions(Position& input) {

  moveable_into_positions_.clear();

  const auto& tiles = board_.getChessTiles();
  const auto& tile = tiles[input.y][input.x];
  const auto& rules = tile.movement;
  const Position original = input;
  int i;
  if (rules.first_move_forward) { // PAWN
    if (tile.white) {
      i = rules.first_move_forward;
      do {
        input = sumPositions(input, directions[3]);
        if (check(input)) moveable_into_positions_.push_back(input);
        i--;
      } while (original.y == 1 && i);
      input = original;
      Position lowerleft_of_input = sumPositions(input, directions[6]);
      if (tiles[lowerleft_of_input.y][lowerleft_of_input.x].white != tile.white) capturable_positions_.push_back(lowerleft_of_input);
      Position lowerright_of_input = sumPositions(input, directions[7]);
      if (tiles[lowerright_of_input.y][lowerright_of_input.x].white != tile.white) capturable_positions_.push_back(lowerright_of_input);
    }
    else {
      i = rules.first_move_forward;
      do {
        input = sumPositions(input, directions[2]);
        if (check(input)) moveable_into_positions_.push_back(input);
        i--;
      } while (original.y == 6 && i);
      input = original;
      Position upperleft_of_input = sumPositions(input, directions[5]);
      if (tiles[upperleft_of_input.y][upperleft_of_input.x].white != tile.white) capturable_positions_.push_back(upperleft_of_input);
      Position upperright_of_input = sumPositions(input, directions[4]);
      if (tiles[upperright_of_input.y][upperright_of_input.x].white != tile.white) capturable_positions_.push_back(upperright_of_input);
    }
  }
  else {
    if (rules.forward) { // CHECK VERTICAL
      const Position& up = directions[2]; // UP
      input = sumPositions(input, up);
      i = rules.forward;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, up);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
      const Position& down = directions[3]; // DOWN
      input = sumPositions(input, down);
      i = rules.forward;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, down);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
    }
    if (rules.sideways) { // CHECK HORIZONTAL
      const Position& right = directions[0]; // RIGHT
      input = sumPositions(input, right);
      i = rules.sideways;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, right);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
      const Position& left = directions[1]; // LEFT
      input = sumPositions(input, left);
      i = rules.sideways;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, left);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
    }
    if (rules.diagonal) {
      const Position& upperright = directions[4]; // UPPER RIGHT
      input = sumPositions(input, upperright);
      i = rules.diagonal;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, upperright);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
      const Position& upperleft = directions[5]; // UPPER LEFT
      input = sumPositions(input, upperleft);
      i = rules.diagonal;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, upperleft);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      const Position& lowerleft = directions[6]; // LOWER LEFT
      input = sumPositions(input, lowerleft);
      i = rules.diagonal;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, lowerleft);
        i--;
      }
      if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
      const Position& lowerright = directions[7]; // LOWER RIGHT
      input = sumPositions(input, lowerright);
      i = rules.diagonal;
      while (check(input) && i) {
        moveable_into_positions_.push_back(input);
        input = sumPositions(input, lowerright);
        i--;
      }
    }
    if (rules.l_shape) {
      for (const auto& direction : l_shape_directions) {
        input = sumPositions(input, direction);
        if (check(input)) {
          moveable_into_positions_.push_back(input);
        }
        else if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
        input = original;
      }
    }
  }
  transferToBoard();
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
  for (auto& tile_row : tiles) {
     for (auto& tile : tile_row) {
      tile.tile_type.moveable_into_tile = false;
      tile.tile_type.capturable_tile = false;
     }
  }
  for (const auto& position : moveable_into_positions_) {
    tiles[position.y][position.x].tile_type.moveable_into_tile = true;
  }
  for (const auto& position : capturable_positions_) {
    tiles[position.y][position.x].tile_type.capturable_tile = true;
  }
}

void MoveValidator::clearPositions() {
  moveable_into_positions_.clear();
  capturable_positions_.clear();
  transferToBoard();
}

bool MoveValidator::isVectorsEmpty() {
  if (moveable_into_positions_.empty() && capturable_positions_.empty()) return true;
  else return false;
}

MOVE_TYPE MoveValidator::validateMove(const Position& target) {
  for (const auto& pos : moveable_into_positions_) {
    if (target.x == pos.x && target.y == pos.y) return MOVE;
  }

  for (const auto& pos : capturable_positions_) {
    if (target.x == pos.x && target.y == pos.y) return CAPTURE;
  }

  return INVALID;
}