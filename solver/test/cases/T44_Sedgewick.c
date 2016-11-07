// Sedgewick chap 4, example 10 
// QUEUE example

// #include <stdlib.h>
// #include "Item.h"
// #include "QUEUE.h"
// typedef struct QUEUEnode* link;
// struct QUEUEnode { Item item; link next; };
static link head, tail;
link NEW(Item item, link next)      
  { link x = malloc(sizeof *x);
    x->item = item; x->next = next;     
    return x;                         
  }                                   
void QUEUEinit(int maxN) 
  { head = NULL; }
int QUEUEempty()
  { return head == NULL; }
void QUEUEput(Item item)
  { 
    if (head == NULL)
      { head = (tail = NEW(item, head)); return; }
    tail->next = NEW(item, tail->next); 
    tail = tail->next;
  }
Item QUEUEget()
  { Item item = head->item;
    link t = head->next; 
    free(head); head = t;
    return item;
  }
