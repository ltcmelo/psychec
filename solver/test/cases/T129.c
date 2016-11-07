/* There are two names in scope: `struct T1' and `T1'. But cannot tell that
   they refer to the same type - in fact they might not be the case. We need
   to reconstruct a new type. */
struct T1 { int x; };
void f1() {
    T1 v1;
    v1.y = 9.9;
}

/* Now, the two names `struct T2' and `struct T2' obviously refer to the same
 * type, so we don't want reconstruct a new type, it's the one declared. */
struct T2 { double x; };
void f2() {
    struct T2 v2;
    v2.x = 9.0;
}
