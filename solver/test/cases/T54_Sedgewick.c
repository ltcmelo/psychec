// Sedgewick chap 17, example 10
// STnode* example (tree3)

//#include <stdlib.h>
//typedef struct STnode* link;
//struct STnode { int index, d; link l, m, r; };
static link head;
static int val, N; 
void STinit() 
  { head = NULL; N = 0; }
link stNEW(int d)
  { link x = malloc(sizeof *x);   
    x->index = -1; x->d = d; 
    x->l = NULL; x->m = NULL; x->r = NULL;
    return x;
  }
link indexR(link h, char* v, int w)
  { int i = v[w];
    if (h == NULL) h = stNEW(i); 
    if (i == 0) 
      { 
        if (h->index == -1) h->index = N++;
        val = h->index;
        return h;
      }
    if (i < h->d) h->l = indexR(h->l, v, w);
    if (i == h->d) h->m = indexR(h->m, v, w+1);
    if (i > h->d) h->r = indexR(h->r, v, w);
    return h;
  }
int STindex(char* key)
  { head = indexR(head, key, 0); return val; }