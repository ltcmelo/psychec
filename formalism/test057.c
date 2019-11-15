T f()
{
    T1 x;
    x->i = 42;
    T2 y;
    y->f = 3.14;
    x->other = y;

    return 0;
}
