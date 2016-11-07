void f() {
    char *p;
    if (p) {}
    T a = p;

    char *p2;
    if (!p2) {}
    T2 b = !p2;
}
