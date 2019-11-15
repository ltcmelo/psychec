T f()
{
    T1 rec;
    rec->x = 42;
    rec->next = rec;
    return rec->x;
}
