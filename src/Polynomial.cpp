#include <iostream>

#include "Polynomial.hpp"

size_t Polynomial::cnt = 0;

Polynomial::Polynomial() : terms(Term(0, 0)), power(0) {
    std::cout << "+Polynomial#0 " << *this << "[" << cnt << "->" << ++cnt << "]" << std::endl;
}

Polynomial::Polynomial(int64_t coef) : terms(Term(coef, 0)), power(0) {
    std::cout << "+Polynomial#1 " << *this << "[" << cnt << "->" << ++cnt << "]" << std::endl;
}

Polynomial::Polynomial(int64_t coef, uint64_t power) : Polynomial(Term(coef, power)) {
}

Polynomial::Polynomial(Abstract const &abstract) : Polynomial() {
    *this = abstract;
}

Polynomial::Polynomial(Polynomial const &other) : terms(other.terms), power(other.power) {
    std::cout << "+Polynomial#2 " << *this << "[" << cnt << "->" << ++cnt << "]" << std::endl;
}

Polynomial::Polynomial(Term const &term) : terms(term), power(term.power) {
    if (power != 0) {
        terms.insertElementSorted(Term(0, 0));
    }
    std::cout << "+Polynomial#3 " << *this << "[" << cnt << "->" << ++cnt << "]" << std::endl;
}

Polynomial::~Polynomial() {
    std::cout << "-Polynomial " << *this << "[" << cnt << "->" << --cnt << "]" << std::endl;
}

Polynomial &Polynomial::operator=(Abstract const &abstract) {
    if (abstract.what() == TERM) {
        return *this = static_cast<Term const &>(abstract);
    }
    return *this = static_cast<Polynomial const &>(abstract);
}

Polynomial &Polynomial::operator=(Abstract &&abstract) {
    if (abstract.what() == TERM) {
        return *this = static_cast<Term &&>(abstract);
    }
    return *this = static_cast<Polynomial &&>(abstract);
}

Polynomial &Polynomial::operator=(Polynomial const &other) {
    terms = other.terms;
    power = other.power;
    return *this;
}

Polynomial &Polynomial::operator=(Polynomial &&other) {
    *this = static_cast<Polynomial const &>(other);
    delete &other;
    return *this;
}

Polynomial &Polynomial::operator=(Term const &term) {
    terms.clear();
    power = 0;
    if (term.power != 0) {
        terms.insertElementSorted(Term(0, 0));
    }
    *this += term;
    return *this;
}

Polynomial &Polynomial::operator=(Term &&term) {
    *this = static_cast<Term const &>(term);
    delete &term;
    return *this;
}

Polynomial &Polynomial::operator+=(Abstract const &other) {
    size_t pos = 0;
    if (other.what() == TERM) {
        Term const &reference = static_cast<Term const &>(other);
        if ((pos = terms.find(reference)) != static_cast<size_t>(-1)) {
            //std::cout << "pos = " << pos << ", but power = " << reference.power << std::endl;
            terms[pos] += reference;
            if (terms[pos].power != 0 && terms[pos].coef == 0) {
                terms.deleteElement(pos);
            }
        } else if (reference.coef != 0) {
            terms.insertElementSorted(reference);
            if (reference.power > power) {
                power = reference.power;
            }
        }
        return *this;
    }
    pos = terms.getSize();
    Polynomial const &reference_poly = static_cast<Polynomial const &>(other);
    for (size_t i = 0; i < pos; ++i) {
        *this += reference_poly.terms[i];
    }
    return *this;
}

Polynomial &Polynomial::operator*=(Abstract const &other) {
    return *this += other;  // Temporary
}

Abstract &&Polynomial::operator+(Abstract const &other) const {
    Polynomial *ans_poly = new(std::nothrow) Polynomial(*this);
    assert(ans_poly);
    *ans_poly += other;
    if (ans_poly->terms.getSize() == 1) {
        Term *ans = new(std::nothrow) Term(ans_poly->terms[0]);
        assert(ans);
        delete ans_poly;
        return std::move(*ans);
    }
    std::cout << sizeof(*ans_poly) << std::endl;
    return std::move(*ans_poly);
}

Abstract &&Polynomial::operator*(Abstract const &other) const {
    return *this + other;  // Temporary
}

bool Polynomial::operator<(Polynomial const &other) const {
    return power < other.power;
}

bool Polynomial::operator<(Term const &term) const {
    return power < term.power;
}

inline What Polynomial::what() const {
    return POLYNOMIAL;
}

std::ostream& operator<<(std::ostream &out, Polynomial const &polynomial) {
    size_t size = polynomial.terms.getSize();
    out << "(" << size << "): ";
    for (size_t i = 0; i < size; ++i) {
        if (i && polynomial.terms[i].getCoef() >= 0) {
            out << '+';
        }
        out << polynomial.terms[i];
    }
    return out;
}

std::istream& operator>>(std::istream &in, Polynomial &polynomial) {
    assert(polynomial.terms.clear() == OK);
    uint64_t power = 0;
    Term tmp;
    char chr = 0;
    size_t size = 0;
    //in >> chr >> size >> chr;
    for (size_t i = 0; i < size; ++i) {
        in >> tmp;
        polynomial += tmp;
    }
    return in;
}
