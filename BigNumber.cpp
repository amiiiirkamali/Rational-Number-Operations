#include "BigNumber.h"

#include <stack>
#include <stdexcept>

BigNumber::BigNumber(std::string number) : head(nullptr), tail(nullptr) {
    isNegative = false;

    if (number[0] == '-') {
        isNegative = true;
        number = number.substr(1);
    }

    // Trim leading zeros
    int i = 0;
    for (; i < number.size() && number[i] == '0'; i++);
    number = number.substr(i);

    number = number.empty() ? "0" : number;

    for (auto it = number.rbegin(); it != number.rend(); it++) adddigit(*it - '0');
}

BigNumber::BigNumber(const char* str) : BigNumber(static_cast<std::string>(str)) {}

BigNumber::BigNumber(const BigNumber& other) {
    Node* iterator = other.head;
    head = tail = nullptr;

    while (iterator) {
        adddigit(iterator->value);
        iterator = iterator->next;
    }

    isNegative = other.isNegative;
}

BigNumber::~BigNumber() {
    Node* current = head;

    while (current) {
        Node* target = current;
        current = current->next;
        delete target;
    }
}

bool BigNumber::getIsNegative() const { return isNegative; }

BigNumber BigNumber::negate() const {
    BigNumber result = *this;
    result.isNegative = !isNegative;
    return result;
}

bool BigNumber::isZero() const {
    return head == nullptr || (head->value == 0 && head->next == nullptr);
}

void BigNumber::adddigit(int digit) {
    Node* newnode = new Node(digit);

    if (!head) {
        head = tail = newnode;
    }
    else {
        tail->next = newnode;
        tail = newnode;
    }
}

BigNumber BigNumber::add(const BigNumber& other) const {
    if (isZero()) {
        return other;
    }
    else if (other.isZero()) {
        return *this;
    }
    else if (!isNegative && other.isNegative) {
        return subtract(other.abs());
    }
    else if (isNegative && !other.isNegative) {
        return other.subtract(abs());
    }

    Node* p1 = head, * p2 = other.head;
    BigNumber result("0");

    result.head = result.tail = nullptr;
    int carry = 0;

    while (p1 || p2 || carry) {
        int val1 = p1 ? p1->value : 0;
        int val2 = p2 ? p2->value : 0;
        int sum = val1 + val2 + carry;

        carry = sum / 10;

        int digit = sum % 10;

        result.adddigit(digit);

        if (p1) p1 = p1->next;
        if (p2) p2 = p2->next;
    }

    result.isNegative = isNegative;

    return result;
}

BigNumber BigNumber::subtract(const BigNumber& other) const {
    BigNumber result;
    Node* i = head, * j = other.head;

    // x - 0 == x
    if (other.isZero()) return *this;
    // 0 - x == -x
    else if (isZero()) return other.negate();
    // x - -y == x + y
    else if (!isNegative && other.isNegative) return add(other.abs()).negate();
    // -x - y == -(y + x)
    else if (isNegative && !other.isNegative) return abs().add(other.abs()).negate();
    // -x - -y == y - x
    else if (isNegative && other.isNegative) return other.abs().subtract(abs());
    // x - y == -(y - x)
    else if (*this < other) {
        result.isNegative = true;
        std::swap(i, j);
    }

    int borrow = 0;

    // Considering the above conditions, `i` is always greater than `j`.
    for (; i && j; i = i->next, j = j->next) {
        int value = (i->value - j->value - borrow + 10) % 10;
        borrow = i->value < (j->value + borrow);
        result.adddigit(value);
    }

    for (; i; i = i->next) {
        result.adddigit((i->value - borrow + 10) % 10);
        borrow = i->value < borrow;
    }

    return result.trim(); // Trim leading zeros
}

BigNumber BigNumber::multiply(const BigNumber& other) const {
    BigNumber result("0");

    if (isZero() || other.isZero()) return result;

    Node* left = other.head, * right = head;

    for (; right && left; right = right->next, left = left->next);

    if (!right) {
        right = head;
        left = other.head;
    }
    else {
        right = other.head;
        left = head;
    }

    for (int pos = 0; right; right = right->next, pos++) {
        BigNumber ss;
        int carry = 0;

        for (int k = 0; k < pos; k++) ss.adddigit(0);
        for (Node* j = left; j; j = j->next) {
            int value = right->value * j->value + carry;
            carry = value / 10;
            ss.adddigit(value % 10);
        }

        if (carry) ss.adddigit(carry);

        result = result.add(ss);
    }

    result.isNegative = isNegative ^ other.isNegative;

    return result;
}

BigNumber BigNumber::divideby(const BigNumber& other) const {
    if (other.isZero()) throw std::invalid_argument("Division by zero");
    else if (other == "1") return *this;
    else if (abs() < other.abs()) return "0";

    BigNumber current, result, divisor = other.abs();

    result.isNegative = other.isNegative ^ isNegative;

    std::stack<int> dvdnt;

    for (Node* i = head; i; i = i->next) dvdnt.push(i->value);

    for (; !dvdnt.empty() && current < divisor; dvdnt.pop()) {
        current.pushFront(dvdnt.top());
    }

    while (true) {
        int q = 0;
        BigNumber temp = "0";

        while (temp.add(divisor) <= current) {
            temp = temp.add(divisor);
            q++;
        }

        current = current.subtract(temp);
        result.pushFront(q);

        if (!dvdnt.empty()) {
            current.pushFront(dvdnt.top());
            current.trim();
            dvdnt.pop();
        }
        else {
            break;
        }
    }

    return result;
}

bool BigNumber::operator>=(const BigNumber& other) const {
    return !(*this < other);
}

bool BigNumber::operator<(const BigNumber& other) const {
    if (isNegative != other.isNegative) return isNegative;

    Node* i = head, * j = other.head;
    std::stack<std::pair<int, int>> stack;

    for (; i && j; i = i->next, j = j->next) stack.push({ i->value, j->value });

    if (!j != !i) return (!i) ^ isNegative;

    while (!stack.empty() && stack.top().first == stack.top().second) stack.pop();

    return stack.empty() ? false : (stack.top().first < stack.top().second) ^ isNegative;
}

bool BigNumber::operator==(const BigNumber& other) const {
    if (isNegative != other.isNegative) return false;

    Node* i = head, * j = other.head;

    for (; i && j && i->value == j->value; i = i->next, j = j->next);

    return !i && !j;
}

BigNumber BigNumber::modulo(const BigNumber& other) const {
    if (other.isZero()) {
        throw std::invalid_argument("Division by zero");
    }
    else if (abs() < other.abs()) {
        return *this;
    }
    else if (*this == other) {
        return "0";
    }

    // NOTE: Negative numbers are not handled.
    // x % y == x - ([x / y] * y)
    BigNumber result = subtract(divideby(other).multiply(other));

    return result;
}

BigNumber::operator std::string() const {
    std::string str;
    Node* iterator = this->head;

    while (iterator) {
        str.insert(str.cbegin(), iterator->value + '0');
        iterator = iterator->next;
    }

    if (this->isNegative) str.insert(str.cbegin(), '-');

    return str;
}
