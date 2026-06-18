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

void MoveValidator::setPositions(const Position& inp) {
  moveable_into_positions_.clear();
  Position input = inp;

  auto& tiles = board_.getChessTiles();
  const auto& tile = tiles[input.y][input.x];
  const auto& rules = tile.movement;
  const Position original = input;
  int i;

  if (rules.first_move_forward) { // PAWN

    if (tile.white) { // WHITE PAWN
      i = rules.first_move_forward;
      do {
        input = sumPositions(input, directions[3]);
        if (check(input)) moveable_into_positions_.push_back(input);
        else break;
        i--;
      } while (original.y == 1 && i);
      if (!i) { // PAWN MADE TWO TILE MOVE
        tiles[input.y - 1][input.x].tile_type.en_passantable_tile = true;
      }
      input = original;
      Position lowerleft_of_input = sumPositions(input, directions[6]);
      if (checkBounds(lowerleft_of_input) && tiles[lowerleft_of_input.y][lowerleft_of_input.x].white != tile.white && (tiles[lowerleft_of_input.y][lowerleft_of_input.x].tile_type.is_occupied || tiles[lowerleft_of_input.y][lowerleft_of_input.x].tile_type.en_passantable_tile)) capturable_positions_.push_back(lowerleft_of_input);
      Position lowerright_of_input = sumPositions(input, directions[7]);
      if (checkBounds(lowerright_of_input) && tiles[lowerright_of_input.y][lowerright_of_input.x].white != tile.white && (tiles[lowerright_of_input.y][lowerright_of_input.x].tile_type.is_occupied || tiles[lowerright_of_input.y][lowerright_of_input.x].tile_type.en_passantable_tile)) capturable_positions_.push_back(lowerright_of_input);
    } else { // BLACK PAWN
      i = rules.first_move_forward;
      do {
        input = sumPositions(input, directions[2]);
        if (check(input)) moveable_into_positions_.push_back(input);
        else break;
        i--;
      } while (original.y == 6 && i);
      if (!i) { // PAWN MADE TWO TILE MOVE
        tiles[input.y + 1][input.x].tile_type.en_passantable_tile = true;
      }
      input = original;
      Position upperleft_of_input = sumPositions(input, directions[5]);
      if (checkBounds(upperleft_of_input) && tiles[upperleft_of_input.y][upperleft_of_input.x].white != tile.white && (tiles[upperleft_of_input.y][upperleft_of_input.x].tile_type.is_occupied || tiles[upperleft_of_input.y][upperleft_of_input.x].tile_type.en_passantable_tile)) capturable_positions_.push_back(upperleft_of_input);
      Position upperright_of_input = sumPositions(input, directions[4]);
      if (checkBounds(upperright_of_input) && tiles[upperright_of_input.y][upperright_of_input.x].white != tile.white && (tiles[upperright_of_input.y][upperright_of_input.x].tile_type.is_occupied || tiles[upperright_of_input.y][upperright_of_input.x].tile_type.en_passantable_tile)) capturable_positions_.push_back(upperright_of_input);
    }
  } else { // NOT PAWN
    if (rules.forward) { // CHECK VERTICAL
      const Position& up = directions[2];
      i = rules.forward;
      for (i = rules.forward, input = sumPositions(input, up); checkBounds(input) && check(input) && i; input = sumPositions(input, up), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;

      const Position& down = directions[3];
      i = rules.forward;
      for (i = rules.forward, input = sumPositions(input, down); checkBounds(input) && check(input) && i; input = sumPositions(input, down), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
    }
    if (rules.sideways) { // CHECK HORIZONTAL
      const Position& right = directions[0];
      for (i = rules.sideways, input = sumPositions(input, right); checkBounds(input) && check(input) && i; input = sumPositions(input, right), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;

      const Position& left = directions[1];
      for (i = rules.sideways, input = sumPositions(input, left); checkBounds(input) && check(input) && i; input = sumPositions(input, left), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
      input = original;
    }
    if (rules.diagonal) {
      const Position& upperright = directions[4];
      input = sumPositions(input, upperright);
      for (i = rules.diagonal; checkBounds(input) && check(input) && i; input = sumPositions(input, upperright), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) {
          capturable_positions_.push_back(input);
      }
      input = original;

      const Position& upperleft = directions[5];
      input = sumPositions(input, upperleft);
      for (i = rules.diagonal; checkBounds(input) && check(input) && i; input = sumPositions(input, upperleft), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) {
          capturable_positions_.push_back(input);
      }
      input = original;

      const Position& lowerleft = directions[6];
      input = sumPositions(input, lowerleft);
      for (i = rules.diagonal; checkBounds(input) && check(input) && i; input = sumPositions(input, lowerleft), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) {
          capturable_positions_.push_back(input);
      }
      input = original;

      const Position& lowerright = directions[7];
      input = sumPositions(input, lowerright);
      for (i = rules.diagonal; checkBounds(input) && check(input) && i; input = sumPositions(input, lowerright), i--) {
          moveable_into_positions_.push_back(input);
      }
      if (checkBounds(input) && tiles[input.y][input.x].white != tile.white) {
          capturable_positions_.push_back(input);
      }
      input = original;
    }
    if (rules.l_shape) { // KNIGHT
      for (const auto& direction : l_shape_directions) {
        input = sumPositions(input, direction);
        if (!checkBounds(input)) {
          input = original;
          continue;
        }
        if (check(input)) {
          moveable_into_positions_.push_back(input);
        } else if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
        input = original;
      }
    }
  }
  transferToBoard();
}


bool MoveValidator::check(Position& selection) {
  if (!checkBounds(selection)) return false;
  const auto& tiles = board_.getChessTiles();
  const auto& tile = tiles[selection.y][selection.x];
  if (!tile.tile_type.is_occupied) return true;
  return false;
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
    tiles[position.y][position.x].tile_type.normal_tile = false;
  }
  for (const auto& position : capturable_positions_) {
    tiles[position.y][position.x].tile_type.capturable_tile = true;
    tiles[position.y][position.x].tile_type.normal_tile = false;
  }
}

void MoveValidator::clearPositions() {
  moveable_into_positions_.clear();
  capturable_positions_.clear();
  transferToBoard();
}

bool MoveValidator::isVectorsEmpty() {
  if (moveable_into_positions_.empty() && capturable_positions_.empty()) return true;
  return false;
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

bool MoveValidator::checkBounds(Position& pos) {
  if (0 > pos.x || pos.x >= rowcol_ || 0 > pos.y || pos.y >= rowcol_) return false;
  return true;
}

bool MoveValidator::isInCheck(bool white) {
  // if white = 1 check for whites king else blacks

  auto& tiles = board_.getChessTiles();
  for (const auto& row : tiles) {
    for (const auto& tile : row) {
      if (tile.white == white || !tile.tile_type.is_occupied) continue;
      setPositions(tile.position);
      for (const auto& pos : capturable_positions_) {
        if (tiles[pos.y][pos.x].piece_type == "king") {
          tiles[pos.y][pos.x].tile_type.check = true;
          tiles[pos.y][pos.x].tile_type.normal_tile = false;
          return true;
        }
      }
      clearPositions();
    }
  }
  return false;
}

bool MoveValidator::noKing(bool white) {
  auto& tiles = board_.getChessTiles();
  for (const auto& row : tiles) {
    for (const auto& tile : row) {
      if (tile.white == white || !tile.tile_type.is_occupied) continue;
      if (tile.piece_type == "king") return false;
    }
  }
  return true;
}