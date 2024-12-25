#pragma once

#include <iostream>

#include "ConfigReader.hpp"
#include "Color.hpp"

struct TileType {
  bool is_occupied{false};
  bool normal_tile{false};
  bool moveable_into_tile{false};
  bool selected_tile{false};
  bool capturable_tile{false};
};

/**
 * @brief Structure to hold tile properties
 */
struct ChessTile {
  TileType tile_type;
  std::string piece_type;
  Position position;
  MovementRules movement;
  bool white;

  /**
   * @brief Get the single char representation of chess pieces
   * @return Character of chess piece type representation
   */
  char getPieceRepresentation() {
    if (white) {
      if (piece_type == "pawn") return 'P';
      else if (piece_type == "rook") return 'R';
      else if (piece_type == "knight") return 'N';
      else if (piece_type == "bishop") return 'B';
      else if (piece_type == "queen") return 'Q';
      else if (piece_type == "king") return 'K';
      else return '?';
    }
    else {
      if (piece_type == "pawn") return 'p';
      else if (piece_type == "rook") return 'r';
      else if (piece_type == "knight") return 'n';
      else if (piece_type == "bishop") return 'b';
      else if (piece_type == "queen") return 'q';
      else if (piece_type == "king") return 'k';
      else return '?';
    }
  }
};

struct PiecePosition {
  char x;
  int y;
};

enum TILE_PROPERTIES {
  NORMAL,
  MOVEABLE_INTO,
  SELECTED,
  CAPTURABLE
};

Position convertToPosition(PiecePosition input);
TILE_PROPERTIES getTileProperty(ChessTile tile);

/**
 * @brief Class responsible for initializing, populating and updating the chess board
 */
class ChessBoard {
  private:
    std::vector<std::vector<ChessTile>> tiles_;
    int board_size_;
    int sqrt_of_board_size_;

    /**
     * @brief Populate function for chess board
     * @param piece_configs Vector of PieceConfig structures
     */
    void buildBoard(std::vector<PieceConfig> piece_configs);
  public:
    /**
     * @brief Constructor
     * @param board_size Size of the board
     */
    ChessBoard(const int& board_size);

    /**
     * @brief Get the populated chess tiles
     * @return 2D vector of ChessTile structure containing tile info
     */
    std::vector<std::vector<ChessTile>> getChessTiles() const;

    /**
     * @brief Get the board size of the chess board
     * @return Integer board size
     */
    int getBoardSize() const;

    /**
     * @brief Get the row or column count of the board
     * @return Integer row or column count
     */
    int getSqrtOfBoardSize() const;

    /**
     * @brief Calls private function buildBoard()
     * @param piece_configs Vector of PieceConfig structures
     * @return True if successful
     */
    bool populateBoard(std::vector<PieceConfig> piece_configs);

    /**
     * @brief Prints the chess board
     */
    void print();

    /**
     * @brief Move the piece
     * @param from The position of the piece to be moved
     * @param to The position piece will be moved to
     * @return True if valid move
     */
    bool move(Position from, Position to);

    bool select(PiecePosition selection);

    bool unselect(PiecePosition selection);
};