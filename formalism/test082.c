int f()
{
  T1 r;
  r->m = 42;
  r->n = r;
  return 0;
}
