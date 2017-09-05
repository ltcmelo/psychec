struct TUPLE split(NODE_PTR a) {
    struct TUPLE x = split(a->next);
    return x;
}

void merge_sort(NODE_PTR c)
{
    struct TUPLE s = split(c);
}
