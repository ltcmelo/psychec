int k()
{
    const int x;
    const int y;
    int z;

    T1 m;
    m = x || y;
    T2 n;
    n = x || z;
    T3 p;
    p = z || x;

    return 23;
}
