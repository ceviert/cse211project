#pragma once

#include "ChessBoard.hpp"

/**
 * @brief Array to hold all 8 directions
 */
extern const Position directions[8];

/**
 * @brief Array to hold all 8 L shape movement directions
 */
extern const Position l_shape_directions[8];

/**
 * @brief Class responsible for move validation
 */
class MoveValidator {
  private:
    ChessBoard& board_;
    std::vector<Position> moveable_into_positions_;
    std::vector<Position> capturable_positions_;
    
    /**
     * @brief Check if the tile is empty
     * @param selection Position of the tile to be checked
     * @return True if empty
     */
    bool check(Position selection);

    /**
     * @brief Adds 2 or 3 positions together
     * @param a Position
     * @param b Position
     * @param c Position (optional)
     * @return Result
     */
    Position sumPositions(const Position& a, const Position& b, const Position& c = {0,0});

    /**
     * @brief Transfer moveable into and capturable tiles to ChessBoard class
     */
    void transferToBoard();

  public:
    /**
     * @brief Constructor
     * @param board ChessBoard instance
     */
    MoveValidator(ChessBoard& board);

    /**
     * @brief Validate moveable and capturable positions and fill the respective vectors
     * @param input Position of the piece whose moves will be validated
     */
    void setPositions(Position& input);

    /**
     * @brief Clear moveable into and capturable positions vectors
     */
    void clearPositions();

    /**
     * @brief Check if vectors in MoveValidator are empty
     * @return True if they both are
     */
    bool isVectorsEmpty();
};