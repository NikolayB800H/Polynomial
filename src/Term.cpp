#include <cassert>

#include "Polynomial.hpp"
#include "Term.hpp"

bool Term::sortedLess = true;

Term::Term() : coef(0), power(0) {
}

Term::Term(int64_t coef) : coef(coef), power(0) {
}

Term::Term(int64_t coef, uint64_t power) : coef(coef), power(power) {
}

Term::Term(Term const &other) : coef(other.coef), power(other.power) {
}

Term::~Term() {
}

Term &Term::operator=(Term const &other) {
    coef = other.coef;
    power = other.power;
    return *this;
}

Term &Term::operator+=(Term const &other) {
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

Abstract &&Term::operator+(Abstract const &other) const {
    if (other.what() == POLYNOMIAL || static_cast<Term const &>(other).power != power) {
        Polynomial *ans_poly = new(std::nothrow) Polynomial(other);
        assert(ans_poly);
        return std::move(*ans_poly += *this);
    }
    Term *ans = new(std::nothrow) Term(*this);
    assert(ans);
    return std::move(*ans += static_cast<Term const &>(other));
}

Abstract &&Term::operator*(Abstract const &abstract) const {
    if (abstract.what() == TERM) {
        return std::move(*this * static_cast<Term const &>(abstract));
    }
    return std::move(*this * static_cast<Polynomial const &>(abstract));
}

Polynomial Term::operator*(Polynomial const &polynomial) const {
    Polynomial tmp(polynomial);
    return tmp *= *this;
}

Term Term::operator*(Term const &term) const {
    Term tmp(term);
    return tmp *= *this;
}

bool Term::operator<(Term const &term) const {
    return sortedLess ? (term.power < power) : (power < term.power);
}

bool Term::operator<(Polynomial const &polynomial) const {
    return sortedLess ? (polynomial.power < power) : (power < polynomial.power);
}

inline What Term::what() const {
    return TERM;
}

inline int64_t Term::getCoef() const {
    return coef;
}

std::ostream& operator<<(std::ostream &out, Term const &term) {
    if (term.coef != 1 || term.power == 0) {
        if (term.coef == -1 && term.power != 0) {
            out << '-';
        } else {
            out << term.coef;
        }
    }
    if (term.power) {
        out << "x";
        if (term.power > 1) {
            out << "^" << term.power;
        }
    }
    return out;
}

std::ostream& operator<<(std::ostream &out, Term const *term) {
    if (term == nullptr) {
        out << "(Ptr to object<Term> at NULL)";
        return out;
    }
    out << "(Ptr to object<Term> at " << static_cast<void const *>(term) << ") {\n\t" << *term << "\n}";
    return out;
}

std::istream& operator>>(std::istream &in, Term &term) {
    char chr = 0;
    bool got_mp = false;
    bool is_positive = true;
    while (!std::isdigit(chr = in.peek()) && chr != 'x') {
        in.get(chr);
        if (chr == '-' && !got_mp) {
            is_positive = false;
            got_mp = true;
        } else if (chr == '+' && !got_mp) {
            is_positive = true;
            got_mp = true;
        } else if (!std::isspace(chr)) {
            assert(false);
            //in.setstate(std::ios_base::failbit);
            //return in;
        }
    }
    if (chr != 'x') {
        in >> term.coef;
        if (in.fail()) {
            term.coef = 1;
        }
    } else {
        term.coef = 1;
    }
    if (!is_positive) {
        term.coef *= -1;
    }
    bool got_x = false;
    bool got_arrow = false;
    while (!std::isdigit(in.peek())) {
        in.get(chr);
        if (chr == 'x' && !got_x) {
            got_x = true;
        } else if (chr == '^' && (!got_arrow && got_x)) {
            got_arrow = true;
        } else if (std::isspace(chr)) {
            if (chr == '\n') {
                break;
            }
        } else {
            break;
        }
    }
    if (got_arrow) {
        if ((!std::isspace(chr) && chr != '^')) {
            assert(false);
            //in.setstate(std::ios_base::failbit);
            //return in;
        }
        in >> term.power;
        if (in.fail()) {
            assert(false);
            //in.setstate(std::ios_base::failbit);
            //return in;
        }
    } else if (got_x) {
        term.power = 1;
    } else {
        term.power = 0;
    }
    return in;
}
