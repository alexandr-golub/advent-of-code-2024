#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <sstream>

using Board = std::vector<std::string>;

struct Direction {
    void turnRight() {
        // rotate 90 degrees counterclockwise
        // (-1; 0) -> (0; 1) -> (1; 0) -> (0; -1)
        Direction newDir{deltaY, -deltaX};
        *this = newDir;
    }

    unsigned toIndex() const {
        // (-1, 0) -> 0
        // (0, -1) -> 1
        // (0, 1) -> 3
        // (1, 0) -> 4
        return (3 * deltaX + deltaY + 3) / 2;
    }
    
    int deltaX{-1}, deltaY{0};
};

struct Position {
    bool isValid(const Board& board, const Direction& dir) const {
        if (x == 0 and dir.deltaX < 0 or y == 0 and dir.deltaY < 0)
            return false;
        return (x + dir.deltaX) < board.size()
           and (y + dir.deltaY) < board[x + dir.deltaX].size();
    }

    void move(const Board& board, const Direction& dir) {
        if (!isValid(board, dir))
            throw std::invalid_argument("Can't move");

        x += dir.deltaX;
        y += dir.deltaY;
    }

    bool isObstacle(const Board& board, const Direction& dir) const {
        if (!isValid(board, dir))
            throw std::invalid_argument("Can't move");

        return '#' == board[x + dir.deltaX][y + dir.deltaY];
    }

    bool operator==(const Position&) const = default;

    unsigned x{0}, y{0};
};

template<>
struct std::hash<Position> {
    std::size_t operator() (const Position &pos) const {
        return std::hash<unsigned>()(pos.x) ^ std::hash<unsigned>()(pos.y);
    }
};

Board parseBoard(const std::string& str) {
    Board board;
    std::string line;
    std::istringstream stream(str);
    
    while (std::getline(stream, line)) {
        board.push_back(line);
    }

    return board;
}

Position parseInitialPosition(const Board& board) {
    for (unsigned i = 0; i < board.size(); i++) {
        for (unsigned j = 0; j < board[i].size(); j++) {
            if ('^' == board[i][j])
                return {i, j};
        }
    }
    
    throw "invalid parseInitialPosition";
}

using Path = std::vector<Position>;

Path buildPath(const Board& board, Position pos) {
    Path path;
    Direction dir;

    while (pos.isValid(board, dir)) {
        if (pos.isObstacle(board, dir))
            dir.turnRight();
        else {
            path.push_back(pos);
            pos.move(board, dir);
        }
    }

    path.push_back(pos);

    return path;
}

using UniquePositions = std::unordered_set<Position>;
UniquePositions getUniquePathPositions(const Path& path) {
    return UniquePositions(path.begin(), path.end());
}

bool isLoop(const Board& board, Position pos, Direction dir) {
    using VisitedDirections = std::array<bool, 4>;
    using VisitedInColumn = std::vector<VisitedDirections>;
    std::vector<VisitedInColumn> visited(board.size(), VisitedInColumn(board.front().size()));

    while (pos.isValid(board, dir)) {
        if (pos.isObstacle(board, dir))
            dir.turnRight();
        else {
            if (visited[pos.x][pos.y][dir.toIndex()])
                return true;

            visited[pos.x][pos.y][dir.toIndex()] = true;
            pos.move(board, dir);
        }
    }

    return false;
}

UniquePositions getUniqueObstaclePositions(Board board, const UniquePositions& pathPositions, Position initialPos) {
    UniquePositions nonInitialPathPositions = pathPositions;
    nonInitialPathPositions.erase(initialPos);

    UniquePositions obstacles;

    for (const Position& obstaclePos : nonInitialPathPositions) {
        const char symbol = board[obstaclePos.x][obstaclePos.y];
            
        board[obstaclePos.x][obstaclePos.y] = '#';

        if (isLoop(board, initialPos, Direction{}))
            obstacles.insert(obstaclePos);

        board[obstaclePos.x][obstaclePos.y] = symbol;
    }

    return obstacles;
}

int main() {
    const std::string rawData = R"()";

    const Board& board = parseBoard(rawData);
    const Position initialPos = parseInitialPosition(board);
    const Path& path = buildPath(board, initialPos);

    const UniquePositions& uniquePos = getUniquePathPositions(path);
    std::cout << uniquePos.size() << std::endl;
    std::cout << getUniqueObstaclePositions(board, uniquePos, initialPos).size() << std::endl;

    return 0;
}
