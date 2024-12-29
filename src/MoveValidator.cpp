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

void MoveValidator::setPositions(InputPosition& input) {

  Position selection = convertToPosition(input);

  moveable_into_positions_.clear();

  const auto& tiles = board_.getChessTiles();
  const auto& tile = tiles[selection.y][selection.x];
  const auto& rules = tile.movement;
  const Position original = selection;
  int i;
  if (rules.first_move_forward) { // PAWN
    if (tile.white) {
      i = rules.first_move_forward;
      do {
        selection = sumPositions(selection, directions[3]);
        if (check(selection)) moveable_into_positions_.push_back(selection);
        i--;
      } while (original.y == 1 && i);
      selection = original;
      Position lowerleft_of_selection = sumPositions(selection, directions[6]);
      if (tiles[lowerleft_of_selection.y][lowerleft_of_selection.x].white != tile.white) capturable_positions_.push_back(lowerleft_of_selection);
      Position lowerright_of_selection = sumPositions(selection, directions[7]);
      if (tiles[lowerright_of_selection.y][lowerright_of_selection.x].white != tile.white) capturable_positions_.push_back(lowerright_of_selection);
    }
    else {
      i = rules.first_move_forward;
      do {
        selection = sumPositions(selection, directions[2]);
        if (check(selection)) moveable_into_positions_.push_back(selection);
        i--;
      } while (original.y == 6 && i);
      selection = original;
      Position upperleft_of_selection = sumPositions(selection, directions[5]);
      if (tiles[upperleft_of_selection.y][upperleft_of_selection.x].white != tile.white) capturable_positions_.push_back(upperleft_of_selection);
      Position upperright_of_selection = sumPositions(selection, directions[4]);
      if (tiles[upperright_of_selection.y][upperright_of_selection.x].white != tile.white) capturable_positions_.push_back(upperright_of_selection);
    }
  }
  else {
    if (rules.forward) { // CHECK VERTICAL
      const Position& up = directions[2]; // UP
      selection = sumPositions(selection, up);
      i = rules.forward;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, up);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      selection = original;
      const Position& down = directions[3]; // DOWN
      selection = sumPositions(selection, down);
      i = rules.forward;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, down);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      selection = original;
    }
    if (rules.sideways) { // CHECK HORIZONTAL
      const Position& right = directions[0]; // RIGHT
      selection = sumPositions(selection, right);
      i = rules.sideways;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, right);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      selection = original;
      const Position& left = directions[1]; // LEFT
      selection = sumPositions(selection, left);
      i = rules.sideways;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, left);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      selection = original;
    }
    if (rules.diagonal) {
      const Position& upperright = directions[4]; // UPPER RIGHT
      selection = sumPositions(selection, upperright);
      i = rules.diagonal;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, upperright);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      selection = original;
      const Position& upperleft = directions[5]; // UPPER LEFT
      selection = sumPositions(selection, upperleft);
      i = rules.diagonal;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, upperleft);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      const Position& lowerleft = directions[6]; // LOWER LEFT
      selection = sumPositions(selection, lowerleft);
      i = rules.diagonal;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, lowerleft);
        i--;
      }
      if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
      selection = original;
      const Position& lowerright = directions[7]; // LOWER RIGHT
      selection = sumPositions(selection, lowerright);
      i = rules.diagonal;
      while (check(selection) && i) {
        moveable_into_positions_.push_back(selection);
        selection = sumPositions(selection, lowerright);
        i--;
      }
    }
    if (rules.l_shape) {
      for (const auto& direction : l_shape_directions) {
        selection = sumPositions(selection, direction);
        if (check(selection)) {
          moveable_into_positions_.push_back(selection);
        }
        else if (tiles[selection.y][selection.x].white != tile.white) capturable_positions_.push_back(selection);
        selection = original;
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