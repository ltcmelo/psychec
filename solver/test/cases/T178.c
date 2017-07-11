int f()
{
    error (0, "foo");

    error (EXIT_FAILURE, 3.14);

    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
