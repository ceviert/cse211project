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