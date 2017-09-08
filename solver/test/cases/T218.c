struct A
{
   unsigned x:3,
            y:4,
             :2;
};

struct B
{
    unsigned w:2;
    unsigned  :6;
};

void f()
{
    struct A a;
    T1 v1 = a;
    struct B b;
    T2 v2 = b;
}
