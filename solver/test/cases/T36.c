typedef struct A { int i; } A;

void f() {
    // x1 must be int*
    T1 x1;
    x1 = 0;
    *x1 = 10; // because of this

    // x2 must be double*
    T2 x2;
    double y2 = 9.9;
    x2 = 0;
    x2 = &y2; // because of this

    // x3 must be int*
    T3 x3;
    int y3;
    *x3 = 10;
    x3 = 0;
    x3 = &y3;

    // x4 must be int
    T4 x4;
    x4 = 0;
    x4*10; // because of this

    // x5 must be int
    T5 x5;
    (void)(x5*10); // because of this

    // x6 must be int
    T6 x6;
    int y6 = x6*10; // because of this
    x6 = 0;

    // x7 must be an int*
    T7 x7;
    int i7;
    x7 = &i7; // because of this

    // x8 must be an int**
    T8 x8;
    x8 = &x8; // because of this
    **x8 = 10;

    // x9 must a pointer, but the underlying element type can be any
    T9 x9;
    *x9; // because of this
    // x10 must be a pointer
    T10 x10;
    y10 = 100;
    x10 = 0;
    *x10 = 100; // because of this

    // y11 must be int and x11 must be pointer
    T11 x11;
    TT11 y11 = 10;
    y11 * 2; // because of this
    *x11 = y11; // because of this

    // x12 must be a pointer
    T12 x12;
    x12 = NULL;
    *x12 = 9.9;

    // x13 must be int*
    T13 x13;
    A a;
    *x13 = a.i; // because of this

    // x14 must be a double*
    T14 x14;
    double* y14;
    x14 = 0;
    x14 = y14;

    // x15 must be a double*
    T15 x15;
    x15 = 0;
    *x15 = 9.9; // because of this

    // x16 must be a double
    T16 x16;
    double y16;
    x16*y16; // because of this
}
