#include "Polynomial.hpp"                                       

bool Polynomial::sortedLess = true;

Polynomial::Polynomial() : terms(Term(0, 0)), power(0), sortedLessLocal(true) {
}

Polynomial::Polynomial(int64_t coef) : Polynomial(Term(coef, 0)) {
}

Polynomial::Polynomial(int64_t coef, uint64_t power) : Polynomial(Term(coef, power)) {
}

Polynomial::Polynomial(Abstract const &abstract) : Polynomial() {
    *this = abstract;
}

Polynomial::Polynomial(Abstract const &&abstract) : Polynomial() {
    *this = abstract;
}

Polynomial::Polynomial(Polynomial const &other) : Polynomial() {
    *this = other;
}

Polynomial::Polynomial(Polynomial const &&other) : Polynomial() {
    *this = other;
}

Polynomial::Polynomial(Term const &term) : Polynomial() {
    *this = term;
}

Polynomial::Polynomial(Term const &&term) : Polynomial() {
    *this = term;
}

Polynomial::~Polynomial() {
}

Polynomial &Polynomial::operator=(Abstract const &abstract) {
    if (abstract.what() == TERM) {
        return *this = static_cast<Term const &>(abstract);
    }
    return *this = static_cast<Polynomial const &>(abstract);
}

Polynomial &Polynomial::operator=(Abstract const &&abstract) {
    if (abstract.what() == TERM) {
        return *this = static_cast<Term const &&>(abstract);
    }
    return *this = static_cast<Polynomial const &&>(abstract);
}

Polynomial &Polynomial::operator=(Polynomial const &other) {
    terms = other.terms;
    power = other.power;
    sortUpdate();
    return *this;
}

Polynomial &Polynomial::operator=(Polynomial const &&other) {
    *this = static_cast<Polynomial const &>(other);
    delete &other;
    return *this;
}

Polynomial &Polynomial::operator=(Term const &term) {
    assert(terms.clear() == OK);
    power = 0;
    if (term.power != 0) {
        assert(terms.insertElementSorted(Term(0, 0)) == OK);
    }
    *this += term;
    sortUpdate();
    return *this;
}

Polynomial &Polynomial::operator=(Term const &&term) {
    *this = static_cast<Term const &>(term);
    delete &term;
    return *this;
}

Polynomial &Polynomial::operator+=(Abstract const &other) {
    sortUpdate();
    size_t pos = 0;
    if (other.what() == TERM) {
        Term const &reference = static_cast<Term const &>(other);
        if ((pos = terms.find(reference)) != static_cast<size_t>(-1)) {
            terms[pos] += reference;
            if (terms[pos].power != 0 && terms[pos].coef == 0) {
                terms.deleteElement(pos);
            }
        } else if (reference.coef != 0) {
            assert(terms.insertElementSorted(reference) == OK);
            if (reference.power > power) {
                power = reference.power;
            }
        }
        return *this;
    }
    Polynomial const &reference_poly = static_cast<Polynomial const &>(other);
    reference_poly.sortUpdate();
    pos = reference_poly.terms.getSize();
    for (size_t i = 0; i < pos; ++i) {
        *this += reference_poly.terms[i];
    }
    return *this;
}

Polynomial &Polynomial::operator*=(Abstract const &abstract) {
    if (abstract.what() == TERM) {
        return *this *= static_cast<Term const &>(abstract);
    }
    return *this *= static_cast<Polynomial const &>(abstract);
}

Polynomial &Polynomial::operator*=(Polynomial const &polynomial) {
    sortUpdate();
    polynomial.sortUpdate();
    Polynomial tmp;
    for (size_t term_i = terms.getSize() - 1; term_i + 1 > 0; --term_i) {
        tmp += polynomial * terms[term_i];
    }
    return *this = tmp;
}

Polynomial &Polynomial::operator*=(Term const &term) {
    sortUpdate();
    if (term.coef == 0) {
        assert(terms.clear() == OK);
        assert(terms.insertElementSorted(Term(0, 0)) == OK);
        return *this;
    }
    for (size_t term_i = terms.getSize() - 1; term_i + 1 > 0; --term_i) {
        terms[term_i] *= term;
    }
    if (terms[0].power > 0) {
        assert(terms.insertElementSorted(Term(0, 0)) == OK);
    }
    return *this;
}

Abstract const &&Polynomial::operator+(Abstract const &other) const {
    sortUpdate();
    Polynomial *ans_poly = new(std::nothrow) Polynomial(*this);
    assert(ans_poly);
    *ans_poly += other;
    if (ans_poly->terms.getSize() == 1) {
        Term *ans = new(std::nothrow) Term(ans_poly->terms[0]);
        assert(ans);
        delete ans_poly;
        return std::move(*ans);
    }
    return std::move(*ans_poly);
}

Abstract const &&Polynomial::operator*(Abstract const &other) const {
    sortUpdate();
    Polynomial *tmp = new(std::nothrow) Polynomial(*this);
    assert(tmp);
    return std::move(*tmp *= other);
}

bool Polynomial::operator<(Abstract const &abstract) const {
    return *this < ((abstract.what() == TERM)
        ? (static_cast<Term const &>(abstract))
        : (static_cast<Polynomial const &>(abstract)));
}

bool Polynomial::operator<(Polynomial const &other) const {
    return sortedLess ? (other.power < power) : (power < other.power);
}

bool Polynomial::operator<(Term const &term) const {
    return sortedLess ? (term.power < power) : (power < term.power);
}

inline What Polynomial::what() const {
    return POLYNOMIAL;
}

inline bool Polynomial::isLessOrder() {
    return sortedLess;
}

inline bool Polynomial::isGreaterOrder() {
    return !sortedLess;
}

void Polynomial::setLessOrder() {
    sortedLess = true;
    Term::sortedLess = true;
}

void Polynomial::setGreaterOrder() {
    sortedLess = false;
    Term::sortedLess = false;
}

std::ostream& operator<<(std::ostream &out, Polynomial const &polynomial) {
    polynomial.sortUpdate();
    size_t end = polynomial.terms.getSize() - 1;
    size_t begin = 0;
    if (end > 0 && (Polynomial::sortedLess && polynomial.terms[end].getCoef() == 0)) {
        --end;
    }
    if (end > 0 && (!Polynomial::sortedLess && polynomial.terms[begin].getCoef() == 0)) {
        ++begin;
    }
    out << "[" << end + 1 - begin << "]: ";
    for (size_t i = begin; i <= end; ++i) {
        if (i && polynomial.terms[i].getCoef() > 0) {
            out << '+';
        }
        out << polynomial.terms[i];
    }
    return out;
}

std::ostream& operator<<(std::ostream &out, Polynomial const *polynomial) {
    if (polynomial == nullptr) {
        out << "(Ptr to object<Polynomial> at NULL)";
        return out;
    }
    out << "(Ptr to object<Polynomial> at " << static_cast<void const *>(polynomial) << ") {\n\t" << *polynomial << "\n}";
    return out;
}

std::istream& operator>>(std::istream &in, Polynomial &polynomial) {
    assert(polynomial.terms.clear() == OK);
    assert(polynomial.terms.insertElementSorted(Term(0, 0)) == OK);
    uint64_t power = 0;
    Term tmp;
    char chr = 0;
    size_t size = 0;
    bool is_open_sz = false;
    while (!std::isdigit(in.peek())) {
        in.get(chr);
        if (chr == '[' && !is_open_sz) {
            is_open_sz = true;
        } else if (!std::isspace(chr)) {
            assert(false);
            //in.setstate(std::ios_base::failbit);
            //return in;
        }
    }
    if (!is_open_sz) {
        assert(false);
        //in.setstate(std::ios_base::failbit);
        //return in;
    }
    in >> size;
    if (in.fail() || size == 0) {
        assert(false);
        //in.setstate(std::ios_base::failbit);
        //return in;
    }
    bool is_closed_sz = false;
    bool is_start_terms = false;
    while (std::isspace(chr = in.peek()) || (chr == ']' || chr == ':')) {
        in.get(chr);
        if (chr == ']' && !is_closed_sz) {
            is_closed_sz = true;
        } else if ((chr == ':' && !is_start_terms) && is_closed_sz) {
            is_start_terms = true;
        } else if (!std::isspace(chr)) {
            assert(false);
            //in.setstate(std::ios_base::failbit);
            //return in;
        }
    }
    if (!is_closed_sz) {
        assert(false);
        //in.setstate(std::ios_base::failbit);
        //return in;
    }
    for (size_t i = 0; i < size; ++i) {
        in >> tmp;
        polynomial += tmp;
    }
    return in;
}

void Polynomial::sortUpdate() const {
    if (sortedLess == sortedLessLocal) {
        return;
    }
    terms.sort();
    sortedLessLocal = sortedLess;
}
