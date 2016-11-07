typedef struct A { float x; int y; } A;
void f() {
    A a;
    T1 i = a.x;
    A* ap;
    T2 j = ap->y;
}
