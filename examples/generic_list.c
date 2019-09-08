#include "point.h"

_Generic void prepend(_Forall(node_t)** head,
                      _Forall(value_t) value)
{
    node_t* n = malloc(sizeof(node_t));
    n->value = value;
    n->next = *head;
    *head = n;
}

int main()
{
    _Exists(node_t)* ilst = 0;
    prepend(&ilst, 42);

    _Exists(node_t)* plst = 0;
    struct point_t p;
    p.x = p.y = 42;
    prepend(&plst, p);
}
