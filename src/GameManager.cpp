#include "GameManager.hpp"

GameManager::GameManager(ConfigReader& reader) : reader_(reader) {}

void GameManager::printPosition(const Position& pos) {
  std::cout << "(" << pos.x << "," << pos.y << ")";
}

void GameManager::printMovementRules(const MovementRules& rules) {
  std::cout << "Movement Rules:\n";
  std::cout << "  Forward: " << rules.forward << "\n";
  std::cout << "  Sideways: " << rules.sideways << "\n";
  std::cout << "  Diagonal: " << rules.diagonal << "\n";
  std::cout << "  L-Shape: " << (rules.l_shape ? "true" : "false") << "\n";
  std::cout << "  First Move Forward: " << rules.first_move_forward << "\n";
  std::cout << "  Diagonal Capture: " << rules.diagonal_capture << "\n";
}

void GameManager::printPieceConfig(const PieceConfig& piece) {
  std::cout << "\nPiece Type: " << piece.type << "\n";

  std::cout << "White Positions: ";
  for (const auto& pos : piece.white_positions) {
    printPosition(pos);
    std::cout << " ";
  }
  std::cout << "\n";

  std::cout << "Black Positions: ";
  for (const auto& pos : piece.black_positions) {
    printPosition(pos);
    std::cout << " ";
  }
  std::cout << "\n";

  printMovementRules(piece.movement);
  std::cout << "Count: " << piece.count << "\n";
}

void GameManager::printPortalConfig(const PortalConfig& portal) {
  std::cout << "\nPortal ID: " << portal.id << "\n";
  std::cout << "Entry: ";
  printPosition(portal.positions.entry);
  std::cout << "\nExit: ";
  printPosition(portal.positions.exit);

  std::cout << "\nProperties:\n";
  std::cout << "  Preserve Direction: "
            << (portal.properties.preserve_direction ? "true" : "false")
            << "\n";
  std::cout << "  Allowed Colors: ";
  for (const auto& color : portal.properties.allowed_colors) {
    std::cout << color << " ";
  }
  std::cout << "\n  Cooldown: " << portal.properties.cooldown << "\n";
}

void GameManager::printCentered(const std::string& text, int width) {
  int padding = (width - text.size()) / 2;
  if (padding > 0) {
    std::cout << std::string(padding, ' ') << text << std::endl;
  } else {
    std::cout << text << std::endl;
  }
}

void GameManager::printMenu() {
  printCentered("1 - START THE GAME", console_width_);
  printCentered("2 - CURRENT CONFIG", console_width_);
  printCentered("3 - ABOUT", console_width_);
  printCentered("4 - EXIT", console_width_);
}

void GameManager::printCurrentConfig() {
  auto settings = reader_.getGameSettings();
  std::cout << "\n=== Game Settings ===\n";
  std::cout << "Name: " << settings.name << "\n";
  std::cout << "Board Size: " << settings.board_size << "\n";
  std::cout << "Turn Limit: " << settings.turn_limit << "\n";

  std::cout << "\n=== Piece Configurations ===\n";
  for (const auto& piece : reader_.getPieceConfigs()) {
    printPieceConfig(piece);
  }

  std::cout << "\n=== Portal Configurations ===\n";
  for (const auto& portal : reader_.getPortalConfigs()) {
    printPortalConfig(portal);
  }

  waitForInput();
}

void GameManager::printAbout() {
  std::cout << "Made by ERTUGRUL CEVIK (gitHub:@ceviert) as a term project for;\n" << std::endl;
  printCentered("Course Name: CSE 211 - Data Structures (2024FALL)", console_width_);
  printCentered("Instructor: Prof. Dr. Mert OZKAYA", console_width_);
  printCentered("Assistant: Batuhan EDGUER", console_width_);

  waitForInput();
}

void GameManager::waitForInput() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
}

void GameManager::startMenu() {
  printCentered("WELCOME TO CHESS", console_width_);
  std::cout << "\n" << std::endl;
  int choice;
  while (!terminate_) {
    printMenu();
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }

    switch (choice) {
      case 1:
        std::cout << "Starting the game...\nGame started, type exit to quit any time." << std::endl;
        startGame();
        break;
      case 2:
        printCurrentConfig();
        break;
      case 3:
        printAbout();
        break;
      case 4:
        std::cout << "EXITING THE GAME..." << std::endl;
        terminate_ = true;
        break;
      default:
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
  }
}

bool GameManager::parseInput(std::string& input) {
  if (input.length() != 2) return false;
  else if (typeid(input.at(0)) != typeid(char) || typeid(input.at(1)) != typeid(int)) return false;

  InputPosition position{input.at(0), input.at(1)};

  input_ = convertToPosition(position);
  return;
}

bool GameManager::parseTarget(std::string& target) {
  if (target.length() != 2) return false;
  else if (typeid(target.at(0)) != typeid(char) || typeid(target.at(1)) != typeid(int)) return false;

  InputPosition position{target.at(0), target.at(1)};

  target_ = convertToPosition(position);
  return;
}

void GameManager::startGame() {
  ChessBoard board(reader_.getGameSettings().board_size);
  MoveValidator validator(board);
  board.populateBoard(reader_.getPieceConfigs());
  auto& tiles = board.getChessTiles();

  /**
   * loop
   * * print
   * * get selection
   * * select piece
   * * if (no piece || wrong color) continue
   * * set positions
   * * if (positions.empty) continue
   * * print with moveable and capturable positions
   * * get target tile
   * * if (tile not valid) continue
   * * make the move
   * * if (move == capturing) clear respective tile
   * * flip whites_turn_
   * * decrement (?) turn limit
   */

  std::string input;
  std::string target;
  while (true) {
    board.print(whites_turn_);

    std::cout << ">";
    std::cin >> input;

    if (input == "exit" || input == "EXIT" || input == "Exit") {
      std::cout << "Returning to main menu..." << std::endl;
      break;
    }

    if (!parseInput(input) ||
          !tiles[input_.y][input_.y].tile_type.is_occupied || 
          tiles[input_.y][input_.x].white != whites_turn_ ) {
      std::cout << "Invalid input, please try again" << std::endl;
      continue;
    }

    validator.setPositions(input_);

    if (validator.isVectorsEmpty()) {
      std::cout << "No moves for this piece, select another" << std::endl;
      continue;
    }

    board.print(whites_turn_);
    std::cout << "[target]>";
    std::cin >> target;

    if (!parseTarget(target) ||
          !std::find(validator.moveable_into_positions_.begin(), validator.moveable_into_positions_.end(), target_))
  }
}