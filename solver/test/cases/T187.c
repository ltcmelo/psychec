
void f1() {
    volatile int* a1;
    T1 b1 = a1;
}

void f2() {
    T2* a2;
    volatile int* b2 = a2;
}

void f3() {
    T3* a3;
    volatile int* b3 = a3;
    int* c3 = a3;
}
