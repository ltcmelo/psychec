void f() { g(); }

void h() {
    g();
    i();
}

void i();

void k() { j(); m(); n(); }

static void g() {}

void j() {}

static void n() {}

