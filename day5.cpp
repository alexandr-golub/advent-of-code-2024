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
#include <set>

using Upgrade = std::vector<int>;
using Rules = std::set<std::pair<int, int>>;

bool isValid(const Upgrade& upgrade, const Rules& rules) {
    for (int i = 0; i < upgrade.size(); i++) {
        for (int j = i + 1; j < upgrade.size(); j++) {
            if (rules.contains({upgrade[j], upgrade[i]}))
                return false;
        }
    }

    return true;
}

int getMedian(const Upgrade& upgrade) {
    const int middle = upgrade.size() / 2;
    return upgrade.at(middle);
}

Upgrade fixUpgrade(Upgrade upgrade, const Rules& rules) {
    std::sort(upgrade.begin(), upgrade.end(), [&rules](int x, int y) {      
        return !rules.contains({y, x});
    });

    return upgrade;
}

int main() {
    Rules rules = {
        // {93, 48},
    };

    std::vector<Upgrade> upgrades = {
        // {77,83,99,62,44,97,74},
    };
    
    // part 1
    std::cout << std::accumulate(upgrades.begin(), upgrades.end(), 0l, [&rules](long acc, const auto& upgrade) {
        return acc + (isValid(upgrade, rules) ? getMedian(upgrade) : 0);
    }) << std::endl;
    
    // part 2
    std::cout << std::accumulate(upgrades.begin(), upgrades.end(), 0l, [&rules](long acc, const auto& upgrade) {
        if (isValid(upgrade, rules))
            return acc;
        
        const Upgrade& fixedUpgrade = fixUpgrade(upgrade, rules);
        return acc + getMedian(fixedUpgrade);
    }) << std::endl;

    return 0;
}
