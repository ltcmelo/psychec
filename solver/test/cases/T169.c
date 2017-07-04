void f()
{
    const struct Type* x = a->b;
    g(a->b);
    g(x);
}
