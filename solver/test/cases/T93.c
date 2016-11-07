// Basic arithmetic types from stdint.h, which we want to keep with their
// platform/compiler specific definitions. So we don't generate typedefs for
// them and instead add #include <stdint.h>.
void f() {
    int8_t i = 1;
    A a = i;
    int_least32_t ii = 1234;
    B b = ii;
    uint_fast64_t iii = 44444;
    C c = iii;
    uintmax_t iv = 999;
    D d = iv;
}
