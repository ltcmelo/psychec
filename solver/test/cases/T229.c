//typedef void (*FP)();
//FP fp;

void f()
{
    if (fp)
        fp();
    if (&fp)
        fp();
}
