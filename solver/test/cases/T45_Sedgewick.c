// Sedgewick chap 3, example 14
// list1 example

// typedef struct node* link;
// struct node { itemType item; link next; };
link freelist;
void initNodes(int N)
  { int i;
    freelist = malloc((N+1)*(sizeof *freelist));
    for (i = 0; i < N+1; i++)
      freelist[i].next = &freelist[i+1];
    freelist[N].next = NULL;
  }    
link newNode(int i)
  { link x = deleteNext(freelist); 
    x->item = i; x->next = x;
    return x;
  }
void freeNode(link x)
  { insertNext(freelist, x); }
void insertNext(link x, link t)
  { t->next = x->next; x->next = t; }
link deleteNext(link x)
  { link t = x->next; x->next = t->next; return t; }
link Next(link x)
  { return x->next; }
int Item(link x)
  { return x->item; }