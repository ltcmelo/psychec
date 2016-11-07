// Sedgewick chap 4, example 5 
// Stack example

// #include <stdlib.h>
// #include "Item.h"
// typedef struct STACKnode* link;
// struct STACKnode { Item item; link next; };
static link head;
link NEW(Item item, link next)      
  { link x = malloc(sizeof *x);
    x->item = item; x->next = next;     
    return x;                         
  }                                   
void STACKinit(int maxN) 
  { head = NULL; }
int STACKempty()
  { return head == NULL; }
STACKpush(Item item)
  { head = NEW(item, head); }
Item STACKpop()
  { Item item = head->item;
    link t = head->next; 
    free(head); head = t;
    return item;
  }