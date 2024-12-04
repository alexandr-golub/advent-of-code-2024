#include <utility>
#include <type_traits>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>

std::pair<long, int> parseArg(const std::string& str, long pos, char delimiter) {
    long idx = 0;
    while (std::isdigit(str[pos + idx]) && idx < 3)
        idx++;

    if (idx < 4 && idx > 0 && str[pos + idx] == delimiter)
        return std::make_pair(pos + idx, std::stoi(str.substr(pos, idx)));
        
    return {0, 0};
}

std::vector<std::pair<int, int>> parseArgs(const std::string& str) {
    std::vector<std::pair<int, int>> args;
    long pos = 0;
    
    while ((pos = str.find("mul(", pos)) != std::string::npos) {
        pos += 4;
        const auto [comaPos, firstArg] = parseArg(str, pos, ',');
        
        if (!comaPos)
            continue;

        pos = comaPos + 1;
        const auto [bracePos, secondArg] = parseArg(str, pos, ')');

        if (bracePos) 
            args.emplace_back(firstArg, secondArg);
    }

    return args;
}

std::string removeDisabled(std::string str) {
    long pos = 0;
    while ((pos = str.find("don't()", pos)) != std::string::npos) {      
        const long doPos = str.find("do()", pos + 7);
        const long count = doPos != std::string::npos ? doPos + 4 - pos : std::string::npos;
        str.erase(pos, count);
    }

    return str;
}

int main() {
    const std::string rawData = "TODO";
    
    const std::string& cleanedData = removeDisabled(rawData);
    const auto& args = parseArgs(cleanedData);
    const long result = std::accumulate(args.begin(), args.end(), 0l, [](int x, auto y) { return x + y.first * y.second; });
    std::cout << result;
    return 0;
}
