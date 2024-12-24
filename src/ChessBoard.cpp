#include "ChessBoard.hpp"

ChessBoard::ChessBoard(const int& board_size)
    : board_size_(board_size), sqrt_of_board_size_(sqrt(board_size)) {}

void ChessBoard::buildBoard(std::vector<PieceConfig> piece_configs) {
  tiles_.resize(sqrt_of_board_size_);
  for (auto& row : tiles_) {
    row.resize(sqrt_of_board_size_);
  }

  for (const auto& piece : piece_configs) {
    for (const auto& pos : piece.black_positions) {
      tiles_[pos.y][pos.x].white = false;
      tiles_[pos.y][pos.x].position = pos;
      tiles_[pos.y][pos.x].isOccupied = true;
      tiles_[pos.y][pos.x].movement = piece.movement;
      tiles_[pos.y][pos.x].type = piece.type;
    }
    for (const auto& pos : piece.white_positions) {
      tiles_[pos.y][pos.x].white = true;
      tiles_[pos.y][pos.x].position = pos;
      tiles_[pos.y][pos.x].isOccupied = true;
      tiles_[pos.y][pos.x].movement = piece.movement;
      tiles_[pos.y][pos.x].type = piece.type;
    }
  }
}

std::vector<std::vector<ChessTile>> ChessBoard::getChessTiles() const {
  return tiles_;
}

int ChessBoard::getBoardSize() const {
  return board_size_;
}

int ChessBoard::getSqrtOfBoardSize() const {
  return sqrt_of_board_size_;
}

bool ChessBoard::populateBoard(std::vector<PieceConfig> piece_configs) {
  buildBoard(piece_configs);
  return true;
}

void ChessBoard::print() {
  std::vector<std::vector<ChessTile>> tiles = getChessTiles();
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
      if ((i + j) % 2 == 0) { // WHITE TILE
        if (tiles[i][j].isOccupied) {
          std::cout << Color::WHITE_BG << "\033[30m " << tiles[i][j].getPieceRepresentation() << " " << Color::RESET;
        }
        else {
          std::cout << Color::WHITE_BG << "   " << Color::RESET;
        }
      }
      else { // BLACK TILE
        if (tiles[i][j].isOccupied) {
          std::cout << Color::BLACK_BG << " " << tiles[i][j].getPieceRepresentation() << " " << Color::RESET;
        }
        else {
          std::cout << Color::BLACK_BG << "   " << Color::RESET;
        }
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool ChessBoard::move(Position from, Position to) {
  ChessTile piece = tiles_[from.y][from.x];
  if (!piece.isOccupied) { // TILE IS EMPTY
    return false;
  }
  tiles_[from.y][from.x].isOccupied = false;
  tiles_[to.y][to.x] = piece;
  return true;
}