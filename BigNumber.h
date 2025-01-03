#pragma once

#include <iostream>
#include <string>

#include "Node.h"

class BigNumber {
    Node* head;
    Node* tail;
    bool isNegative;

public:
    BigNumber() : head(nullptr), tail(nullptr), isNegative(false) {}
    BigNumber(std::string);
    BigNumber(const char* str);
    BigNumber(const BigNumber& other);

    ~BigNumber();

    void adddigit(int);

    void pushFront(int value) {
        head = new Node(value, head);
        if (!tail) tail = head;
    }

    BigNumber negate() const;
    BigNumber abs() const {
        BigNumber result = *this;
        result.isNegative = false;
        return result;
    }

    bool isZero() const;
    bool getIsNegative() const;

    BigNumber& trim() {
        Node* i = head, * target = nullptr;
        if (!i || !i->next) return *this;

        // Possile memory leak.
        while (i && i->next) {
            if (i->next->value == 0) target = target ? target : i;
            else target = nullptr;
            i = i->next;
        }

        if (target) {
            tail = target;
            target->next = nullptr;
        }

        return *this;
    }

    BigNumber add(const BigNumber&) const;
    BigNumber multiply(const BigNumber&) const;
    BigNumber divideby(const BigNumber&) const;
    BigNumber subtract(const BigNumber&) const;
    BigNumber modulo(const BigNumber& other) const;

    BigNumber& operator=(const BigNumber& other) {
        Node* iterator = other.head;
        // FIXME: Possible memory leak on head.
        head = tail = nullptr;

        while (iterator) {
            adddigit(iterator->value);
            iterator = iterator->next;
        }

        isNegative = other.isNegative;

        return *this;
    }

    bool operator>=(const BigNumber&) const;
    bool operator<(const BigNumber& other) const;
    bool operator==(const BigNumber& other) const;
    bool operator!=(const BigNumber& other) const { return !(*this == other); }
    bool operator<=(const BigNumber& other) const { return (*this < other) || (*this == other); }
    bool operator>(const BigNumber& other) const { return !(*this <= other); }

    // cast to std::string
    operator std::string() const;

    // std::ostream << operator
    friend std::ostream& operator<<(std::ostream& out, const BigNumber& object);
};

inline std::ostream& operator<<(std::ostream& out, const BigNumber& object) {
    return (out << static_cast<std::string>(object));
}
