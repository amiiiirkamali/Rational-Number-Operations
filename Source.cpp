#include <iomanip>
#include <iostream>
#include <string>
#include "BigNumber.h"
#include "Rational.h"
using namespace std;

#define TEST(e) std::cout << std::setw(40) << std::left << std::boolalpha << #e": " << (e) << std::endl;

int main() {
    std::string num1, den1, num2, den2;

    cout << "Enter the numerator of the first rational number: ";
    cin >> num1;
    cout << "Enter the denominator of the first rational number: ";
    cin >> den1;

    cout << "Enter the numerator of the second rational number: ";
    cin >> num2;
    cout << "Enter the denominator of the second rational number: ";
    cin >> den2;

    BigNumber bnum(num1);
    BigNumber bdnum(den1);
    BigNumber bnum2(num2);
    BigNumber bdnum2(den2);
    Rational rational1(bnum, bdnum);
    Rational rational2(bnum2, bdnum2);

    Rational sum = rational1.add(rational2);
    Rational difference = rational1.subtract(rational2);
    Rational product = rational1.multiply(rational2);
    Rational quotient = rational1.divide(rational2);

    cout << "First Rational Number: ";
    rational1.print();
    cout << endl;

    cout << "Second Rational Number: ";
    rational2.print();
    cout << endl;

    cout << "Sum: ";
    sum.print();
    cout << endl;

    cout << "Difference: ";
    difference.print();
    cout << endl;

    cout << "Product: ";
    product.print();
    cout << endl;

    cout << "Quotient: ";
    quotient.print();
    cout << endl;

    return 0;
}
