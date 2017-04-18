void f()
{
    /*
        We treat void* as a top type. Below:
            - T1 can be void* or a pointer to an orphan type.
            - T2 can be void or an orphan type.
        When a named type stands for a void*, entirely, we make it as such. Otherwise,
        the pointer-declarator appears in the declaration, we make a pointer to orphan.
     */
    T1 a = malloc(4);
    T2* b = malloc(4);
}
