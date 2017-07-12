Result generate(double val)
{
    List list;            /* No declaration of `List' available. */
    list = 0;             /* `List' is a pointer or an integral. */
    list = make();        /* Function `make' must return a `List'. */
    list->data = val;     /* List has a member of type `double'. */
    list->next = list;    /* And it's a recursive type. */
    if (analyse(list))    /* Function `analyse' must take a `List'. */
        return "OK";      /* We got `generate's return type. */
    return FAIL_MACRO(1); /* Unexpanded macro, but syntactically valid. */
}
