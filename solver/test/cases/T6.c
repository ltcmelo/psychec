int foo(D d)
{
  return 2 * d;
}

int function(C c) 
{
  D d = c.x + c.y;
  A a = foo(d);
  return a;
}
