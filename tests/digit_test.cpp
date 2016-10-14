#include "digit.h"

#include <iostream>

bool decimal() {
    std::cout << "Running test: " << __PRETTY_FUNCTION__ << '\n';
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < 10; ++k) {
                int val = 100*i + 10*j +k;
                if (DigitInt<10, 10, 10>(i, j , k) != val) {
                    return false; }
            }
        }
    }
    return true;
}

bool getset() {
    std::cout << "Running test: " << __PRETTY_FUNCTION__ << '\n';
    DigitInt<8, 5, 3> d;
    for (int i = 0; i < 8; ++i) {
        d.set(0, i);
        if (i != d.get(0)) {
            return false;
        }
    }
    for (int i = 0; i < 5; ++i) {
        d.set(1, i);
        if (i != d.get(1)) {
            return false;
        }
    }
    for (int i = 0; i < 3; ++i) {
        d.set(2, i);
        if (i != d.get(2)) {
            return false;
        }
    }
    return true;
}

bool array() {
    std::cout << "Running test: " << __PRETTY_FUNCTION__ << '\n';
    union {
        int flat[120];
        int multi[5][6][4];
    } array;

    using IndexT = DigitInt<5, 6, 4>;

    int count = 0;
    for (IndexT i{}; i < i.max_size; i++) {
        array.flat[i] = count;
        ++count;
    }

    count = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            for (int k = 0; k < 4; ++k) {
                if (array.multi[i][j][k] != count)
                    return false;
                ++count;
            }
        }
    }
    return true;
}

bool inits() {
    std::cout << "Running test: " << __PRETTY_FUNCTION__ << '\n';
    for (int i = 0; i < 23; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 8; ++k) {
                DigitInt<23, 5, 8> d1(i, j , k);
                DigitInt<23, 5, 8> d2(static_cast<int>(d1));
                if (static_cast<int>(d1) != static_cast<int>(d2)) {
                    return false;
                }
                if (d1.get(0) != i)
                    return false;
                if (d1.get(1) != j)
                    return false;
                if (d1.get(2) != k)
                    return false;
            }
        }
    }
    return true;
}

int main() {
    assert(decimal());
    assert(getset());
    assert(array());
    assert(inits());
    std::cout << "All tests passed!\n";
}
