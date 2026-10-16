#pragma once

class Operation {
public:
    virtual ~Operation() = default;

    virtual char symbol() const noexcept = 0;
    virtual double apply(double left, double right) const = 0;
};
