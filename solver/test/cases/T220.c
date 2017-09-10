struct T { int member; };

void f() {
    struct T x; // the `struct T' above
    x.member = 10;
    A a = x;

    T y; // *not* the `struct T' above
    y.member = 10;
    B b = y;
}
