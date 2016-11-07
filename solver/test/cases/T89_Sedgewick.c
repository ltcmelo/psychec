int GRAPHpathE(Graph G, int v, int w)
  { int t;
    t = GRAPHdeg(G, v) + GRAPHdeg(G, w);
    if ((t % 2) != 0) return 0;
    for (t = 0; t < G->V; t++)
      if ((t != v) && (t != w))
        if ((GRAPHdeg(G, t) % 2) != 0) return 0;
    return 1;
  }