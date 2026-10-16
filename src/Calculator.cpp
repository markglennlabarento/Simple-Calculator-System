#include "Calculator.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace {

class Addition final : public Operation {
public:
    char symbol() const noexcept override { return '+'; }
    double apply(double left, double right) const override {
        return left + right;
    }
};

class Subtraction final : public Operation {
public:
    char symbol() const noexcept override { return '-'; }
    double apply(double left, double right) const override {
        return left - right;
    }
};

class Multiplication final : public Operation {
public:
    char symbol() const noexcept override { return '*'; }
    double apply(double left, double right) const override {
        return left * right;
    }
};

class Division final : public Operation {
public:
    char symbol() const noexcept override { return '/'; }
    double apply(double left, double right) const override {
        if (right == 0.0) {
            throw std::domain_error("Division by zero is not allowed.");
        }
        return left / right;
    }
};

class Power final : public Operation {
public:
    char symbol() const noexcept override { return '^'; }
    double apply(double left, double right) const override {
        return std::pow(left, right);
    }
};

} // namespace

Calculator::Calculator() {
    registerOperation(std::make_unique<Addition>());
    registerOperation(std::make_unique<Subtraction>());
    registerOperation(std::make_unique<Multiplication>());
    registerOperation(std::make_unique<Division>());
    registerOperation(std::make_unique<Power>());
}

void Calculator::registerOperation(std::unique_ptr<Operation> operation) {
    if (!operation) {
        throw std::invalid_argument("Operation must not be null.");
    }

    const char key = operation->symbol();
    const auto inserted = operations_.emplace(key, std::move(operation));
    if (!inserted.second) {
        throw std::invalid_argument("An operation with this symbol is already registered.");
    }
}

double Calculator::calculate(double left, char operation, double right) {
    const auto found = operations_.find(operation);
    if (found == operations_.end()) {
        throw std::invalid_argument("Unsupported operator.");
    }

    const double result = found->second->apply(left, right);
    history_.push_back({left, operation, right, result});
    return result;
}

const std::vector<Calculation>& Calculator::history() const noexcept {
    return history_;
}
