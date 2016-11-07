void pathEshow(Graph G, int v, int w)
  { int t; 
    if ((v == w) && (G->E == 0)) return;
    for (t = 0; t < G->V; t++)
      if (G->adj[v][t] != 0) 
        {
          GRAPHremoveE(G, EDGE(v, t));
          if (GRAPHiso(G, v) || GRAPHpath(G, t, v))
            {
              printf("%d-%d\n", v, t); 
              pathEshow(G, t, w);
              GRAPHinsertE(G, EDGE(v, t));
              return; 
            }
          GRAPHinsertE(G, EDGE(v, t));
        }
  }