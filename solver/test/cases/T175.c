void f()
{
    V v = g();
    *v = 3.14;
    U u = g();
    (*u).m = 10;
    W w = g();
    w->n = "foo";
    Z z = g();
    z->y->p = 3.14;
}
