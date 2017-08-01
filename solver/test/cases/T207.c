void g0()
{
    char* wp0;
    size_t remaining_bytes0;
    remaining_bytes0 = wp0 - bpout0;
}

void g1()
{
    char* wp1;
    size_t remaining_bytes1;
    remaining_bytes1 = bpout1 - wp1;
}

void g2()
{
    char* wp2;
    unsigned int remaining_bytes2;
    remaining_bytes2 = bpout2 - wp2;
}

void f1() {
    T1 a1;
    long* b1 = a1 - 1;
}

void f2() {
    T2 a2;
    long* b2;
    b2 = a2 - 2;
}

void k() {
    long u;
    u = u1 - u2;
}

void k2() {
    j = j1 - j2;
}

