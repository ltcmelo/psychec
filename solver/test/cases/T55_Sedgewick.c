// THIS IS NOT THE FULL CODE AND SEEMS TO HAVE SOME MODIFICATIONS
// Sedgewick chap 17, example 
//  example ()

Graph GRAPHinit(int V)
{ Graph G = malloc(sizeof *G);
  G->V = V; G->E = 0;
  G->adj = MATRIXint(V, V, 0);
  return G;
}
void GRAPHinsertE(Graph G, Edge e)
{ int v = e.v, w = e.w;
  if (G->adj[v][w] == 0) G->E++;
  G->adj[v][w] = 1;
  G->adj[w][v] = 1;
}
