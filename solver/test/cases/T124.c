void f(enum E e) {
    e = X;
    e = Z;
    e = 10;

    switch (e) {
    case Y:
        break;
    case W:
        break;
    default:
        break;
    }
}
