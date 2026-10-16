#include "Calculator.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

bool readNumber(const std::string& prompt, double& value) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        char extra = '\0';
        if (input >> value && !(input >> extra) && std::isfinite(value)) {
            return true;
        }
        std::cout << "Please enter a valid number.\n";
    }
}

bool readOperator(char& operation) {
    while (true) {
        std::cout << "Operator (+, -, *, /, ^): ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }
        if (line.size() == 1 &&
            std::string("+-*/^").find(line[0]) != std::string::npos) {
            operation = line[0];
            return true;
        }
        std::cout << "Please enter one of +, -, *, /, or ^.\n";
    }
}

void showHistory(const Calculator& calculator) {
    const auto& history = calculator.history();
    if (history.empty()) {
        std::cout << "No calculations yet.\n";
        return;
    }

    std::cout << "\nCalculation history:\n";
    for (const Calculation& entry : history) {
        std::cout << entry.left << ' ' << entry.operation << ' '
                  << entry.right << " = " << entry.result << '\n';
    }
}

} // namespace

int main() {
    Calculator calculator;
    std::cout << std::fixed << std::setprecision(6);

    while (true) {
        std::cout << "\nSimple Calculator\n"
                  << "1. Calculate\n"
                  << "2. Show history\n"
                  << "3. Quit\n"
                  << "Choose an option: ";

        std::string choice;
        if (!std::getline(std::cin, choice)) {
            break;
        }

        if (choice == "3") {
            break;
        }
        if (choice == "2") {
            showHistory(calculator);
            continue;
        }
        if (choice != "1") {
            std::cout << "Please choose 1, 2, or 3.\n";
            continue;
        }

        double left = 0.0;
        double right = 0.0;
        char operation = '\0';
        if (!readNumber("First number: ", left) ||
            !readOperator(operation) ||
            !readNumber("Second number: ", right)) {
            break;
        }

        try {
            std::cout << "Result: "
                      << calculator.calculate(left, operation, right) << '\n';
        } catch (const std::exception& error) {
            std::cout << "Calculation error: " << error.what() << '\n';
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
