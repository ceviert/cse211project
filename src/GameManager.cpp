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
  std::cout << "=================================================================" << std::endl;
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

  std::cout << "=================================================================" << std::endl;

  std::cout << "PRESS ENTER TO CONTINUE...";

  waitForInput();
}

void GameManager::printAbout() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "Made by ERTUGRUL CEVIK (gitHub:@ceviert) as a term project for;\n" << std::endl;
  printCentered("Course Name: CSE 211 - Data Structures (2024FALL)", console_width_);
  printCentered("Instructor: Prof. Dr. Mert OZKAYA", console_width_);
  printCentered("Assistant: Batuhan EDGUER", console_width_);
  std::cout << "=================================================================" << std::endl;

  std::cout << "PRESS ENTER TO CONTINUE...";

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
    std::cout << ">";
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
  if (input.length() != 2) {
    std::cout << "Invalid format. Use column+row (e.g. e2)." << std::endl;
    return false;
  }
  
  char col = input.at(0);
  char row_ch = input.at(1);

  if (col < 'a' || col > 'z' || row_ch < '1' || row_ch > '9') {
    std::cout << "Invalid position. Column must be a letter, row must be 1-9." << std::endl;
    return false;
  }

  int i = static_cast<int>(row_ch) - 48;
  InputPosition position{col, i};
  input_ = convertToPosition(position);
  return true;
}

bool GameManager::parseTarget(std::string& target) {
  if (target.length() != 2) {
    std::cout << "Invalid format. Use column+row (e.g. e4)." << std::endl;
    return false;
  }
  
  char col = target.at(0);
  char row_ch = target.at(1);

  if (col < 'a' || col > 'z' || row_ch < '1' || row_ch > '9') {
    std::cout << "Invalid position. Column must be a letter, row must be 1-9." << std::endl;
    return false;
  }

  int i = static_cast<int>(row_ch) - 48;
  InputPosition position{col, i};
  target_ = convertToPosition(position);
  return true;
}

void GameManager::startGame() {
  ChessBoard board(reader_.getGameSettings().board_size);
  MoveValidator validator(board);
  board.populateBoard(reader_.getPieceConfigs());
  auto& tiles = board.getChessTiles();
  GameSettings settings = reader_.getGameSettings();
  turn_limit_ = settings.turn_limit;
  whites_turn_ = true;

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
  while (turn_limit_) {
    // Clear en-passant flags at the start of each turn (valid for 1 turn only)
    board.clearEnPassantFlags();

    board.print(whites_turn_);

    std::cout << ">";
    std::cin >> input;

    if (input == "exit" || input == "EXIT" || input == "Exit") {
      std::cout << "Returning to main menu..." << std::endl;
      break;
    }

    if (!parseInput(input) ||
          !tiles[input_.y][input_.x].tile_type.is_occupied || 
          tiles[input_.y][input_.x].white != whites_turn_) {
      std::cout << "Invalid selection. Pick your own piece using column+row (e.g. e2)." << std::endl;
      continue;
    }

    validator.setPositions(input_);

    if (validator.isVectorsEmpty()) {
      std::cout << "No legal moves for this piece, select another." << std::endl;
      continue;
    }

    board.select(input_);

    board.print(whites_turn_);
    std::cout << "[target]>";
    std::cin >> target;

    if (!parseTarget(target)) {
      std::cout << "Invalid target. Use column+row (e.g. e4)." << std::endl;
      validator.clearPositions();
      board.unselect(input_);
      continue;
    }

    MOVE_TYPE type = validator.validateMove(target_);

    switch (type) {
      case INVALID:
        std::cout << "That square is not a legal move. Try again." << std::endl;
        board.unselect(input_);
        validator.clearPositions();
        continue;
      case MOVE:
        board.move(input_, target_);
        break;
      case CAPTURE:
        board.move(input_, target_);
        break;
    }
    validator.clearPositions();

    board.unselect(input_);
    board.unselect(target_);

    // --- Pawn Promotion ---
    int back_rank_white = board.getSqrtOfBoardSize() - 1; // y = 7 for white reaching black's side
    int back_rank_black = 0;                              // y = 0 for black reaching white's side
    for (auto& row : tiles) {
      for (auto& tile : row) {
        if (!tile.tile_type.is_occupied || tile.piece_type != "pawn") continue;
        if ((tile.white && tile.position.y == back_rank_white) ||
            (!tile.white && tile.position.y == back_rank_black)) {
          std::string choice;
          std::cout << Color::BLUE_BG << " PAWN PROMOTION! " << Color::RESET
                    << " Choose: [q]ueen  [r]ook  [b]ishop  [n]knight\n>";
          while (true) {
            std::cin >> choice;
            if (choice == "q" || choice == "Q") { tile.piece_type = "queen"; break; }
            else if (choice == "r" || choice == "R") { tile.piece_type = "rook"; break; }
            else if (choice == "b" || choice == "B") { tile.piece_type = "bishop"; break; }
            else if (choice == "n" || choice == "N") { tile.piece_type = "knight"; break; }
            else std::cout << "Invalid choice. Enter q, r, b, or n: >";
          }
          // Copy movement rules from config
          for (const auto& cfg : reader_.getPieceConfigs()) {
            if (cfg.type == tile.piece_type) {
              tile.movement = cfg.movement;
              break;
            }
          }
          std::cout << "Promoted to " << tile.piece_type << "!" << std::endl;
        }
      }
    }

    // --- Check detection ---
    if (validator.isInCheck(whites_turn_) || validator.isInCheck(!whites_turn_)) {
      std::cout << Color::RED_BG << "= = = = = C H E C K = = = = =" << Color::RESET << std::endl;
      check_mode_ = true;
    }

    if (validator.noKing(whites_turn_)) {
        std::cout << Color::RED_BG << "= = = G A M E  O V E R = = =" << Color::RESET
                  << (whites_turn_ ? " White" : " Black") << " wins!" << std::endl;
        board.print(whites_turn_);
        turn_limit_ = 1;
    }

    whites_turn_ = !whites_turn_;
    turn_limit_--;

    /**
     * isInCheck for white and black
     * if is for w/b print CHECK
     * set king check
     * next turn with checkmode = 1
     * movevalidator getkingmovements
     * if no movement checkmate turn_limit = 0
     * call isincheck for every possible movement of king
     * 
     */
  }
}