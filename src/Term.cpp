#include <cassert>

#include <iostream>

#include "Polynomial.hpp"
#include "Term.hpp"

size_t Term::cnt = 0;

Term::Term() : coef(0), power(0) {
    std::cout << "+Term " << *this << "[" << cnt << "->" << ++cnt << "]\n";
}

Term::Term(int64_t coef) : coef(coef), power(0) {
    std::cout << "+Term " << *this << "[" << cnt << "->" << ++cnt << "]\n";
}

Term::Term(int64_t coef, uint64_t power) : coef(coef), power(power) {
    std::cout << "+Term " << *this << "[" << cnt << "->" << ++cnt << "]\n";
}

Term::Term(Term const &other) : coef(other.coef), power(other.power) {
    std::cout << "+Term " << *this << "[" << cnt << "->" << ++cnt << "]\n";
}

Term::~Term() {
    std::cout << "-Term " << *this << "[" << cnt << "->" << --cnt << "]\n";
}

Term &Term::operator=(Term const &other) {
    coef = other.coef;
    power = other.power;
    return *this;
}

Term &Term::operator+=(Term const &other) {
    //std::cout << *this << " ? "<< other << std::endl;
    if (power == other.power) {
        coef += other.coef;
        return *this;
    }
    assert(false);
    return *this;
}

Term &Term::operator*=(Term const &other) {
    power += other.power;
    coef *= other.coef;
    return *this;
}

Abstract const &&Term::operator+(Abstract const &other) const {
    if (other.what() == POLYNOMIAL || static_cast<Term const &>(other).power != power) {
        Polynomial *ans_poly = new(std::nothrow) Polynomial(other);
        //std::cout << "test " << *ans_poly << std::endl;
        assert(ans_poly);
        return std::move(*ans_poly += *this);
    }
    Term *ans = new(std::nothrow) Term(*this);
    assert(ans);
    return std::move(*ans += static_cast<Term const &>(other));
}

Abstract const &&Term::operator*(Abstract const &other) const {
    return *this + other;  // Temporary
}

bool Term::operator<(Term const &term) const {
    //std::cout << *this << " < " << term << " = " << (power < term.power) << std::endl;
    return power < term.power;
}

bool Term::operator<(Polynomial const &polynomial) const {
    return power < polynomial.power;
}

inline What Term::what() const {
    return TERM;
}

inline int64_t Term::getCoef() const {
    return coef;
}

std::ostream& operator<<(std::ostream &out, Term const &term) {
    out << term.coef;
    if (term.power) {
        out << "x";
        if (term.power > 1) {
            out << "^" << term.power;
        }
    }
    return out;
}

std::istream& operator>>(std::istream &in, Term &term) {
    char chr = 0;
    //in >> term.coef >> chr >> chr >> term.power;
    return in;
}
