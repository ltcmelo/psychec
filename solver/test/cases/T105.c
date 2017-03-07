// Just like 104.c, but the function name is `new', which is a keyword in C++
// but not in C.

void new(set* s, unsigned capacity) {
  s->vector = (unsigned*)malloc((1 + capacity / INT_BITS) * sizeof(unsigned));
  s->size = capacity;
}
