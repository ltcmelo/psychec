T f()
{
    const int* cip;
    T1 x;
    x = cip;
    
    const double* cdp;
    T2 y;
    y = cdp;

    x = y;

    return 42;
}
