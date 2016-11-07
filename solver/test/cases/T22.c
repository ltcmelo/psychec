List new_node(List anchor) {
  List node = (List)malloc(12);
  node->next = anchor;
  return node;
}

List2 new_node2(List2 anchor2) {
  List2 node2 = malloc(12); // Cast is implicit.
  node2->next2 = anchor2;
  return node2;
}
