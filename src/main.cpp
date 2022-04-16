#include <iostream>

#include "Abstract.hpp"
#include "Polynomial.hpp"
#include "Term.hpp"

int main(int argc, char const *argv[]) {
    Polynomial p;
    p += Term(-1, 2);
    std::cout << p << std::endl;
    p += Term(-2, 1);
    std::cout << p << std::endl;
    p += Term(3, 0);
    std::cout << p << std::endl;
    p += Term(0, 3);
    std::cout << p << std::endl;
    Polynomial q(p);
    q = p + q;
    std::cout << q << std::endl;
    /*Term t_power_1(1, 1);
    Term t_power_2(1, 2);
    std::cout << (t_power_1 + t_power_2) << std::endl;
    std::cout << (t_power_1 + p + t_power_1) << std::endl;*/
    return 0;
}
