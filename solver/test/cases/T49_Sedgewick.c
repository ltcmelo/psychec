// Sedgewick chap 4, example 23
// poly example
// Renamed exp -> expo from the original, so clang/gcc doesn't complain that exp
// is a function (from std lib) and trigger an error.

Poly POLYterm(int coeff, int expo)
  { int i; Poly t = malloc(sizeof *t);
    t->a = malloc((expo+1)*sizeof(int));
    t->N = expo+1; t->a[expo] = coeff;
    for (i = 0; i < expo; i++) t->a[i] = 0;
    return t;
  }    
Poly POLYadd(Poly p, Poly q)
  { int i; Poly t;
    if (p->N < q->N) { t = p; p = q; q = t; }
    for (i = 0; i < q->N; i++) p->a[i] += q->a[i];
    return p;
  }
Poly POLYmult(Poly p, Poly q)
  { int i, j;
    Poly t = POLYterm(0, (p->N-1)+(q->N-1));
    for (i = 0; i < p->N; i++) 
      for (j = 0; j < q->N; j++) 
        t->a[i+j] += p->a[i]*q->a[j];
    return t;
  }
float POLYeval(Poly p, float x)
  { int i; double t = 0.0;
    for (i = p->N-1; i >= 0; i--) 
      t = t*x + p->a[i];
    return t;
  }
