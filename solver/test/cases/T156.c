void f()
{
    struct { int x; float y; } a;
    a.x = 1;

    struct { T1 v; T2 w; } b;
    b.v = "foo";
    b.w = 3.14;

    struct { Q q; } c;
    c.q.s.t = 3.14;
}
