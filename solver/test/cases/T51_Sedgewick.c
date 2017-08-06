// Sedgewick chap 5, example 20
// tree example 2

char *a; int abc;
// typedef struct Tnode* link;
// struct Tnode { char token; link l, r; };
link NEW(char token, link l, link r)
  { link x = malloc(sizeof *x); 
    x->token = token; x->l = l; x->r = r;
    return x;
  }
link parse()
  { char t = a[abc++];
    link x = NEW(t, NULL, NULL);
    if ((t == '+') || (t == '*'))
      { x->l = parse(); x->r = parse(); }
    return x;
  }