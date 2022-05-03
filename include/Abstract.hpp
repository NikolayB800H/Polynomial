#pragma once

enum What {
    TERM,
    POLYNOMIAL
};

class Abstract {
public:
    virtual Abstract const &&operator+(Abstract const &other) const = 0;
    virtual Abstract const &&operator*(Abstract const &other) const = 0;
    virtual inline What what() const = 0;
};
