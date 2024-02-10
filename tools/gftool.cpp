#include "gf++.hpp"

#include <cstdio>

int main() {
    using GF8 = GF<0x11d, uint8_t, impl::calc_lookup>;

    size_t x = 0;
    for(auto& v : GF8::impl_type::exp_table) {
        printf("%0*x, ", (GF8::impl_type::degree + 3) / 4, v);
        if (!(++x % 16)) {
            printf("\n");
        }
    }
    printf("\n\n");

    x = 0;
    for(auto& v : GF8::impl_type::log_table) {
        printf("%0*x, ", (GF8::impl_type::degree + 3) / 4, v);
        if (!(++x % 16)) {
            printf("\n");
        }
    }
    printf("\n");

    return 0;
}
