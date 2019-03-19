T f()
{
    int* ip;
    T1 y;
    y = ip;
    const int* cip;
    T2 x;
    x = cip;

    x = y;
    return 42;
}
