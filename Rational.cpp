#include <iostream>

#include "Rational.h"
#include "BigNumber.h"

Rational::Rational(const BigNumber& num, const BigNumber& den) : numerator(num), denominator(den) {
    simplify();
}

void Rational::print() const {
    std::cout << *this;
}

Rational Rational::add(const Rational& other) const {
    BigNumber newNumerator = numerator.multiply(other.denominator).add(denominator.multiply(other.numerator));
    BigNumber newDenominator = denominator.multiply(other.denominator);
    Rational result(newNumerator, newDenominator);

    result.simplify();
    return result;
}

Rational Rational::subtract(const Rational& other) const {
    BigNumber newNumerator = numerator.multiply(other.denominator).subtract(denominator.multiply(other.numerator));
    BigNumber newDenominator = denominator.multiply(other.denominator);
    Rational result(newNumerator, newDenominator);
    result.simplify();
    return result;
}

Rational Rational::multiply(const Rational& other) const {
    BigNumber newNumerator = numerator.multiply(other.numerator);
    BigNumber newDenominator = denominator.multiply(other.denominator);
    Rational result(newNumerator, newDenominator);
    result.simplify();
    return result;
}

Rational Rational::divide(const Rational& other) const {
    BigNumber newNumerator = numerator.multiply(other.denominator);
    BigNumber newDenominator = denominator.multiply(other.numerator);
    Rational result(newNumerator, newDenominator);
    result.simplify();
    return result;
}

void Rational::simplify() {
    BigNumber gcd = calculateGCD(numerator, denominator);
    numerator = numerator.divideby(gcd);
    denominator = denominator.divideby(gcd);

    if (denominator.getIsNegative()) {
        numerator = numerator.negate();
        denominator = denominator.negate();
    }
}

BigNumber Rational::calculateGCD(const BigNumber& a, const BigNumber& b) {
    if (b.isZero()) return a;
    return calculateGCD(b, a.modulo(b));
}
