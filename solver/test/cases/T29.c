INT multiply(INT *a, INT *b, INT size) {
  INT result = 0;
  for (INT i=0; i < size; i++) {
      result += a[i] * b[i];
  }
  return result;
}