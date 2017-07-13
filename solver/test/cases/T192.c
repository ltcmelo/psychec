void f() {
    struct point p = { .y = 10 , .x = 20 };
    T0 v0 = { .a = "aaa", .b = "bbb" };
    T1 v1 = { .foo = "foo", .nest.bar = "bar" };
    T2 v2 = { .a = 3.14, { .b = "xyz", .c.e = 3.14 } };
}
