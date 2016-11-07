static int visited[10];
int pathR(Graph G, int v, int w, int d)
  { int t; 
    if (v == w) 
      { if (d == 0) return 1; else return 0; }
    visited[v] = 1; 
    for (t = 0; t < G->V; t++)
      if (G->adj[v][t] == 1)
        if (visited[t] == 0)
          if (pathR(G, t, w, d-1)) return 1;
    visited[v] = 0;
    return 0;
  }
int GRAPHpathH(Graph G, int v, int w)
  { int t; 
    for (t = 0; t < G->V; t++) visited[t] = 0;
    return pathR(G, v, w, G->V-1);
  }