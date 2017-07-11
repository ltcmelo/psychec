
void g()
{
    // Function printf is annotated so we can extract type informatin from the format
    // specifiers. There are more constraints than in a non-annotated variadic function.
    printf("bar %d", num);
    printf("foo %s", str);
}
