void f()
{
    char  *err;
    parse_user_spec = err;

    foo (parse_user_spec);
    foo (parse_additional_groups);

    if (parse_additional_groups != 0)
    {
    }
}

void f2()
{
    char const *err2;
    err2= parse_user_spec2;

    foo2 (parse_user_spec2);
    foo2 (parse_additional_groups2);

    if (parse_additional_groups2 != 0)
    {
    }
}

void f3()
{
    char const *err3 = parse_user_spec3;

    foo3 (parse_user_spec3);
    foo3 (parse_additional_groups3);

    if (parse_additional_groups3 != 0)
    {
    }
}

void f4()
{

    foo4 (parse_user_spec4);
    foo4 (parse_additional_groups4);

    char const *err4 = parse_user_spec4;

    if (parse_additional_groups4 != 0)
    {
    }
}
