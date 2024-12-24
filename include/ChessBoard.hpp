#pragma once

#include <iostream>

#include "ConfigReader.hpp"
#include "Color.hpp"

/**
 * @brief Structure to hold tile properties
 */
struct ChessTile {
  bool isOccupied = false;
  std::string type;
  Position position;
  MovementRules movement;
  bool white;

  /**
   * @brief Get the single char representation of chess pieces
   * @return Character of chess piece type representation
   */
  char getPieceRepresentation() {
    if (white) {
      if (type == "pawn") return 'P';
      else if (type == "rook") return 'R';
      else if (type == "knight") return 'N';
      else if (type == "bishop") return 'B';
      else if (type == "queen") return 'Q';
      else if (type == "king") return 'K';
    }
    else {
      if (type == "pawn") return 'p';
      else if (type == "rook") return 'r';
      else if (type == "knight") return 'n';
      else if (type == "bishop") return 'b';
      else if (type == "queen") return 'q';
      else if (type == "king") return 'k';
    }
  }
};

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
};