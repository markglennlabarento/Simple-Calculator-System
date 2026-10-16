# Simple Calculator System

A menu-driven console calculator written in C++17. It supports addition,
subtraction, multiplication, division, exponentiation, and an in-memory history
of successful calculations.

## Features

- Arithmetic with `+`, `-`, `*`, `/`, and `^` (power).
- Input validation for menu choices, operator symbols, and finite numbers.
- Division-by-zero and unsupported-operation errors are reported without
  terminating the program.
- Successful calculations can be viewed in the current session's history.
- Arithmetic operations use a polymorphic interface and can be extended through
  the `Calculator` C++ API.

## Requirements

- A C++17 compiler. The examples below use `g++`.
- On Windows, add `g++` to `PATH` or use an MSYS2/MinGW installation.

## Build and run

From the project root in PowerShell:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -I include src\Calculator.cpp src\main.cpp -o calculator.exe
.\calculator.exe
```

### VS Code

The `.vscode` configuration provides a **Build Calculator** task that compiles
both source files into `calculator.exe`. Run it with **Terminal > Run Build
Task**. The **Debug Calculator** launch configuration starts the program in an
external console and builds it before launching.

The debugger configuration expects MSYS2 UCRT64 GDB at
`C:\msys64\ucrt64\bin\gdb.exe`. Update `miDebuggerPath` in
`.vscode/launch.json` if GDB is installed elsewhere.

## Using the calculator

Choose an option from the menu:

1. **Calculate** — enter the first number, an operator, and the second number.
2. **Show history** — display successful calculations from this run.
3. **Quit** — exit the program.

Numbers must be finite values. The console currently accepts only the built-in
operators (`+`, `-`, `*`, `/`, and `^`); unsupported symbols are rejected.

## Project structure

```text
include/
  Calculator.hpp  Calculator API and calculation history type
  Operation.hpp   Abstract operation interface
src/
  Calculator.cpp  Calculator implementation and built-in operations
  main.cpp        Console user interface
.vscode/
  tasks.json      Build task
  launch.json    External-console debugger configuration
  c_cpp_properties.json
  settings.json
```

## Object-oriented design

- **Abstraction:** `Operation` defines the `symbol()` and `apply()` contract.
- **Inheritance and runtime polymorphism:** Built-in operations implement
  `Operation`; `Calculator` invokes them through the interface.
- **Encapsulation:** The operation registry and history are private to
  `Calculator`.
- **Composition and RAII:** `Calculator` owns operations with
  `std::unique_ptr`.
- **Extensibility:** Register another `Operation` implementation without
  changing `Calculator::calculate()`. Duplicate symbols and null registrations
  are rejected.

For example, a client can add remainder support through the API:

```cpp
#include "Calculator.hpp"

#include <cmath>
#include <memory>
#include <stdexcept>

class Remainder final : public Operation {
public:
    char symbol() const noexcept override { return '%'; }

    double apply(double left, double right) const override {
        if (right == 0.0) {
            throw std::domain_error("Remainder by zero is not allowed.");
        }
        return std::fmod(left, right);
    }
};

int main() {
    Calculator calculator;
    calculator.registerOperation(std::make_unique<Remainder>());
    return calculator.calculate(7.0, '%', 4.0) == 3.0 ? 0 : 1;
}
```

This custom operation is available to C++ callers using `Calculator`; adding it
to the interactive console would also require updating the console's operator
input and help text.
