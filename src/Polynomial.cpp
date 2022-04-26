//#include <iostream>

#include "Polynomial.hpp"                                       

//size_t Polynomial::cnt = 0;
bool Polynomial::sortedLess = true;

Polynomial::Polynomial() : terms(Term(0, 0)), power(0), sortedLessLocal(true) {
    //std::cout << "+Polynomial#0 " << this << " [" << cnt << "->" << ++cnt << "]" << std::endl;
}

Polynomial::Polynomial(int64_t coef) : Polynomial(Term(coef, 0)) {
    //std::cout << "+Polynomial#1 " << this << " [" << cnt << "->" << ++cnt << "]" << std::endl;
}

Polynomial::Polynomial(int64_t coef, uint64_t power) : Polynomial(Term(coef, power)) {
    //std::cout << "(^from coef and power^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::Polynomial(Abstract const &abstract) : Polynomial() {
    *this = abstract;
    //std::cout << "(^from lvalue Abstract^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::Polynomial(Abstract const &&abstract) : Polynomial() {
    //std::cout << "Hard test: " << static_cast<void const *>(this) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    //std::cout << "Hard test: " << (*this = static_cast<Polynomial const &>(abstract)) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    *this = abstract;
    //std::cout << "(^from rvalue Abstract^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::Polynomial(Polynomial const &other) : Polynomial() {
    *this = other;
    //std::cout << "(^from lvalue Polynomial^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::Polynomial(Polynomial const &&other) : Polynomial() {
    *this = other;
    //std::cout << "(^from rvalue Polynomial^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::Polynomial(Term const &term) : Polynomial() {
    *this = term;
    //std::cout << "(^from lvalue Term^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::Polynomial(Term const &&term) : Polynomial() {
    *this = term;
    //std::cout << "(^from rvalue Term^)" << std::endl;
    //std::cout << "At the end: " << this << std::endl;
}

Polynomial::~Polynomial() {
    //std::cout << "-Polynomial " << this << " [" << cnt << "->" << --cnt << "]" << std::endl;
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
        //std::cout << "GOT " << reference << "|";
        if ((pos = terms.find(reference)) != static_cast<size_t>(-1)) {
            //std::cout << "pos = " << pos << ", but power = " << reference.power << std::endl;
            terms[pos] += reference;
            if (terms[pos].power != 0 && terms[pos].coef == 0) {
                terms.deleteElement(pos);
            }
            //std::cout << "A";
        } else if (reference.coef != 0) {
            //terms.insertElementSorted(reference);
            assert(terms.insertElementSorted(reference) == OK);
            if (reference.power > power) {
                power = reference.power;
            }
            //std::cout << "B";
        }
        return *this;
    }
    //std::cout << other << " = ";
    Polynomial const &reference_poly = static_cast<Polynomial const &>(other);
    reference_poly.sortUpdate();
    pos = reference_poly.terms.getSize();
    //std::cout << this << '\n' << &reference_poly << std::endl;
    for (size_t i = 0; i < pos; ++i) {
        *this += reference_poly.terms[i];
        //std::cout << "PLUS " << reference_poly.terms[i];
    }
    return *this;
}

/*Polynomial &Polynomial::operator+=(Abstract const &&abstract) {
    *this += static_cast<Abstract const &>(abstract);
    delete &abstract;
    return *this;
}*/

Polynomial &Polynomial::operator*=(Abstract const &abstract) {
    if (abstract.what() == TERM) {
        //std::cout << "Its term" << std::endl;
        return *this *= static_cast<Term const &>(abstract);
    }
    //std::cout << "Its polynomial" << std::endl;
    return *this *= static_cast<Polynomial const &>(abstract);
}

/*Polynomial &Polynomial::operator*=(Abstract const &&abstract) {
    *this *= static_cast<Abstract const &>(abstract);
    delete &abstract;
    return *this;
}*/

Polynomial &Polynomial::operator*=(Polynomial const &polynomial) {
    sortUpdate();
    polynomial.sortUpdate();
    Polynomial tmp;
    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    for (size_t term_i = terms.getSize() - 1; term_i + 1 > 0; --term_i) {
        //std::cout << tmp << " += ";
        tmp += polynomial * terms[term_i];
        //std::cout << polynomial << " * " << terms[term_i] << " = " << tmp << std::endl;
    }
    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    return *this = tmp;
}

/*Polynomial &Polynomial::operator*=(Polynomial const &&polynomial) {
    *this *= static_cast<Polynomial const &>(polynomial);
    delete &polynomial;
    return *this;
}*/

Polynomial &Polynomial::operator*=(Term const &term) {
    sortUpdate();
    if (term.coef == 0) {
        assert(terms.clear() == OK);
        //terms.insertElementSorted(Term(0, 0));
        assert(terms.insertElementSorted(Term(0, 0)) == OK);
        return *this;
    }
    for (size_t term_i = terms.getSize() - 1; term_i + 1 > 0; --term_i) {
        terms[term_i] *= term;
    }
    if (terms[0].power > 0) {
        //terms.insertElementSorted(Term(0, 0));
        assert(terms.insertElementSorted(Term(0, 0)) == OK);
    }
    return *this;
}

/*Polynomial &Polynomial::operator*=(Term const &&term) {
    *this *= static_cast<Term const &>(term);
    delete &term;
    return *this;
}*/

Abstract const &&Polynomial::operator+(Abstract const &other) const {
    sortUpdate();
    //std::cout << "Hard check: " << static_cast<void const *>(&other) << std::endl;
    Polynomial *ans_poly = new(std::nothrow) Polynomial(*this);
    //std::cout << "Hard check: " << static_cast<void const *>(ans_poly) << std::endl;
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

/*Abstract const &&Polynomial::operator+(Abstract const &other) const {
    Abstract const &&(Polynomial::*ptr)(Abstract const &) const = &Polynomial::operator+;
    std::cout << "Test 2 " << reinterpret_cast<void *>(ptr) << std::endl;
    Polynomial ans_poly(*this);
    ans_poly += other;
    if (ans_poly.terms.getSize() == 1) {
        Term ans(ans_poly.terms[0]);
        return std::move(ans);
    }
    return std::move(ans_poly);
}*/

/*Abstract const &&Polynomial::operator+(Abstract const &&other) const {
    Polynomial *tmp = new(std::nothrow) Polynomial(other);
    assert(tmp);
    return std::move(*tmp += *this);
}*/

Abstract const &&Polynomial::operator*(Abstract const &other) const {
    sortUpdate();
    Polynomial *tmp = new(std::nothrow) Polynomial(*this);
    assert(tmp);
    return std::move(*tmp *= other);
}

/*Abstract const &&Polynomial::operator*(Abstract const &&other) const {
    Polynomial *tmp = new(std::nothrow) Polynomial(other);
    assert(tmp);
    return std::move(*tmp *= *this);
}*/

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
    //std::cout << "TEST " << end << std::endl;
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
        //if (polynomial.terms[i].getCoef() || size == 1) {
        out << polynomial.terms[i];
        //}
    }
    return out;
}

/*std::ostream& operator<<(std::ostream &out, Polynomial const &&polynomial) {
    out << static_cast<Polynomial const &>(polynomial);
    delete &polynomial;
    return out;
}*/

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
    //std::cout << "TEST " << polynomial << std::endl;
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
