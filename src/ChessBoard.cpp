#include "ChessBoard.hpp"

ChessBoard::ChessBoard(const int& board_size)
    : board_size_(board_size), sqrt_of_board_size_(sqrt(board_size)) {}

void ChessBoard::buildBoard(std::vector<PieceConfig> piece_configs) {
  std::cout << "Board size: " << sqrt_of_board_size_ << "x" << sqrt_of_board_size_ << "\n";
  tiles_.resize(sqrt_of_board_size_, std::vector<ChessTile>(sqrt_of_board_size_));

  for (const auto& piece : piece_configs) {
    for (const auto& pos : piece.black_positions) {
      tiles_[pos.y][pos.x].white = false;
      tiles_[pos.y][pos.x].position = pos;
      tiles_[pos.y][pos.x].tile_type.is_occupied = true;
      tiles_[pos.y][pos.x].movement = piece.movement;
      tiles_[pos.y][pos.x].piece_type = piece.type;
    }
    for (const auto& pos : piece.white_positions) {
      tiles_[pos.y][pos.x].white = true;
      tiles_[pos.y][pos.x].position = pos;
      tiles_[pos.y][pos.x].tile_type.is_occupied = true;
      tiles_[pos.y][pos.x].movement = piece.movement;
      tiles_[pos.y][pos.x].piece_type = piece.type;
    }
  }
}

std::vector<std::vector<ChessTile>>& ChessBoard::getChessTiles() {
  return tiles_;
}

int ChessBoard::getBoardSize() const {
  return board_size_;
}

int ChessBoard::getSqrtOfBoardSize() const {
  return sqrt_of_board_size_;
}

TILE_PROPERTIES getTileProperty(ChessTile tile) {
  const auto& type = tile.tile_type;
  if (type.capturable_tile) return CAPTURABLE;
  else if (type.check) return CHECK;
  else if (type.moveable_into_tile) return MOVEABLE_INTO;
  else if (type.normal_tile) return NORMAL;
  else if (type.selected_tile) return SELECTED;
  else return NORMAL;
}

bool ChessBoard::populateBoard(const std::vector<PieceConfig> piece_configs) {
  buildBoard(piece_configs);
  return true;
}

void ChessBoard::print(const bool whos_turn) {
  const auto& tiles = getChessTiles();
  std::cout << "      ";
  for (int ascii = 65; ascii < getSqrtOfBoardSize()+65; ascii++ ) {
    std::cout << static_cast<char>(ascii) << "  ";
  }
  std::cout << "\n     ";
  for (int i = 0; i < 3*getSqrtOfBoardSize(); i++) {
    std::cout << "-";
  }
  std::cout << std::endl;
  for (int i = 0; i < getSqrtOfBoardSize(); i++) {
    std::cout << std::setw(2) << i+1 << " | ";
    for (int j = 0; j < getSqrtOfBoardSize(); j++) {
      ChessTile tile = tiles[i][j];
      TILE_PROPERTIES prop = getTileProperty(tile);
      bool is_white_tile = (i + j) % 2 == 0;
      if (is_white_tile) { // WHITE TILE
        if (tile.tile_type.is_occupied) { // OCCUPIED
          switch (prop) {
            case NORMAL:
              std::cout << Color::WHITE_BG << "\033[30m " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case MOVEABLE_INTO:
              std::cout << Color::ORANGE_ON_WHITE_BG << "\033[30m " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case SELECTED:
              std::cout << Color::BLUE_BG << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case CAPTURABLE:
              std::cout << Color::RED_BG << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case CHECK:
              std::cout << Color::CHECK_RED << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
          }
        }
        else {
          switch (prop) { // NOT OCCUPIED
            case NORMAL:
              std::cout << Color::WHITE_BG << "   " << Color::RESET;
              break;
            case MOVEABLE_INTO:
              std::cout << Color::ORANGE_ON_WHITE_BG << "   " << Color::RESET;  
              break;
            case SELECTED: // not possible
              std::cout << "?a?";
              break;
            case CAPTURABLE: // not possible (spoiler alert, it IS for en-passant)
              std::cout << Color::RED_BG << "   " << Color::RESET;
              break;
          }
        }
      }
      else { // BLACK TILE
        if (tile.tile_type.is_occupied) { // OCCUPIED
          switch (prop) {
            case NORMAL:
              std::cout << Color::BLACK_BG << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case MOVEABLE_INTO:
              std::cout << Color::ORANGE_ON_BLACK_BG << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case SELECTED:
              std::cout << Color::BLUE_BG << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case CAPTURABLE:
              std::cout << Color::RED_BG << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
            case CHECK:
              std::cout << Color::CHECK_RED << " " << tile.getPieceRepresentation() << " " << Color::RESET;
              break;
          }
        }
        else {
          switch (prop) { // NOT OCCUPIED
            case NORMAL:
              std::cout << Color::BLACK_BG << "   " << Color::RESET;
              break;
            case MOVEABLE_INTO:
              std::cout << Color::ORANGE_ON_BLACK_BG << "   " << Color::RESET;  
              break;
            case SELECTED: // not possible
              std::cout << "?c?" << std::endl;
              break;
            case CAPTURABLE: // not possible (spoiler alert, it IS for en-passant)
              std::cout << Color::RED_BG << "   " << Color::RESET;
              break;
          }
        }
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  if (whos_turn) std::cout << Color::WHITE_BG << "\033[30m WHITES TURN " << Color::RESET;
  else std::cout << Color::BLACK_BG <<" BLACKS TURN " << Color::RESET;
}


void ChessBoard::move(const Position& from, const Position& to) {
  ChessTile& tile_from = tiles_[from.y][from.x];
  ChessTile& tile_to = tiles_[to.y][to.x];

  if (tile_to.tile_type.en_passantable_tile && from.x != to.x) {
    clearTile(to);
    tile_to = tile_from;
    clearTile(from);
    Position ep_pawn = {from.x - (from.x - to.x), from.y};
    clearTile(ep_pawn);
    return;
  }

  clearTile(to);
  tile_to = tile_from;
  clearTile(from);
}

Position convertToPosition(InputPosition input) {
  Position pos;
  char ch = input.x;
  pos.x = static_cast<int>(ch) - 97;
  pos.y = input.y - 1;
  return pos;
}

bool ChessBoard::select(const Position& pos) {
  if (tiles_[pos.y][pos.x].tile_type.is_occupied) { // A PIECE IS SELECTED
    tiles_[pos.y][pos.x].tile_type.selected_tile = true;
    tiles_[pos.y][pos.x].tile_type.normal_tile = false;
    return true;
  }
  else {
    return false;
  }
}

bool ChessBoard::unselect(const Position& pos) {
  if (tiles_[pos.y][pos.x].tile_type.selected_tile) { // PIECE TO UNSELECT IS A PIECE THAT HAS SELECTED BEFORE
    tiles_[pos.y][pos.x].tile_type.selected_tile = false;
    tiles_[pos.y][pos.x].tile_type.normal_tile = true;
    return true;
  }
    return false;
}

void ChessBoard::clearTile(const Position& pos) {
  ChessTile& tile = tiles_[pos.y][pos.x];

  tile.tile_type.is_occupied = false;
  tile.tile_type.capturable_tile = false;
  tile.tile_type.moveable_into_tile = false;
  tile.tile_type.selected_tile = false;
  tile.tile_type.normal_tile = true;
  tile.tile_type.en_passantable_tile = false;
}
