int randV(Graph G)
  { return G->V * (rand() / (RAND_MAX + 1.0)); }
Graph GRAPHrand(int V, int E)
  { Graph G = GRAPHinit(V);
    while (G->E < E)
      GRAPHinsertE(G, EDGE(randV(G), randV(G)));
    return G;
  }