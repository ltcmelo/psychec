T fun()
{
    int* ip;
    const int* cip;
    T1 a;
    T2 b;
    T3 c;
    T4 d;
    T5 e;
    T6 f;
    T7 g;
    T8 h;
    T9 i;

    a = cip;
    b = ip;
    
    c = b;
    d = c;
    e = d;
    f = e;
    
    g = a;
    h = g;
    i = h;
    i = e;


    return 0;
}
