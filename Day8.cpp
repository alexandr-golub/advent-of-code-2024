#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

using Map = std::vector<std::string>;

struct Position {
    bool isValid(const Map& map, int dx, int dy) const {
        if ((x + dx) < 0 or (y + dy) < 0)
            return false;
        return (x + dx) < map.size()
           and (y + dy) < map[x + dx].size();
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

using UniquePositions = std::unordered_set<Position>;

using Antennas = std::unordered_map<char, std::vector<Position>>;

Map parseData(const std::string& data) {
    Map map;
    std::string line;
    std::istringstream stream(data);
    
    while (std::getline(stream, line)) {
        map.push_back(line);
    }

    return map;
}

Antennas parseAntennas(const Map& map) {
    Antennas antennas;
    for (unsigned i = 0; i < map.size(); i++) {
        for (unsigned j = 0; j < map[i].size(); j++) {
            if (char symbol = map[i][j]; std::isalnum(map[i][j]))
                antennas[symbol].emplace_back(i, j);
        }
    }
    return antennas;
}

UniquePositions getAntinodePositions(const Map& map, const Antennas& antennas) {
    UniquePositions antinodes;
    for (const auto& [symbol, positions] : antennas) {
        for (int i = 0; i < positions.size() - 1; i++) {
            for (int j = i + 1; j < positions.size(); j++) {
                const Position& p1 = positions[i];
                const Position& p2 = positions[j];
                const int dx = p2.x - p1.x;
                const int dy = p2.y - p1.y;

                if(p2.isValid(map, dx, dy))
                    antinodes.emplace(p2.x + dx, p2.y + dy);

                if(p1.isValid(map, -dx, -dy))
                    antinodes.emplace(p1.x - dx, p1.y - dy);
            }
        }
    }

    return antinodes;
}

UniquePositions getResonantAntinodePositions(const Map& map, const Antennas& antennas) {
    UniquePositions antinodes;
    for (const auto& [symbol, positions] : antennas) {
        for (int i = 0; i < positions.size() - 1; i++) {
            for (int j = i + 1; j < positions.size(); j++) {
                const Position& p1 = positions[i];
                const Position& p2 = positions[j];
                const int dx = p2.x - p1.x;
                const int dy = p2.y - p1.y;

                Position resonant2 = p2;
                for (int k = 0; resonant2.isValid(map, k * dx, k * dy); k++) {
                    antinodes.emplace(p2.x + k * dx, p2.y + k * dy);
                } 

                Position resonant1 = p1;
                for (int k = 0; resonant1.isValid(map, k * dx, k * dy); k--) {
                    antinodes.emplace(p1.x + k * dx, p1.y + k * dy);
                } 
            }
        }
    }

    return antinodes;
}

int main() {
    const std::string rawData =
R"(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............
)";

    const Map& map = parseData(rawData);
    const Antennas& antennas = parseAntennas(map);

    std::cout << getAntinodePositions(map, antennas).size() << std::endl;
    std::cout << getResonantAntinodePositions(map, antennas).size() << std::endl;

    return 0;
}
