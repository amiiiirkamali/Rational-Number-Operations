#pragma once
#include "BigNumber.h"

class Rational {
private:
    BigNumber numerator;
    BigNumber denominator;

public:
    Rational(const BigNumber&, const BigNumber&);
    void print() const;
    Rational add(const Rational&) const;
    Rational subtract(const Rational&) const;
    Rational multiply(const Rational&) const;
    Rational divide(const Rational&) const;
    void simplify();

    static BigNumber calculateGCD(const BigNumber& a, const BigNumber& b);

    // cast to std::string
    operator std::string() const {
        return static_cast<std::string>(numerator)
            + std::string("/")
            + static_cast<std::string>(denominator);
    }

    // std::ostream << operator
    friend std::ostream& operator<<(std::ostream& out, const Rational& object) {
        return (out << static_cast<std::string>(object));
    }
};
