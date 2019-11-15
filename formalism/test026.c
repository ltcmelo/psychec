T f()
{
    const int* cip;
    T1 x;
    x = cip;
 
    int* ip;
    T2 y;
    y = ip;

    x = y;
    return 42;
}
