#pragma once

enum What {
    TERM,
    POLYNOMIAL
};

class Term;
class Polynomial;

class Abstract {
public:
    //Abstract() = 0;
    //Abstract(Abstract const &other) = 0;
    //virtual ~Abstract() = 0;
    //virtual Abstract &operator=(Abstract const &other) = 0;
    //virtual Abstract &operator+=(Abstract const &other) = 0;
    //virtual Abstract &operator*=(Abstract const &other) = 0;
    virtual Abstract const &&operator+(Abstract const &other) const = 0;
    virtual Abstract const &&operator*(Abstract const &other) const = 0;
    //virtual bool operator<(Abstract const &other) const = 0;
    virtual inline What what() const = 0;
    //operator Term() const;
    //operator Polynomial();
};
