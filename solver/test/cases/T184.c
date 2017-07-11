void f()
{
    const char* name;
    if (*name)
    {
        quote (name);
        quote ("foo");
    }
}

void f2()
{
    char* name2;
    if (*name2)
    {
        quote2 (name2);
        quote2 ("foo");
    }
}
