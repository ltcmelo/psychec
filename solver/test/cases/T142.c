void f0() {
    T0 a0;
    const int* b0 = a0;
    // expect T0 = int*
}

void f1() {
    T1 a1;
    const int* b1 = a1;
    a1 = b1;
    // expect T1 = const int*
}

void f2() {
    T2 a2;
    const int* b2 = a2;
    int* c2 = a2;
    // expect T2 = int*
}

void f3() {
    T3 a3;
    int* c3 = a3;
    const int* b3 = a3;
    // expect T3 = int*
}

void f4() {
    const int* x4;
    T4 a4;
    a4 = x4;
    // expect T4 = const int*
}

void f5() {
    T5 a5;
    const int* b5;
    a5 = b5;
    // expect T5 = const int*
}

void f6() {
    T6 a6;
    T6 b6 = a6;
    const int* c6 = b6;
    // expect T6 = int* (but could also be const int*)
}

void f7() {
    T7x a7;
    T7y b7 = a7;
    const int* c7 = b7;
    // expect T7 = int* (but could also be const int*)
}

void f8() {
    T8x a8;
    T8y b8 = a8;
    const int* c8;
    b8 = c8;
    // expect T8x = const int* (but could also be int*)
    // expect T8y = const int*
}

void f9() {
    T9x a9;
    T9y* b9 = a9;
    const int* c9;
    b9 = c9;
    // expect T9x = const int* (but could also be int*)
    // expect T9y = const int
}

void f10() {
    T10x a10;
    const int* c10;
    T10y* b10 = a10;
    b10 = c10;
    // expect T10x = const int * (but could also be int*)
    // expect T10y = const int
}

void f11() {
    T11 a11;
    int* c11 = a11;
    const int* b11 = a11;
    // expect T11 = int*
}

void f12() {
    const int* c12;
    T12 a12;
    int* b12 = a12;
    c12 = a12;
    // expect T12 = int*
}

void f13() {
    const int* c13;
    int* a13;
    T13* b13 = a13;
    c13 = a13;
    // expect T13 = int
}

void f14() {
    const int* c14;
    T14x a14;
    T14y* b14 = a14;
    c14 = a14;
    // expect T14x = const int* (but could be int*)
    // expect T14y = const int (but could be int)
}

void f15() {
    const int* c15;
    T15x a15;
    const T15y* b15 = a15;
    c15 = a15;
    // expect T15x  = const int* (but could be int*)
    // expect T15y = const int (but could be int)
}

void f16() {
    T16x a16;
    const T16y* b16 = a16;
    const int* c16;
    c16 = a16;
    // expect T16x  = const int* (but could be int*)
    // expect T16y = const int (but could be int)
}

void f17() {
    T17x a17;
    const T17y* b17 = a17;
    const int* c17 = a17;
    // expect T17x  = int*
    // expect T17y = int
}

void f18() {
    T18x a18;
    const T18y b18 = a18;
    const int* c18 = a18;
    // expect T18x  = int*
    // expect T18y = int
}

void f19() {
    T19x a19;
    T19y b19;
    int* c19 = a19;
    b19 = a19;
    // expect T19x  = int*
    // expect T19y = int*
}

void f20() {
    T20x a20;
    T20y b20;
    int* c20 = a20;
    a20 = b20;
    // expect T20x  = int*
    // expect T20y = int*
}

void f21() {
    T21x a21;
    const T21y* b21 = a21;
    const int* c21 = a21;
    // expect T21x  = int*
    // expect T21y = int
}

void f22() {
    int** p22;
    T22 c22 = p22;
    // expect T22 = int**
}

void f23() {
    const int a23;
    T23 b34 = a23;
    // expect T23 = int (but could also be const int)
}

void f24() {
    const int a24;
    T24 b24;
    b24 = a24;
    // expect T24 = int
}

void f25() {
    const int a25;
    T25 b25;
    b25 = a25;
    const int c25 = a25;
    // expect T25 = int
}

void f26() {
    const int a26;
    const int c26 = a26;
    T26 b26;
    b26 = a26;
    // expect T26 = int
}

void f27() {
    const int a27;
    T27 c27;
    c27 = a27;
    // expect T27 = int
}

void f28() {
    const int a28;
    const int b28 = a28;
    // nothing to infer, but must unify
}

void f29() {
    const int a29;
    int b29 = a29;
    // nothing to infer, but must unify
}

void f30() {
    const int a30;
    int b30;
    b30 = a30;
    // nothing to infer, but must unify
}

void f31() {
    int* a31;
    T31 x31 = a31;
    const int* b31;
    T31 y31 = b31;
}

void f32() {
    const int* a32;
    T32 x32 = a32;
    int* y32;
    T32 t32 = y32;
}

void f33() {
    int* a33;
    T33 x33 = a33;
    const int* b33;
    x33 = b33;
}

void f34() {
    T34 x34;
    int* a34;
    x34 = a34;
    const int* b34;
    x34 = b34;
}

void f35() {
    T35 x35;
    const int* b35;
    x35 = b35;
    int* a35;
    x35 = a35;
}

// TODO

//void f100() {
//    int* const* p;
//    T100 c = p;
//    // expect T100 = int* const*
//}

//void f200() {
//    int const* const* p;
//    T200 c = p;
//    // expect T200 = int const* const*
//}
