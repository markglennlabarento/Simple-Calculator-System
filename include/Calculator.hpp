#pragma once

#include "Operation.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

struct Calculation {
    double left;
    char operation;
    double right;
    double result;
};

class Calculator {
public:
    Calculator();

    void registerOperation(std::unique_ptr<Operation> operation);
    double calculate(double left, char operation, double right);
    const std::vector<Calculation>& history() const noexcept;

private:
    std::unordered_map<char, std::unique_ptr<Operation>> operations_;
    std::vector<Calculation> history_;
};
