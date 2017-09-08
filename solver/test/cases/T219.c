int foobar(void (*)(void));
int barfoo(void (*)());

void f()
{
    T a = foobar;
    T2 b = barfoo;
}

