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
    Polynomial(Abstract const &&abstract);
    Polynomial(Polynomial const &other);
    Polynomial(Polynomial const &&other);
    Polynomial(Term const &term);
    Polynomial(Term const &&term);
    virtual ~Polynomial();
    virtual Polynomial &operator=(Abstract const &abstract);
    virtual Polynomial &operator=(Abstract const &&abstract);
    virtual Polynomial &operator=(Polynomial const &polynomial);
    virtual Polynomial &operator=(Polynomial const &&polynomial);
    virtual Polynomial &operator=(Term const &term);
    virtual Polynomial &operator=(Term const &&term);
    virtual Polynomial &operator+=(Abstract const &abstract);
    //virtual Polynomial &operator+=(Abstract const &&abstract);
    virtual Polynomial &operator*=(Abstract const &abstract);
    //virtual Polynomial &operator*=(Abstract const &&abstract);
    virtual Polynomial &operator*=(Polynomial const &polynomial);
    //virtual Polynomial &operator*=(Polynomial const &&polynomial);
    virtual Polynomial &operator*=(Term const &term);
    //virtual Polynomial &operator*=(Term const &&term);
    virtual Abstract const &&operator+(Abstract const &other) const override;
    //virtual Abstract const &&operator+(Abstract const &&other) const;
    virtual Abstract const &&operator*(Abstract const &other) const override;
    //virtual Abstract const &&operator*(Abstract const &&other) const;
    virtual bool operator<(Abstract const &abstract) const;
    virtual bool operator<(Polynomial const &other) const;
    virtual bool operator<(Term const &term) const;
    virtual inline What what() const override;
    static inline bool isLessOrder();
    static inline bool isGreaterOrder();
    static void setLessOrder();
    static void setGreaterOrder();
    friend std::ostream& operator<<(std::ostream &out, Polynomial const &polynomial);
    //friend std::ostream& operator<<(std::ostream &out, Polynomial const &&polynomial);
    friend std::ostream& operator<<(std::ostream &out, Polynomial const *polynomial);
    friend std::istream& operator>>(std::istream &in, Polynomial &polynomial);
    friend Term;
    //static size_t cnt;
private:
    static bool sortedLess;
    mutable bool sortedLessLocal;
    mutable Vector<Term> terms;
    uint64_t power;

    virtual void sortUpdate() const;
};

std::ostream& operator<<(std::ostream &out, Polynomial const &polynomial);
//std::ostream& operator<<(std::ostream &out, Polynomial const &&polynomial);
std::ostream& operator<<(std::ostream &out, Polynomial const *polynomial);
std::istream& operator>>(std::istream &in, Polynomial &polynomial);
