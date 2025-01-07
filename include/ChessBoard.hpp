#pragma once

#include <iostream>

#include "ConfigReader.hpp"
#include "Color.hpp"

/**
 * @brief Structure to hold tile type
 */
struct TileType {
  bool is_occupied{false};
  bool normal_tile{false};
  bool moveable_into_tile{false};
  bool selected_tile{false};
  bool capturable_tile{false};
};

/**
 * @brief Structure to hold chess tile
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

/**
 * @brief Structure to hold input position
 */
struct InputPosition {
  char x;
  int y;
};

/**
 * @brief Tile Properties
 */
enum TILE_PROPERTIES {
  NORMAL,
  MOVEABLE_INTO,
  SELECTED,
  CAPTURABLE
};

/**
 * @brief Converts InputPositon type to Position type
 * @param input Input position to be converted
 * @return Same position in Position type
 */
Position convertToPosition(InputPosition input);

/**
 * @brief Get tile property
 * @param tile The tile which the properties will be extracted
 * @return Property in TILE_PROPERTIES enumarated type
 */
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
    std::vector<std::vector<ChessTile>>& getChessTiles();

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
    void print(bool whos_turn);

    /**
     * @brief Move the piece (temp func does not check movement rules)
     * @param from The position of the piece to be moved
     * @param to The position piece will be moved to
     * @return True if valid move
     */
    bool move(Position from, Position to);

    /**
     * @brief Select the piece
     * @param selection Piece to be selected
     * @return True if there exist a piece at given position
     */
    bool select(Position& pos);

    /**
     * @brief Unselect the piece
     * @param selection Piece to be unselected
     * @return True if given piece was selected before
     */
    bool unselect(Position& pos);

    void clearTile(Position& pos);
};