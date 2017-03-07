link joinLR(link a, link b)
  { 
    if (a == z) return b;
    if (b == z) return a;
    if (rand()/(RAND_MAX/(a->N+b->N)+1) < a->N)
         { a->r = joinLR(a->r, b); return a; }
    else { b->l = joinLR(a, b->l); return b; }
  }

/* int rand(); */
