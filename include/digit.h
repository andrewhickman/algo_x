#include <cassert>
#include <cstdlib>
#include <type_traits>

namespace Util { 
    // Get nth number in a parameter pack of length > n
    constexpr int get_nth(size_t n, int head) {
        assert(n == 0);
        return head;
    }

    template<typename... ints>
    constexpr int get_nth(size_t n, int head, ints... tail) {
        return (n == 0) ? head : get_nth(n - 1, tail...);
    }

    // Multiply first n numbers in a parameter pack
    constexpr int product_first(int) {
        return 1;
    }

    template<typename... ints>
    constexpr int product_first(int n, int head, ints... tail) {
        return (n == 0) ? 1 : head * product_first(n-1, tail...);
    }

    // Multiply last n numbers in a parameter pack
    constexpr int product_last(int) {
        return 1;
    }

    template<typename... ints>
    constexpr int product_last(size_t n, int head, ints... tail) {
        if (n == 1 + sizeof...(tail))
            return head * product_last(n - 1, tail...);
        else
            return product_last(n, tail...);
    }

    // Multiply all numbers in a parameter pack
    constexpr int product() {
        return 1;
    }

    template<typename... ints>
    constexpr int product(int head, ints... tail) {
        return head * product(tail...);
    }
} // namespace util

struct DigitReference {
    const int div, mod;
    int& data;

    operator int() const {
        return (data / div) % mod;
    }

    int operator=(int x) const {
        assert(x < mod);
        return data += (x - *this) * div;
    }
};

// Store several bounded ints of different sizes in one int
template<int... digit_sizes>
struct DigitInt {
    DigitInt()
    : data(0) {
    }

    DigitInt(int data_)
    : data(data_) {
        assert(data < max_size);
    }

    template<typename ... ints, 
             std::enable_if_t<sizeof...(ints) == sizeof...(digit_sizes), int> = 1>
    DigitInt(ints ... xs)
    : data(0) {
        init(xs...);
    }

    auto at(int n) {
        assert(n < num_digits);

        return (*this)[n];
    }

    auto operator[](int n) {
        return DigitReference{
            Util::product_last(num_digits - 1 - n, digit_sizes...),
            Util::get_nth(n, digit_sizes...),
            data
        };
    }

    int get(size_t n) {
        return at(n);
    }

    int set(size_t n, int x) {
        assert(x < Util::get_nth(n, digit_sizes...));
        return at(n) = x;
    }

    operator int() {
        return data;
    }

    int operator++() {
        return ++data;
    }

    int operator++(int) {
        return data++;
    }

    static constexpr int max_size   = Util::product(digit_sizes...);
    static constexpr int num_digits = sizeof...(digit_sizes);

    static_assert(max_size != 0, "All template args must be non-zero");
    static_assert(num_digits > 0, "Must have at least one template arg");

private:
    void init() {
    }

    template<typename... ints>
    void init(int first, ints ... tail) {
        init(tail...);
        set(num_digits - 1 - sizeof...(ints), first);
    }

    int data;
};

