T f()
{
    T1 r;
    r->x = 42;
    T2 q;
    q = r;
    q->x = r->x;
    return r->x;
}
