void new_(set* s, unsigned capacity) {
  s->vector = (unsigned*)malloc((1 + capacity / INT_BITS) * sizeof(unsigned));
  s->size = capacity;
}

void new2_(set2* s2, unsigned int capacity2) {
  s2->vector2 = (unsigned int*)malloc((1 + capacity2 / INT_BITS2) * sizeof(unsigned int));
  s2->size2 = capacity2;
}
