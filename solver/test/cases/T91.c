// Nothing to be inferred, just make sure we still run smoothly.
// Take a look at T38.c and T39.c too.
typedef struct A_str {int i;}* A_ptr;
typedef A_ptr link;
int main() {
  A_ptr a;
  link x = malloc(1);
  x = a;
}
