// The issue (simplified) from T50: scoping in domain lattice.

void NEW(link param)
{
    x->l = param;
}

link max(long param)
{
    x->l = y(param);
    return x;
}
