void x() {
    f();
    T1 at = f;
    T2 bt = *f;
    T3 ct = **f;
    T4 dt = ***f;
}

void y() {
    Q1 aq = g;
    Q2 bq = *g;
    Q3 cq = **g;
    Q4 dq = ***g;
    g();
}

void z() {
    S1 as = h;
    S2 bs = *h;
    h();
    S3 cs = **h;
    S4 ds = ***h;
}
