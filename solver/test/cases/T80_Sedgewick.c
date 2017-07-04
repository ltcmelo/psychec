//typedef struct STnode* link;
//struct STnode { Item item; link* next; int sz; };
static link head, z;
static int N, lgN;
link NEW(Item item, int k)      
  { int i; link x = malloc(sizeof *x);
    x->next = malloc(k*sizeof(link));
    x->item = item; x->sz = k;
    for (i = 0; i < k; i++) x->next[i] = z;
    return x;                         
  }                                   
void STinit(int max) 
  { 
    N = 0; lgN = 0; 
    z = NEW(NULLitem, 0); 
    head = NEW(NULLitem, lgNmax); 
  }
