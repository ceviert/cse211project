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

    std::cout << "Original position: (" << original.x << ", " << original.y << ")" << std::endl;

    if (rules.first_move_forward) { // PAWN
        std::cout << "Pawn detected. White: " << tile.white << std::endl;

        if (tile.white) { // WHITE PAWN
            i = rules.first_move_forward;
            do {
                input = sumPositions(input, directions[3]);
                std::cout << "Checking position: (" << input.x << ", " << input.y << ")" << std::endl;
                if (check(input)) moveable_into_positions_.push_back(input);
                i--;
            } while (original.y == 1 && i);
            input = original;
            Position lowerleft_of_input = sumPositions(input, directions[6]);
            std::cout << "Lower left position: (" << lowerleft_of_input.x << ", " << lowerleft_of_input.y << ")" << std::endl;
            if (tiles[lowerleft_of_input.y][lowerleft_of_input.x].white != tile.white && tiles[lowerleft_of_input.y][lowerleft_of_input.x].tile_type.is_occupied) capturable_positions_.push_back(lowerleft_of_input);
            Position lowerright_of_input = sumPositions(input, directions[7]);
            std::cout << "Lower right position: (" << lowerright_of_input.x << ", " << lowerright_of_input.y << ")" << std::endl;
            if (tiles[lowerright_of_input.y][lowerright_of_input.x].white != tile.white && tiles[lowerright_of_input.y][lowerright_of_input.x].tile_type.is_occupied) capturable_positions_.push_back(lowerright_of_input);
        } else { // BLACK PAWN
            i = rules.first_move_forward;
            do {
                input = sumPositions(input, directions[2]);
                std::cout << "Checking position: (" << input.x << ", " << input.y << ")" << std::endl;
                if (check(input)) moveable_into_positions_.push_back(input);
                i--;
            } while (original.y == 6 && i);
            input = original;
            Position upperleft_of_input = sumPositions(input, directions[5]);
            std::cout << "Upper left position: (" << upperleft_of_input.x << ", " << upperleft_of_input.y << ")" << std::endl;
            if (tiles[upperleft_of_input.y][upperleft_of_input.x].white != tile.white && tiles[upperleft_of_input.y][upperleft_of_input.x].tile_type.is_occupied) capturable_positions_.push_back(upperleft_of_input);
            Position upperright_of_input = sumPositions(input, directions[4]);
            std::cout << "Upper right position: (" << upperright_of_input.x << ", " << upperright_of_input.y << ")" << std::endl;
            if (tiles[upperright_of_input.y][upperright_of_input.x].white != tile.white && tiles[upperright_of_input.y][upperright_of_input.x].tile_type.is_occupied) capturable_positions_.push_back(upperright_of_input);
        }
    } else { // NOT PAWN
        std::cout << "Pawn NOT detected. White: " << tile.white << std::endl;
        if (rules.forward) { // CHECK VERTICAL
          std::cout << "RULES.FORWARD DETECTED." << std::endl;
          const Position& up = directions[2];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, up); check(input) && i && checkBounds(input); input = sumPositions(input, up), i--) {
              std::cout << "Checking upward position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;
          std::cout << "AAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;

          const Position& down = directions[3];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, down); check(input) && i && checkBounds(input); input = sumPositions(input, down), i--) {
              std::cout << "Checking downward position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;
        }
        if (rules.sideways) { // CHECK HORIZONTAL
          std::cout << "RULES.SIDEWAYS DETECTED." << std::endl;
          const Position& right = directions[0];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, right); check(input) && i && checkBounds(input); input = sumPositions(input, right), i--) {
              std::cout << "Checking right position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;

          const Position& left = directions[1];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, left); check(input) && i && checkBounds(input); input = sumPositions(input, left), i--) {
              std::cout << "Checking left position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;
        }
        if (rules.diagonal) {
          std::cout << "RULES.DIAGONAL DETECTED." << std::endl;
          const Position& upperright = directions[4];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, upperright); check(input) && i && checkBounds(input); input = sumPositions(input, upperright), i--) {
              std::cout << "Checking downward position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;

          const Position& upperleft = directions[5];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, upperleft); check(input) && i && checkBounds(input); input = sumPositions(input, upperleft), i--) {
              std::cout << "Checking downward position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;

          const Position& lowerleft = directions[6];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, lowerleft); check(input) && i && checkBounds(input); input = sumPositions(input, lowerleft), i--) {
              std::cout << "Checking downward position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;
          
          const Position& lowerright = directions[7];
          i = rules.forward;
          for (i = rules.forward, input = sumPositions(input, lowerright); check(input) && i && checkBounds(input); input = sumPositions(input, lowerright), i--) {
              std::cout << "Checking downward position: (" << input.x << ", " << input.y << ")" << std::endl;
              moveable_into_positions_.push_back(input);
          }
          if (tiles[input.y][input.x].white != tile.white) capturable_positions_.push_back(input);
          input = original;
        }
        if (rules.l_shape) { // KNIGHT
            for (const auto& direction : l_shape_directions) {
                input = sumPositions(input, direction);
                std::cout << "Checking L-shape position: (" << input.x << ", " << input.y << ")" << std::endl;
                if (!checkBounds(input)) continue;
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