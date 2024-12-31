#pragma once

#include "ConfigReader.hpp"
#include "ChessBoard.hpp"
#include "MoveValidator.hpp"
#include <iostream>

class GameManager {
  private:

    const int console_width_{60};
    bool terminate_{false};
    bool whites_turn_{true};

    ConfigReader& reader_;

    void printMenu();
    void printPosition(const Position& pos);
    void printMovementRules(const MovementRules& rules);
    void printPieceConfig(const PieceConfig& piece);
    void printPortalConfig(const PortalConfig& portal);
    void printCurrentConfig();
    void printCentered(const std::string& text, int width);
    void printAbout();

    void waitForInput();

    void startGame();
  public:
    GameManager(ConfigReader& reader);
    void startMenu();
};