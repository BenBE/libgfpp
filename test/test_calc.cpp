#include <boost/test/included/unit_test.hpp>
#include "gf++.hpp"

// Define the finite field GF(2^8) with primitive polynomial 0x11d
using GF8 = GF<0x11d, uint16_t>;

// Define the finite field GF(2^8) with primitive polynomial 0x11d
using GF8_inline = GF<0x11d, uint8_t, impl::calc_inline>;
using GF8_lookup = GF<0x11d, uint8_t, impl::calc_lookup>;

// Define some values in the field
const GF8 zero = GF8(0);
const GF8 one = GF8(1);
const GF8 two = GF8(2);
const GF8 three = GF8(3);

// Define a test suite
BOOST_AUTO_TEST_SUITE(Calc)

// Define a test case for addition
BOOST_AUTO_TEST_CASE(Addition)
{
    // Check that zero is the additive identity
    BOOST_CHECK_EQUAL(zero + zero, zero);
    BOOST_CHECK_EQUAL(one + zero, one);
    BOOST_CHECK_EQUAL(two + zero, two);

    // Check that addition is commutative
    BOOST_CHECK_EQUAL(one + two, two + one);
    BOOST_CHECK_EQUAL(two + three, three + two);

    // Check that addition is associative
    BOOST_CHECK_EQUAL((one + two) + three, one + (two + three));

    // Check that addition is inverse
    BOOST_CHECK_EQUAL(one + one, zero);
    BOOST_CHECK_EQUAL(two + two, zero);
    BOOST_CHECK_EQUAL(three + three, zero);

    // Check that the inline and lookup implementations are consistent
    BOOST_CHECK_EQUAL(GF8_inline(one) + GF8_inline(two), GF8_lookup(one) + GF8_lookup(two));
    BOOST_CHECK_EQUAL(GF8_inline(two) + GF8_inline(three), GF8_lookup(two) + GF8_lookup(three));
}

// Define a test case for subtraction
BOOST_AUTO_TEST_CASE(Subtraction)
{
    // Check that zero is the subtractive identity
    BOOST_CHECK_EQUAL(zero - zero, zero);
    BOOST_CHECK_EQUAL(one - zero, one);
    BOOST_CHECK_EQUAL(two - zero, two);

    // Check that subtraction is equivalent to addition with inverse
    BOOST_CHECK_EQUAL(one - two, one + two);
    BOOST_CHECK_EQUAL(two - three, two + three);

    // Check that the inline and lookup implementations are consistent
    BOOST_CHECK_EQUAL(GF8_inline(one) - GF8_inline(two), GF8_lookup(one) - GF8_lookup(two));
    BOOST_CHECK_EQUAL(GF8_inline(two) - GF8_inline(three), GF8_lookup(two) - GF8_lookup(three));
}

// Define a test case for multiplication
BOOST_AUTO_TEST_CASE(Multiplication)
{
    // Check that zero is the multiplicative zero
    BOOST_CHECK_EQUAL(zero * zero, zero);
    BOOST_CHECK_EQUAL(one * zero, zero);
    BOOST_CHECK_EQUAL(two * zero, zero);

    // Check that one is the multiplicative identity
    BOOST_CHECK_EQUAL(one * one, one);
    BOOST_CHECK_EQUAL(one * two, two);
    BOOST_CHECK_EQUAL(one * three, three);

    // Check that multiplication is commutative
    BOOST_CHECK_EQUAL(two * three, three * two);

    // Check that multiplication is associative
    BOOST_CHECK_EQUAL((one * two) * three, one * (two * three));

    // Check that the inline and lookup implementations are consistent
    BOOST_CHECK_EQUAL(GF8_inline(one) * GF8_inline(two), GF8_lookup(one) * GF8_lookup(two));
    BOOST_CHECK_EQUAL(GF8_inline(two) * GF8_inline(three), GF8_lookup(two) * GF8_lookup(three));
}

// Define a test case for division
BOOST_AUTO_TEST_CASE(Division)
{
    // Check that division by zero is undefined
    // BOOST_CHECK_THROW(zero / zero, std::runtime_error);
    // BOOST_CHECK_THROW(one / zero, std::runtime_error);
    // BOOST_CHECK_THROW(two / zero, std::runtime_error);

    // Check that division by one is equivalent to multiplication by one
    BOOST_CHECK_EQUAL(zero / one, zero);
    BOOST_CHECK_EQUAL(one / one, one);
    BOOST_CHECK_EQUAL(two / one, two);

    // Check that division is equivalent to multiplication by inverse
    BOOST_CHECK_EQUAL(one / two, GF8::exp(two, -1));
    BOOST_CHECK_EQUAL(one / two, GF8::mul(one, GF8(0x8e)));
    BOOST_CHECK_EQUAL(one / two, GF8::mul(one, GF8::exp(two, -1)));
    BOOST_CHECK_EQUAL(two / three, GF8::mul(two, GF8::exp(three, -1)));

    // Check that the inline and lookup implementations are consistent
    BOOST_CHECK_EQUAL(GF8_inline(one) / GF8_inline(two), GF8_lookup(one) / GF8_lookup(two));
    BOOST_CHECK_EQUAL(GF8_inline(two) / GF8_inline(three), GF8_lookup(two) / GF8_lookup(three));
}

// Define a test case for logarithm
BOOST_AUTO_TEST_CASE(Logarithm)
{
    // Check that logarithm of zero is undefined
    // BOOST_CHECK_THROW(log(zero), std::runtime_error);

    // Check that logarithm of one is zero
    BOOST_CHECK_EQUAL(GF8::log(one), zero);

    // Check that logarithm is consistent with exponentiation
    BOOST_CHECK_EQUAL(GF8::log(two), one);
    BOOST_CHECK_EQUAL(GF8::log(three), GF8(0x19));

    // Check that the inline and lookup implementations are consistent
    BOOST_CHECK_EQUAL(GF8_inline::log(GF8_inline(two)), GF8_lookup::log(GF8_lookup(two)));
    BOOST_CHECK_EQUAL(GF8_inline::log(GF8_inline(three)), GF8_lookup::log(GF8_lookup(three)));
}

// Define a test case for exponentiation
BOOST_AUTO_TEST_CASE(Exponentiation)
{
    // Check that exponentiation of zero is one
    BOOST_CHECK_EQUAL(GF8::exp(0), one);

    // Check that exponentiation of one is two
    BOOST_CHECK_EQUAL(GF8::exp(1), two);

    // Check that exponentiation is consistent with logarithm
    BOOST_CHECK_EQUAL(GF8::exp(GF8::log(two)), two);
    BOOST_CHECK_EQUAL(GF8::exp(GF8::log(three)), three);

    // Check that the inline and lookup implementations are consistent
    BOOST_CHECK_EQUAL(GF8_inline::exp(GF8_inline::log(GF8_inline(two))), GF8_lookup::exp(GF8_lookup::log(GF8_lookup(two))));
    BOOST_CHECK_EQUAL(GF8_inline::exp(GF8_inline::log(GF8_inline(three))), GF8_lookup::exp(GF8_lookup::log(GF8_lookup(three))));
}

// End of test suite
BOOST_AUTO_TEST_SUITE_END()
