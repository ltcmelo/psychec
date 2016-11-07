typedef struct A_str {int i;}* A_ptr;
//typedef A_ptr link;
int main() {
  A_ptr a;
  link x = malloc(1);
  x = a;
}
