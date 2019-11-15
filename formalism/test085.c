node_t new_node(value_t value, node_t next)
{
  node_t node;
  node = 0;
  node->next = next;
  node->value = value;
  return node;
}
