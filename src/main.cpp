#include <iostream>

#include "Abstract.hpp"
#include "Polynomial.hpp"
#include "Term.hpp"

int main(int argc, char const *argv[]) {
    Polynomial::setLessOrder();
    int redo = 1;
    do {
        Polynomial p;
        std::cout << "0) " << p << std::endl;
        p += Term(-1, 2);
        std::cout << "1) " << p << std::endl;
        p += Term(-2, 1);
        std::cout << "2) " << p << std::endl;
        p += Term(3, 0);
        std::cout << "3) " << p << std::endl;
        p += Term(0, 3);
        std::cout << "4) " << p << std::endl;
        Polynomial q(p);
        std::cout << "5) " << (p + q)<< std::endl;
        q = p + q;
        std::cout << "6) " << q << std::endl;
        Term t_power_1(1, 1);
        Term t_power_2(1, 2);
        std::cout << "7) " << (t_power_2 < t_power_1) << std::endl;
        std::cout << "8) " << (t_power_1 + t_power_2) << std::endl;
        std::cout << "9) " << (t_power_1 * t_power_2) << std::endl;
        std::cout << "10) " << (t_power_1 + p + t_power_1) << std::endl;
        std::cout << "11) " << (t_power_1 + p + t_power_1) * t_power_2 << std::endl;
        std::cout << "12) " << (t_power_1 + p + t_power_1) * (t_power_1 + t_power_2) << std::endl;
        std::cout << "13) " << &p << std::endl;
        std::cout << "14) " << &t_power_2 << std::endl;
        std::cout << "15) " << "Type term: ";
        std::cin >> t_power_1;
        std::cout << "16) " << "Input is: " << t_power_1 << std::endl;
        std::cout << "17) " << "Type polynomial: ";
        std::cin >> p;
        std::cout << "18) " << "Input is: " << p << std::endl;
        Polynomial::setGreaterOrder();
    } while (redo--);
    return 0;
}
