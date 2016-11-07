void f() {
    // Currently we infer T2 as int always (because the way operator * is defined
    // as builtin). But T1 can be either an int or a float, due or our conversion
    // table. Although weird, T1 as int compiles successfully but has a truncation.
    T1 a, b = 1.0;
    T2 c = a * b;
}
