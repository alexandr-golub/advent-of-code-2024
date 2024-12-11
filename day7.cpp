#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <numeric>
#include <functional>
#include <cstdint>
#include <cmath>

struct Equation;
using Equations = std::vector<Equation>;

struct Equation {
    using Result = std::int64_t;
    using Arg = std::int64_t;
    using Args = std::vector<Arg>;

    Result result;
    Args args;
    static Equation parse(const std::string& str);
    bool isValid() const;

    using Operation = std::function<Arg(Arg, Arg)>;
    void evalFront(Operation op);
    static Arg concat(Arg, Arg);
};

Equation::Arg Equation::concat(Arg x, Arg y) {
    int base = 1;
    while (base <= y) {
        base *= 10;
    }
    return x * base + y;
}

void Equation::evalFront(Operation op) {
    if (args.size() < 2)
        return;

    args[1] = op(args[0], args[1]);
    args.erase(args.begin());
}

bool Equation::isValid() const {
    std::deque<Equation> queue{*this};

    while (!queue.empty()) {
        const Equation& eq = queue.front();
        if (eq.args.size() == 1) {
            if (eq.args.front() == eq.result)
                return true;
            else {
                queue.pop_front();
                continue;
            }
        }
        
        Equation eqPlus = eq;
        eqPlus.evalFront(std::plus());
        queue.push_back(eqPlus);
    
        Equation eqMul = eq;
        eqMul.evalFront(std::multiplies());
        queue.push_back(eqMul);

        Equation eqConcat = eq;
        eqConcat.evalFront(Equation::concat);
        queue.push_back(eqConcat);

        queue.pop_front();
    }

    return false;
}

Equation Equation::parse(const std::string& str) {
    std::istringstream stream(str);

    Equation eq;
    stream >> eq.result;

    char delim;
    stream >> delim;

    Arg arg;
    while (stream >> arg) {
        eq.args.push_back(arg);
    }

    return eq;
}

Equations parseEquations(const std::string& str) {
    Equations eqs;
    
    std::istringstream stream(str);
    std::string line;
    while (std::getline(stream, line)) {
        eqs.push_back(Equation::parse(line));
    }

    return eqs;
}

int main()
{
    const std::string rawData = R"()";    

    const Equations& equations = parseEquations(rawData);
    const long result = std::accumulate(equations.begin(), equations.end(), std::int64_t{0}, [](std::int64_t acc, const Equation& eq) {
        if (!eq.isValid()) {
            return acc;
        }

        return acc + eq.result;
    });

    std::cout << result;

    return 0;
}
