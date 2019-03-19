T f()
{
    const int* cip;
    T1 x;
    x = cip;
    T11 xx;
    xx = x;
 
    int* ip;
    T2 y;
    y = ip;
    T22 yy;
    yy = y;

    xx = yy;
    return 42;
}
