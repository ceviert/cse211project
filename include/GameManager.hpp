#pragma once

#include "ConfigReader.hpp"
#include "ChessBoard.hpp"
#include "MoveValidator.hpp"
#include "Color.hpp"
#include <iostream>
#include <typeinfo>
#include <algorithm>

/**
 * @brief Class to hold interactions with other classes and gameplay
 */
class GameManager {
  private:
  
    const int console_width_{65};
    bool terminate_{false};
    bool whites_turn_{true};
    bool check_mode_{false};
    Position input_;
    Position target_;
    int turn_limit_;

    ConfigReader& reader_;

    /**
     * @brief Print the menu
     */
    void printMenu();

    /**
     * @brief Print position
     * @param pos Position
     */
    void printPosition(const Position& pos);

    /**
     * @brief Print movement rules
     * @param rules Movement Rules
     */
    void printMovementRules(const MovementRules& rules);
    
    /**
     * @brief Print piece configs
     * @param piece Piece
     */
    void printPieceConfig(const PieceConfig& piece);

    /**
     * @brief Print portal configs
     * @param portal Portal
     */
    void printPortalConfig(const PortalConfig& portal);

    /**
     * @brief Print current config
     */
    void printCurrentConfig();

    /**
     * @brief Print text centered
     * @param text The text to be printed
     * @param width Console width
     */
    void printCentered(const std::string& text, int width);

    /**
     * @brief Print about
     */
    void printAbout();

    /**
     * @brief Parse input
     * @param input Input in string format
     */
    bool parseInput(std::string& input);

    /**
     * @brief Parse target
     * @param target Target in string format
     */
    bool parseTarget(std::string& target);

    /**
     * @brief Wait for an enter
     */
    void waitForInput();

    /**
     * @brief Start the game
     */
    void startGame();

  public:
    /**
     * @brief Constructor
     * @param reader ConfigReader type reader to get the current config
     */
    GameManager(ConfigReader& reader);

    /**
     * @brief start the menu
     */
    void startMenu();
};