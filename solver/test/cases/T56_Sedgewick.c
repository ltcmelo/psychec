// THIS IS NOT THE FULL CODE
// Sedgewick chap 17, example 10
// tree example 

void STinit()
{ head = NULL; N = 0; }
link stNEW(int d)
{ link x = malloc(sizeof *x);
  x->index = -1; x->d = d;
  x->l = NULL; x->m = NULL; x->r = NULL;
  return x;
}
