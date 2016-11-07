void f() {
    struct node a;
    a.x = 0; // pointer or integer?
    a.w = 5; // integer
    a.y = &a;
    *(a.z) = a;

    struct data b, *c;
    b.m = 0; // m classified as scalar
    b.m = c; // m grows higher in the lattice, classified as pointer
}
