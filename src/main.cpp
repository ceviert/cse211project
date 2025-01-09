#include <fstream>
#include <iomanip>
#include <iostream>

#include "ConfigReader.hpp"
#include "ChessBoard.hpp"
#include "MoveValidator.hpp"
#include "GameManager.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <config_file>\n";
    return 1;
  }

  // Check if file exists
  std::ifstream file(argv[1]);
  if (!file.good()) {
    std::cerr << "Error: Could not open config file: " << argv[1] << "\n";
    return 1;
  }
  file.close();

  ConfigReader reader(argv[1]);
  if (!reader.readConfig()) {
    std::cerr << "Failed to read configuration file\n";
    return 1;
  }

  /*// Print game settings
  auto settings = reader.getGameSettings();
  std::cout << "\n=== Game Settings ===\n";
  std::cout << "Name: " << settings.name << "\n";
  std::cout << "Board Size: " << settings.board_size << "\n";
  std::cout << "Turn Limit: " << settings.turn_limit << "\n";

  // Print piece configurations
  std::cout << "\n=== Piece Configurations ===\n";
  for (const auto& piece : reader.getPieceConfigs()) {
    printPieceConfig(piece);
  }

  // Print portal configurations
  std::cout << "\n=== Portal Configurations ===\n";
  for (const auto& portal : reader.getPortalConfigs()) {
    printPortalConfig(portal);
  }*/

  // ben yazdım altını

  /*// initialize board
  ChessBoard myBoard(reader.getGameSettings().board_size);

  MoveValidator validator(myBoard);

  myBoard.populateBoard(reader.getPieceConfigs());

  myBoard.print();

  Position from1{0,7};
  Position to1{0,5};

  myBoard.move(from1,to1);

  myBoard.print();

  InputPosition input_position{'a',6};

  myBoard.select(input_position);

  validator.setPositions(input_position);

  myBoard.print();

  myBoard.unselect(input_position);

  validator.clearPositions();

  myBoard.print();

  InputPosition input_position1{'c',7};

  myBoard.select(input_position1);

  validator.setPositions(input_position1);

  myBoard.print();

  myBoard.unselect(input_position1);

  validator.clearPositions();

  myBoard.print();

  Position from2{2,6};
  Position to2{2,2};

  myBoard.move(from2,to2);

  myBoard.print();

  InputPosition input_position2{'c',3};

  myBoard.select(input_position2);

  validator.setPositions(input_position2);

  myBoard.print();

  myBoard.unselect(input_position2);

  validator.clearPositions();

  myBoard.print();

  Position from3{6,7};
  Position to3{5,4};

  myBoard.move(from3,to3);

  myBoard.print();

  InputPosition input_position3{'f',5};

  myBoard.select(input_position3);

  validator.setPositions(input_position3);

  myBoard.print();

  myBoard.unselect(input_position3);

  validator.clearPositions();

  myBoard.print(); */

  GameManager game(reader);

  game.startMenu();

  return 0;
}