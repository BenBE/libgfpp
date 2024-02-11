#include <boost/test/included/unit_test.hpp>
#include "gf++.hpp"

// Define the finite field GF(2^8) with primitive polynomial 0x11d
using GF8 = GF<0x11d, uint16_t, impl::calc_lookup>;

// Define the inline and lookup implementations of GF(2^8)
using GF8_inline = GF<0x11d, uint8_t, impl::calc_inline>;
using GF8_lookup = GF<0x11d, uint8_t, impl::calc_lookup>;

// Define the finite field GF(2^13) with primitive polynomial 0x201b
using GF13 = GF<0x201b, uint16_t, impl::calc_inline>;

// Define the inline and lookup implementations of GF(2^13)
using GF13_inline = GF<0x201b, uint16_t, impl::calc_inline>;
using GF13_lookup = GF<0x201b, uint16_t, impl::calc_lookup>;

// Define a test suite
BOOST_AUTO_TEST_SUITE(Dump)

// Define a test case for comparing exp_table and log_table of GF(2^8)
BOOST_AUTO_TEST_CASE(Compare_GF8)
{
    BOOST_CHECK_EQUAL(GF8::impl_type::order, GF8_inline::impl_type::order);
    BOOST_CHECK_EQUAL(GF8::impl_type::order, GF8_lookup::impl_type::order);

    // Loop through all the elements in the field
    for (uint16_t i = 0; i < GF8::impl_type::order; i++) {
        // Convert i to a GF8 element
        GF8 x = GF8(i);
        GF8 y = GF8(i + 1);

        // Check that the exp_table and log_table of GF8, GF8_inline, and GF8_lookup are consistent
        BOOST_CHECK_EQUAL(GF8::log(GF8::exp(x)), x);
        BOOST_CHECK_EQUAL(GF8_inline::log(GF8_inline::exp(GF8_inline(x))), x);
        BOOST_CHECK_EQUAL(GF8_lookup::log(GF8_lookup::exp(GF8_lookup(x))), x);

        BOOST_CHECK_EQUAL(GF8::exp(GF8::log(y)), y);
        BOOST_CHECK_EQUAL(GF8_inline::exp(GF8_inline::log(GF8_inline(y))), y);
        BOOST_CHECK_EQUAL(GF8_lookup::exp(GF8_lookup::log(GF8_lookup(y))), y);
    }
}

BOOST_AUTO_TEST_CASE(FullScan_GF8)
{
    BOOST_CHECK_EQUAL(GF8::impl_type::order, GF8_inline::impl_type::order);
    BOOST_CHECK_EQUAL(GF8::impl_type::order, GF8_lookup::impl_type::order);

    // Loop through all the elements in the field
    for (uint16_t i = 0; i < GF8::impl_type::order; i++) {
        for (uint16_t j = 0; j < GF8::impl_type::order; j++) {
            // Convert i and j to GF8 elements
            GF8 x = GF8(i);
            GF8 y = GF8(j);

            // Check that operations for GF13_inline and GF13_lookup are consistent
            BOOST_CHECK_EQUAL(GF8_inline::add(x, y), GF8_lookup::add(x, y));
            BOOST_CHECK_EQUAL(GF8_inline::sub(x, y), GF8_lookup::sub(x, y));
            BOOST_CHECK_EQUAL(GF8_inline::mul(x, y), GF8_lookup::mul(x, y));
            BOOST_CHECK_EQUAL(GF8_inline::div(x, y), GF8_lookup::div(x, y));
        }
    }
}

// Define a test case for comparing exp_table and log_table of GF(2^13)
BOOST_AUTO_TEST_CASE(Compare_GF13)
{
    BOOST_CHECK_EQUAL(GF13::impl_type::order, GF13_inline::impl_type::order);
    BOOST_CHECK_EQUAL(GF13::impl_type::order, GF13_lookup::impl_type::order);

    // Loop through all the elements in the field
    for (uint16_t i = 0; i < GF13::impl_type::order; i++) {
        // Convert i to a GF13 element
        GF13 x = GF13(i);
        GF13 y = GF13(i + 1);

        // Check that the exp_table and log_table of GF13, GF13_inline, and GF13_lookup are consistent
        BOOST_CHECK_EQUAL(GF13::log(GF13::exp(x)), x);
        BOOST_CHECK_EQUAL(GF13_inline::log(GF13_inline::exp(GF13_inline(x))), x);
        BOOST_CHECK_EQUAL(GF13_lookup::log(GF13_lookup::exp(GF13_lookup(x))), x);

        BOOST_CHECK_EQUAL(GF13::exp(GF13::log(y)), y);
        BOOST_CHECK_EQUAL(GF13_inline::exp(GF13_inline::log(GF13_inline(y))), y);
        BOOST_CHECK_EQUAL(GF13_lookup::exp(GF13_lookup::log(GF13_lookup(y))), y);
    }
}

// Define a test case for comparing exp_table and log_table of GF(2^8)
BOOST_AUTO_TEST_CASE(FullScan_GF13)
{
    BOOST_CHECK_EQUAL(GF13::impl_type::order, GF13_inline::impl_type::order);
    BOOST_CHECK_EQUAL(GF13::impl_type::order, GF13_lookup::impl_type::order);

    // Loop through all the elements in the field
    for (uint16_t i = 0; i < GF13::impl_type::order; i++) {
        for (uint16_t j = 0; j < GF13::impl_type::order; j++) {
            // Convert i and j to GF13 elements
            GF13 x = GF13(i);
            GF13 y = GF13(j);

            // Check that operations for GF13_inline and GF13_lookup are consistent
            BOOST_CHECK_EQUAL(GF13_inline::add(x, y), GF13_lookup::add(x, y));
            BOOST_CHECK_EQUAL(GF13_inline::sub(x, y), GF13_lookup::sub(x, y));
            BOOST_CHECK_EQUAL(GF13_inline::mul(x, y), GF13_lookup::mul(x, y));
            BOOST_CHECK_EQUAL(GF13_inline::div(x, y), GF13_lookup::div(x, y));
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
