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
    T4 q;
    q = x + z;
    T5 s;
    s = z + x;
    T6 t;
    t = z / x;
    T7 l;
    l = x / z;

    return 23;
}
