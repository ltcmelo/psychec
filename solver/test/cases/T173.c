void f()
{
    int* t = v;
    *t = 10;
    double* u = v;
}

void f1()
{
    T1* t1 = v1;
    *t1 = 10;
    double* u1 = v1;
}

void f2()
{
    T2* t2 = v2;
    *t2 = 20;
    U2* u2 = v2;
    *u2 = 3.14;
}

void f3()
{
    U3* u3 = v3;
    *u3 = 3.14;
    T3* t3 = v3;
    *t3 = 30;
}
