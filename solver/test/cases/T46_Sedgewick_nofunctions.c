int scanf(char* str, int* p1, int* p2) {
  return 0;
}
link NEW(int v, link next)
  { link x = malloc(sizeof *x);
    x->v = v; x->next = next;     
    return x;                         
  }
main()
  { int i, j; link adj[V];
    for (i = 0; i < V; i++) adj[i] = NULL;
    while (scanf("%d %d\n", &i, &j) == 2)
      {
        adj[j] = NEW(i, adj[j]);
        adj[i] = NEW(j, adj[i]);
      }
  }