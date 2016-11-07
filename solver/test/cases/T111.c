
void f() {
    const char* s;
    char const* s1;
    const char* const s2;
    g(s);
    h(s1);
    j(s2);

    const T1 x;
    T1 const y;
    T1 z;
    k(x);
    l(y);
    m(z);
    m(x);
}

