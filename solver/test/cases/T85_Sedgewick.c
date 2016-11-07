Graph GRAPHrand(int V, int E)
  { int i, j;
    double p = 2.0*E/V/(V-1);
    Graph G = GRAPHinit(V);
    for (i = 0; i < V; i++)
      for (j = 0; j < i; j++)
        if (rand() < p*RAND_MAX)
          GRAPHinsertE(G, EDGE(i, j));
    return G;
  }