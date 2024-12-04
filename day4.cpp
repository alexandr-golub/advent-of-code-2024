#include <utility>
#include <type_traits>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <sstream>
#include <array>

using Row = std::string;
using Board = std::vector<Row>;

Board parseBoard(const std::string& str) {
    Board board;
    std::istringstream stream;
    stream.str(str);
 
    std::string line;
    while (std::getline(stream, line)) {
        board.push_back(line);
    }

    return board;
}

bool inBounds(int value, int low, int high) {
    return value >= low and value < high;
}

bool isMAS(const Board& board, int row, int col, int deltaRow, int deltaCol) {
    static std::array<char, 3> MAS = {'M', 'A', 'S'};

    int idx = -1;
    do {
        idx++;
        row += deltaRow;
        col += deltaCol;
    }
    while (inBounds(row, 0, board.size())
        and inBounds(col, 0, board[row].size())
        and idx < MAS.size()
        and board[row][col] == MAS[idx]);

    return idx == 3;
}

long countXMAS(const Board& board) {
    long count = 0;

    for (int row = 0; row < board.size(); row++) {
        for (int col = 0; col < board[row].size(); col++) {
            if (board[row][col] == 'X') {
                count += isMAS(board, row, col, 0, 1);
                count += isMAS(board, row, col, 1, 0);
                count += isMAS(board, row, col, 0, -1);
                count += isMAS(board, row, col, -1, 0);
                count += isMAS(board, row, col, 1, -1);
                count += isMAS(board, row, col, 1, 1);
                count += isMAS(board, row, col, -1, -1);
                count += isMAS(board, row, col, -1, 1);
            }
        }
    }

    return count;
}

bool isDiagonalMAS(const Board& board, int row, int col, int deltaRow, int deltaCol) {
    return board[row + deltaRow][col + deltaCol] == 'M' and board[row - deltaRow][col - deltaCol] == 'S'
        or board[row + deltaRow][col + deltaCol] == 'S' and board[row - deltaRow][col - deltaCol] == 'M';
}

long countX_MAS(const Board& board) {
    long count = 0;

    for (int row = 0; row < board.size(); row++) {
        for (int col = 0; col < board[row].size(); col++) {
            count += board[row][col] == 'A'
                    and inBounds(row, 1, board.size() - 1)
                    and inBounds(col, 1, board[row].size() - 1)
                    and isDiagonalMAS(board, row, col, 1, 1)
                    and isDiagonalMAS(board, row, col, -1, 1);
        }
    }

    return count;
}

int main() {
    const std::string rawData = R"()";
    
    // could be solved without splitting string into rows
    // but having rows simplifies cell indexing
    const Board& board = parseBoard(rawData);
    //std::cout << countXMAS(board);
    std::cout << countX_MAS(board);

    return 0;
}
