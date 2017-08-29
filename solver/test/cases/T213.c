NODE_PTR make(int v) {
    NODE_PTR n = malloc(sizeof(NODE));
    n->next = 0;
    n->value = v;
    return n;
}

int main(int argc, char* argv[])
{
    NODE_PTR head = make(0);
    NODE_PTR node = head;
    for (int i = 0; i < atoi(argv[1]); ++i) {
        node->next = make(atoi(argv[i + 2]));
        node = node->next;
    }
}
