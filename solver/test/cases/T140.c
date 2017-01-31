void x() {
    T1 at = f;
    T2 bt = *f;
    T3 ct = **f;
    T4 dt = ***f;
    at();
}

void y() {
    Q1 aq = g;
    Q2 bq = *g;
    Q3 cq = **g;
    Q4 dq = ***g;
    dq();
}

void z() {
    S1 as = h;
    S2 bs = *h;
    bs();
    S3 cs = **h;
    S4 ds = ***h;
}
