#pragma once

#include <array>
#include <bit>
#include <compare>
#include <concepts>
#include <cstdint>

template <int N>
struct smallest_uint
{
    using type = std::conditional_t<N <= 8, uint8_t,
        std::conditional_t<N <= 16, uint16_t,
        std::conditional_t<N <= 32, uint32_t,
        std::conditional_t<N <= 64, uint64_t,
        void>>>>;
};

template <typename T>
constexpr int bit_width(T x) {
    return x == 0 ? 0 : 1 + bit_width(x >> 1);
}

namespace impl {
    template <
        int Poly,
        typename T = typename smallest_uint<bit_width(Poly) - 1>::type>
    struct attr {
        using value_type = T;

        static constexpr value_type modulus = Poly;
        static constexpr int degree = std::bit_width((uint64_t)Poly) - 1;
        static constexpr value_type order = (value_type{1} << degree) - 1;
        static constexpr value_type generator = 2;
        static constexpr value_type zero = 0;
        static constexpr value_type one = 1;
    };

    template <
        int Poly,
        typename T = typename smallest_uint<bit_width(Poly) - 1>::type>
    class calc_inline : public attr<Poly, T> {
    public:
        using attr_type = attr<Poly, T>;
        using value_type = typename attr_type::value_type;

        static constexpr value_type mul(value_type x, value_type y)
        {
            value_type z = 0;

            while (y > 0) {
                if (y & 1) {
                    z ^= x;
                }

                if ((x >> (attr_type::degree - 1)) != 0) {
                    x <<= 1;
                    x ^= attr_type::modulus;
                } else {
                    x <<= 1;
                }

                y >>= 1;
            }

            return z;
        }

        static constexpr value_type div(value_type x, value_type y)
        {
            return mul(x, exp(y, attr_type::order - 1));
        }

        static constexpr value_type exp(int n) {
            return exp(attr_type::generator, n);
        }

        static constexpr value_type exp(value_type x, int n)
        {
            if (n == 0) {
                return attr_type::one;
            }

            if (n < 0) {
                x = exp(x, attr_type::order - 1);
                n = -n;
            }

            value_type z = attr_type::one;

            while (n > 0) {
                if (n & 1) {
                    z = mul(z, x);
                }

                x = mul(x, x);
                n >>= 1;
            }

            return z;
        }

        static constexpr int log(value_type x) {
            if (x == attr_type::zero) {
                return attr_type::order;
            }

            int result = 0;

            while (x != attr_type::one) {
                x = div(x, attr_type::generator);

                result++;
            }

            return result;
        }
    };

    template <
        int Poly,
        typename T = typename smallest_uint<bit_width(Poly) - 1>::type>
    class calc_lookup : public attr<Poly, T> {
    public:
        using attr_type = attr<Poly, T>;
        using value_type = typename attr_type::value_type;

        static constexpr value_type mul(value_type x, value_type y) {
            if (x == attr_type::zero || y == attr_type::zero) {
                return attr_type::zero;
            }

            return exp_table[(log_table[x - 1] + log_table[y - 1]) % attr_type::order];
        }

        static constexpr value_type div(value_type x, value_type y) {
            if (x == attr_type::zero || y == attr_type::zero) {
                return attr_type::zero;
            }

            return exp_table[(attr_type::order + log_table[x - 1] - log_table[y - 1]) % attr_type::order];
        }

        static constexpr value_type exp(int value)
        {
            while (value < 0) {
                value += attr_type::order;
            }
            return exp_table[value % attr_type::order];
        }

        static constexpr value_type exp(value_type x, int n)
        {
            return exp(log(x) * n);
        }

        static constexpr int log(value_type x)
        {
            return x == attr_type::zero ? -1 : log_table[x - 1];
        }

    private:
        static constexpr std::array<value_type, attr_type::order> generate_exp_table()
        {
            std::array<value_type, attr_type::order> table{};
            value_type x = attr_type::one;

            for (int i = 0; i < attr_type::order; i++) {
                table[i] = x;
                x = calc_inline<Poly, T>::mul(x, attr_type::generator);
            }

            return table;
        }

        static constexpr std::array<value_type, attr_type::order> generate_log_table()
        {
            std::array<value_type, attr_type::order> table{};

            for (int i = 0; i < attr_type::order; i++) {
                table[exp_table[i] - 1] = i;
            }

            return table;
        }

    public:
        static constexpr std::array<value_type, attr_type::order> exp_table = generate_exp_table();
        static constexpr std::array<value_type, attr_type::order> log_table = generate_log_table();
    };

template <
    int Poly,
    typename T = typename smallest_uint<bit_width(Poly) - 1>::type>
class calc_default :
    public std::conditional<
        (bit_width(Poly) - 1 > 16),
        impl::calc_inline<Poly, T>,
        impl::calc_lookup<Poly, T>
    >::type
{};

}

template <
    int Poly,
    typename T = typename smallest_uint<bit_width(Poly) - 1>::type,
    template <int, typename> typename Impl = impl::calc_default
>
class GF : public impl::attr<Poly, T> {
public:
    using impl_type = Impl<Poly, T>;
    using value_type = typename impl_type::value_type;

    constexpr GF() : value(impl_type::zero) {}
    constexpr GF(value_type x) : value(x & impl_type::order) {}
    constexpr GF(const GF &other) : value(other.value) {}
    constexpr GF &operator=(const GF &other)
    {
        value = other.value;
        return *this;
    }
    constexpr operator value_type() const
    {
        return value;
    }

    constexpr GF operator+() const
    {
        return *this;
    }
    constexpr GF operator-() const
    {
        return *this;
    }

    constexpr GF operator+(const GF &other) const
    {
        return GF(add(value, other.value));
    }
    constexpr GF operator-(const GF &other) const
    {
        return GF(sub(value, other.value));
    }
    constexpr GF operator*(const GF &other) const
    {
        return GF(mul(value, other.value));
    }
    constexpr GF operator/(const GF &other) const
    {
        return GF(div(value, other.value));
    }

    constexpr GF &operator+=(const GF &other)
    {
        value ^= other.value;
        return *this;
    }
    constexpr GF &operator-=(const GF &other)
    {
        value ^= other.value;
        return *this;
    }
    constexpr GF &operator*=(const GF &other)
    {
        value = mul(value, other.value);
        return *this;
    }
    constexpr GF &operator/=(const GF &other)
    {
        value = div(value, other.value);
        return *this;
    }

    constexpr GF operator^(int n) const
    {
        return GF(exp(value, n));
    }
    constexpr GF &operator^=(int n)
    {
        value = exp(value, n);
        return *this;
    }

    constexpr int log() const
    {
        return log(value);
    }

    constexpr bool operator==(const GF &other) const
    {
        return value == other.value;
    }
    constexpr bool operator!=(const GF &other) const
    {
        return value != other.value;
    }
    constexpr bool operator<(const GF &other) const
    {
        return value < other.value;
    }
    constexpr bool operator<=(const GF &other) const
    {
        return value <= other.value;
    }
    constexpr bool operator>(const GF &other) const
    {
        return value > other.value;
    }
    constexpr bool operator>=(const GF &other) const
    {
        return value >= other.value;
    }
    constexpr std::strong_ordering operator<=>(const GF &other) const
    {
        return value <=> other.value;
    }

    static constexpr GF add(value_type x, value_type y) {
        return GF(x ^ y);
    }
    static constexpr GF sub(value_type x, value_type y) {
        return GF(x ^ y);
    }
    static constexpr GF mul(value_type x, value_type y) {
        return impl_type::mul(x, y);
    }
    static constexpr GF div(value_type x, value_type y) {
        return impl_type::div(x, y);
    }
    static constexpr GF log(value_type x) {
        return impl_type::log(x);
    }
    static constexpr GF exp(int n) {
        return impl_type::exp(n);
    }
    static constexpr GF exp(value_type x, int n) {
        return impl_type::exp(impl_type::log(x) * n);
    }

private:
    value_type value;
};
