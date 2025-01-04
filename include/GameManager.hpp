#pragma once

#include "ConfigReader.hpp"
#include "ChessBoard.hpp"
#include "MoveValidator.hpp"
#include <iostream>
#include <typeinfo>
#include <algorithm>

class GameManager {
  private:
  
    const int console_width_{40};
    bool terminate_{false};
    bool whites_turn_{true};
    Position input_;
    Position target_;

    ConfigReader& reader_;

    void printMenu();
    void printPosition(const Position& pos);
    void printMovementRules(const MovementRules& rules);
    void printPieceConfig(const PieceConfig& piece);
    void printPortalConfig(const PortalConfig& portal);
    void printCurrentConfig();
    void printCentered(const std::string& text, int width);
    void printAbout();
    bool parseInput(std::string& input);
    bool parseTarget(std::string& target);

    void waitForInput();

    void startGame();
  public:
    GameManager(ConfigReader& reader);
    void startMenu();
};