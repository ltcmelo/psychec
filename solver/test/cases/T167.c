void f(char *name, const char *suffix) {
    len(name);
    len(suffix);
}

void f3(const char *suffix3, char *name3) {
    len3(name3);
    len3(suffix3);
}

void f2() {
    char *name2 = 0;
    const char *suffix2 = 0;
    len2(name2);
    len2(suffix2);
}
