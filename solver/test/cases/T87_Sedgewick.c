static int visited[10];
int pathR(Graph G, int v, int w)
  { int t; 
    if (v == w) return 1;
    visited[v] = 1;
    for (t = 0; t < G->V; t++)
      if (G->adj[v][t] == 1)
        if (visited[t] == 0)
          if (pathR(G, t, w)) return 1;
    return 0;
  }
int GRAPHpath(Graph G, int v, int w)
  { int t; 
    for (t = 0; t < G->V; t++) visited[t] = 0;
    return pathR(G, v, w);
  }