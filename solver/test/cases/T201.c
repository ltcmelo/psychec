void f()
{
    char const *group;
    xstrdup (pwd ? pwd->pw_name
                 : imaxtostr (uid, buf) ? imaxtostr (uid, buf)
                                        : umaxtostr (uid, buf));
    if (user) {}
    else if (group)
    {
        xstrdup (group);
    }
}
