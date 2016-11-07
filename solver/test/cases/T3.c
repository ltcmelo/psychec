int main() {
    T t;
    t.i = 0;
    t.next = &t;

    T2 t2;
    t2.i = 10;
    t2.next = &t2;
}
