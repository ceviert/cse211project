#pragma once

#include "ConfigReader.hpp"

struct ChessTile {
  PieceConfig piece;
  Position position;
};


class ChessBoard {
  private:
    std::vector<ChessTile> tiles_;
};