// Detection of variadic functions doesn't consider implicit conversions.
void f()
{
    g("g", 10);
    g("g", 3.14);

    k("k");
    k("k", 10);
    k("k", 10, 3.14);

    m(1, 3.14, "abc");
    m(1, 3.14, 100);
    m(1, 3.14, 3.14, "xyz");

    n(1, 3, "abc");
    n(1, 3, 100);
    n(1, 3, 3.14, "xyz");
}
