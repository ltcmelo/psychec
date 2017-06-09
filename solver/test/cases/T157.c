void f()
{
    union O orphan;
}

void g()
{
    union U u;
    u.w = 1;

    struct ST st;
    st.ww = 1;
}

union P { int xx; };

void h() {
    union P p;
    p.xx = 1;
}

union Q { T x; };

void k() {
    union Q q;
    q.x = 3.14;
}

typedef union S { double y; } S;

void i() {
    S s;
    s.y = 3.14;
}

typedef union J { TT z; } J;

void m() {
    J j;
    j.z = "foo";
}
