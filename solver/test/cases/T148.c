void f() {
    T1 a;
    T2 b;
    b.y = 10;
    a.x = b;
}

void g() {
    T3 c;
    T4 d;
    d.w = c;
    c.z = 10;
}

void h() {
    T5 m;
    T6 n;
    n.yy = 10;
    m->xx = n;
}

void i() {
    T7 p;
    T8 q;
    q->ww = p;
    p.zz = 10;
}
