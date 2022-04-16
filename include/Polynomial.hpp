#pragma once

#include <istream>
#include <ostream>

#include "Abstract.hpp"
#include "Term.hpp"
#include "Vector.hpp"

class Polynomial : public Abstract {
public:
    Polynomial();
    Polynomial(int64_t coef);
    Polynomial(int64_t coef, uint64_t power);
    Polynomial(Abstract const &abstract);
    Polynomial(Polynomial const &other);
    Polynomial(Term const &term);
    virtual ~Polynomial();
    virtual Polynomial &operator=(Abstract const &abstract);
    virtual Polynomial &operator=(Polynomial const &other);
    virtual Polynomial &operator=(Term const &term);
    virtual Polynomial &operator+=(Abstract const &other);
    virtual Polynomial &operator*=(Abstract const &other);
    virtual Abstract const &&operator+(Abstract const &other) const override;
    virtual Abstract const &&operator*(Abstract const &other) const override;
    virtual bool operator<(Polynomial const &other) const;
    virtual bool operator<(Term const &term) const;
    virtual inline What what() const override;
    friend std::ostream& operator<<(std::ostream &out, Polynomial const &abstract);
    friend std::istream& operator>>(std::istream &in, Polynomial &abstract);
    friend Term;
    static size_t cnt;
private:
    Vector<Term> terms;
    uint64_t power;
};

std::ostream& operator<<(std::ostream &out, Polynomial const &polynomial);
std::istream& operator>>(std::istream &in, Polynomial &polynomial);
