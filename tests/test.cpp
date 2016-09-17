#include <cassert>
#include <cstdlib>

namespace impl {

// Get nth number in a parameter pack of length > n
template<size_t n, int head, int ... tail>
struct get_nth {
    enum {value = get_nth<n - 1, tail...>::value};
};

template<size_t head, int... tail>
struct get_nth<0, head, tail...> {
    enum {value = head};
};

// Get nth number from last
template<size_t n, int... args>
struct get_nth_last {
    enum {value = get_nth<sizeof...(args) - 1 - n, args...>::value};
};

// Multiply first n numbers in a parameter pack of length >= n
template<size_t n, int... args>
struct product_first;

template<>
struct product_first<0> {
    enum {value = 1};
};

template<int head, int... tail>
struct product_first<0,  head, tail...> { enum {value = 1};
};

template<size_t n, int head, int... tail>
struct product_first<n, head, tail...> {
    enum {value = head * product_first<n - 1, tail...>::value};
};

// DigitReference
template<int div, int mod>
struct DigitReference {
    int const* data;

    constexpr operator int() const {
        return (*data / div) % mod;
    }

    int operator=(int x) {
        assert(x < mod);
        return *data = *data + (x - *this) * div;
    }
};

} // namespace impl

// Store several bounded ints of different sizes in one int
template<int ... digit_sizes>
struct DenseInt {
    DenseInt(int data_)
    : data(data_) {
    }

    template<typename... ints>
    DenseInt(int head, ints... tail) 
    : DenseInt(tail...) {
        int div = impl::product_first<sizeof...(ints), digit_sizes ...>::value;
        int mod = impl::get_nth_last<sizeof...(ints), digit_sizes ...>::value;
        assert(head < mod);
        data += div * head;
    }

    int data;

    template<size_t n>
    auto at() const {
        static_assert(n < sizeof...(digit_sizes), "get argument out of range");

        return DigitReference<impl::product_first<n, digit_sizes ...>::value,
                              impl::get_nth_last<n, digit_sizes ...>::value>{
                                  &data
                              };
    }

    template<size_t n>
    int get() const {
        return at<n>();
    }
    
    template<size_t n>
    int set(int x) {
        return at<n>() = x;
    }

    constexpr operator int() const {
        return data;
    }

    operator++()
};

#include <iostream>

int main() {
    using ArrayIndex = DenseInt<9, 9, 6>;

}
