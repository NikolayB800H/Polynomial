#pragma once

#include <istream>
#include <ostream>

#include "Abstract.hpp"

class Polynomial;

class Term : public Abstract {
public:
    Term();
    Term(int64_t coef);
    Term(int64_t coef, uint64_t power);
    Term(Term const &other);
    virtual ~Term();
    virtual Term &operator=(Term const &other);
    virtual Term &operator+=(Term const &other);
    virtual Term &operator*=(Term const &other);
    virtual Abstract &&operator+(Abstract const &other) const override;
    virtual Abstract &&operator*(Abstract const &other) const override;
    virtual bool operator<(Term const &other) const;
    virtual bool operator<(Polynomial const &polynomial) const;
    virtual inline What what() const override;
    virtual inline int64_t getCoef() const;
    friend std::ostream& operator<<(std::ostream &out, Term const &abstract);
    friend std::istream& operator>>(std::istream &in, Term &abstract);
    friend Polynomial;
    static size_t cnt;
private:
    int64_t coef;
    uint64_t power;
};

std::ostream& operator<<(std::ostream &out, Term const &term);
std::istream& operator>>(std::istream &in, Term &term);
