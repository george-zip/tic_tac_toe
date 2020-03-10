#include <iostream>
#include <array>
#include <variant>
#include <string>
#include <cctype>
#include <optional>

enum CellOwner {
    X, O, None
};

std::ostream& operator<<(std::ostream& os, CellOwner c) {
    if(c == X) {
        os << "X";
    } else if (c == O) {
        os << "O";
    } else {
        os << " ";
    }
    return os;
}

struct Board {
    using Row = std::array<CellOwner, 3>;
    std::array<Row, 3> grid;
    Board()
            : grid{{{None, None, None},
                    {None, None, None},
                    {None, None, None}}} {}
};

struct Quit {};

struct Move {
    int row, col;
};

std::istream& operator>>(std::istream& input, Move& m) {
    input >> std::skipws >> m.row >> m.col;
    return input;
}

using GameChoice = std::variant<Move, Quit>;

void printBoard(const Board &board);

using Result = std::optional<CellOwner>;

Result getResult(const Board &board);

GameChoice getNextChoice();

void applyMove(Board &board, const Move &move, CellOwner c);

void printHeader();

int main() {
    printHeader();
    CellOwner currentTurn = X;
    Board board;
    while(true) {
        printBoard(board);
        auto next = getNextChoice();
        if(!std::holds_alternative<Quit>(next)) {
            Move m = std::get<Move>(next);
            applyMove(board, m, currentTurn);
            auto winner = getResult(board);
            if(winner) {
                if(winner == X || winner == O) {
                    std::cout << *winner << " won\n";
                } else {
                    std::cout << "Stalemate\n";
                }
                break;
            }
            currentTurn = currentTurn == X ? O : X;
        } else {
            break;
        }
    }
    std::cout << "Game over" << std::endl;
    return 0;
}

void printHeader() {
    std::cout << "Lets play tic tac toe. X goes first.\n";
    std::cout << "To move type the column and row with a space in between.\n";
    std::cout << "For example the leftmost middle square type is '0 1'.\n";
}

void applyMove(Board &board, const Move &move, CellOwner c) {
    board.grid.at(move.col).at(move.row) = c;
}

GameChoice getNextChoice() {
    std::cout << "Input your move or (q)uit: ";
    std::cin >> std::ws;
    auto c = std::cin.peek();
    if(toupper(c) == 'Q') {
        return Quit{};
    }
    else {
        // TODO: Validate that input is within range
        //  and that the space isn't occupied
        Move m;
        std::cin >> m;
        return m;
    }
}

Result getResult(const Board &board) {
    bool all_cells_occupied = true;
    for(int col = 0; col < board.grid.size(); ++col) {
        if(board.grid.at(col).at(0) == board.grid.at(col).at(1) &&
           board.grid.at(col).at(0) == board.grid.at(col).at(2) &&
           board.grid.at(col).at(0) != None) {
            return board.grid.at(col).at(0);
        }
        if(board.grid.at(0).at(col) == board.grid.at(1).at(col) &&
           board.grid.at(0).at(col) == board.grid.at(2).at(col) &&
           board.grid.at(0).at(col) != None) {
            return board.grid.at(0).at(col);
        }
        for(int row = 0; row < board.grid.size(); ++row) {
            if(board.grid.at(col).at(row) == None) {
                all_cells_occupied = false;
            }
        }
    }
    if(all_cells_occupied) {
        return None;
    }
    if(board.grid.at(0).at(0) == board.grid.at(1).at(1) &&
       board.grid.at(0).at(0) == board.grid.at(2).at(2) &&
       board.grid.at(0).at(0) != None) {
        return board.grid.at(0).at(0);
    }
    if(board.grid.at(0).at(2) == board.grid.at(1).at(1) &&
       board.grid.at(0).at(2) == board.grid.at(2).at(0) &&
       board.grid.at(0).at(2) != None) {
        return board.grid.at(0).at(2);
    }
    return {};
}

void printBoard(const Board &board) {
    std::cout << "\n";
    for(auto i = 0; i < board.grid.size(); ++i) {
        auto row = board.grid[i];
        for (int j = 0; j < row.size(); ++j) {
            std::cout << " " << row[j] << " ";
            if(j + 1 < row.size())
                std::cout << "|";
        }
        if(i + 1 < board.grid.size()) {
            std::cout << "\n-----------\n";
        }
    }
    std::cout << std::endl;
}
