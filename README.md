# ♟️ CSE211 Chess Engine

A terminal-based, two-player chess engine written in **C++20** as a term project for **CSE 211 – Data Structures (2024 FALL)** at Yeditepe University.

The engine features full ANSI-colored board rendering, JSON-driven piece configuration, legal move highlighting, en-passant, check detection, pawn promotion, and a configurable turn limit.

---

## Features

- **ANSI-colored terminal board** — white/black tile backgrounds, highlighted squares for selected pieces (blue), legal moves (orange), captures (red), and check (red king)
- **Full standard chess movement** — pawns, rooks, knights, bishops, queens, kings — each loaded from a JSON config file
- **Legal move validation** — only valid destination squares are highlighted and accepted
- **En-passant** — correctly handled and cleared after each turn
- **Pawn promotion** — player chooses queen, rook, bishop, or knight when a pawn reaches the back rank
- **Check detection** — prints a CHECK banner when a king is threatened
- **Game over on king capture** — game ends with a winner announcement when a king is taken
- **JSON-configurable** — board size, turn limit, piece types, starting positions, and movement rules are all defined in external JSON files
- **Fantasy mode ready** — a second config slot (`fantasy_chess.json`) is available for custom piece setups

---

## Building

### Dependencies
- `g++` (C++20)
- `make`
- `curl` (for auto-downloading [nlohmann/json](https://github.com/nlohmann/json) on first build)

### Build
```bash
make
```

The Makefile will automatically download the `nlohmann/json` header library on first build. No other external dependencies are required.

### Clean
```bash
make clean        # Remove build artifacts (bin/, obj/)
make distclean    # Also remove downloaded dependencies (third_party/)
```

---

## Running

```bash
# Standard chess
make run
# or
./bin/chess_game data/chess_pieces.json

# Fantasy mode
make run_fantasy
# or
./bin/chess_game data/fantasy_chess.json
```

---

## How to Play

### Input Format
Moves are entered as a **column letter + row number** (e.g., `e2`, `a7`, `h1`).

- **Column**: `a` (leftmost) to `h` (rightmost)  
- **Row**: `1` (top of board / white's back rank) to `8` (bottom / black's back rank)

### Turn Flow
1. The board is printed with whose turn it is shown below it
2. Type the position of the piece you want to move (e.g., `e2`) and press **Enter**
3. The board re-renders with legal move squares highlighted:
   - 🟦 **Blue** — the selected piece
   - 🟠 **Orange** — squares the piece can move to
   - 🔴 **Red** — enemy pieces that can be captured
4. Type the destination square (e.g., `e4`) and press **Enter**
5. The move is made, then it's the opponent's turn

### Special Actions
| Input | Effect |
|---|---|
| `exit` | Return to the main menu at any time during a game |

### Pawn Promotion
When a pawn reaches the opposite back rank, you are prompted to choose:
```
PAWN PROMOTION! Choose: [q]ueen  [r]ook  [b]ishop  [n]knight
>
```

---

## Project Structure

```
cse211project/
├── Makefile
├── README.md
├── data/
│   ├── chess_pieces.json     # Standard chess configuration
│   └── fantasy_chess.json    # Custom/fantasy configuration
├── include/
│   ├── ChessBoard.hpp        # Board, tile, and position structures
│   ├── ConfigReader.hpp      # JSON config parsing structures and class
│   ├── GameManager.hpp       # Game loop and menu management
│   ├── MoveValidator.hpp     # Move legality checking
│   └── Color.hpp             # ANSI terminal color constants
└── src/
    ├── main.cpp
    ├── ChessBoard.cpp
    ├── ConfigReader.cpp
    ├── GameManager.cpp
    ├── MoveValidator.cpp
    └── Color.cpp
```

---

## JSON Configuration Format

You can create your own game configurations:

```json
{
    "game_settings": {
        "name": "my_game",
        "board_size": 64,
        "turn_limit": 100
    },
    "pieces": [
        {
            "type": "rook",
            "positions": {
                "white": [{"x": 0, "y": 0}],
                "black": [{"x": 0, "y": 7}]
            },
            "movement": {
                "forward": 8,
                "sideways": 8,
                "diagonal": 0,
                "l_shape": false,
                "first_move_forward": 0,
                "diagonal_capture": 0
            },
            "count": 1
        }
    ]
}
```

### Movement Rule Fields
| Field | Description |
|---|---|
| `forward` | Max squares vertically (use `8` for unlimited) |
| `sideways` | Max squares horizontally |
| `diagonal` | Max squares diagonally |
| `l_shape` | `true` for knight-style L-shaped jumps |
| `first_move_forward` | Extra squares allowed on first move (pawn: `2`) |
| `diagonal_capture` | Non-zero enables diagonal capture (pawn logic) |

### Coordinate System
- `x`: column, `0` = column A ... `7` = column H
- `y`: row, `0` = row 1 (top) ... `7` = row 8 (bottom)
- `board_size`: total cell count (e.g., `64` for an 8×8 board)

---

## Known Limitations

- Rows 1–9 only (input parsing handles single-digit row numbers)
- No castling
- No forced-check enforcement (a player can ignore check; the game ends on king capture)
- No AI opponent — two-player only

---

## Course Info

| | |
|---|---|
| **Course** | CSE 211 – Data Structures (2024 FALL) |
| **Instructor** | Prof. Dr. Mert OZKAYA |
| **Assistant** | Batuhan EDGUER |
| **Author** | Ertugrul Cevik — [@ceviert](https://github.com/ceviert) |
