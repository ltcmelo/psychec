void f() {
    int* a;
    X x = a;
    free(a); // X should be int* (void* gets implicitly converted).
}
