Edge createEDGE()
{
    Edge e;
    e.v = 10;
    return e;
}

void GRAPHinsertE(Edge e);

void f()
{
    Edge e = createEDGE();
    GRAPHinsertE(e);
}
