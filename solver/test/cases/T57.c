// Similar to T22, but with missing declaration of node,
// that's why it's separate (triggers different solver aspects)

List3 new_node2(List3 anchor3) {
  //  no declaration of node 3
  node3->next3 = anchor3;
  return node3;
}
