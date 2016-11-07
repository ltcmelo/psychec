// example 1
m() {} // gen alpha = int

// example 2
void h() { /* no ret */ } // alpha = void

// example 3
int f() { /* no ret */ } // alpha = int

// example 4
Foo g() { /* no ret */ } // alpha = Foo and foo is orphan => uintptr_t = Foo

T3 a() {}
void b() { T3 x = 9.9; }