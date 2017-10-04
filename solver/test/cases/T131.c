Result generate(double val)
{
  List list;
  list->data = val;
  list->next = list;
  return analyse(list);
}

Result analyse(Chain chain) { /*...*/ }
