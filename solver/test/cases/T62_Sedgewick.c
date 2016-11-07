//#include <stdlib.h>
//typedef int numType;
numType randNum()
  { return rand(); }
int main(int argc, char *argv[])
  { int i, N = atoi(argv[1]);
    float m1 = 0.0, m2 = 0.0;
    numType x;
    for (i = 0; i < N; i++)
      {
        x = randNum();
        m1 += ((float) x)/N; 
        m2 += ((float) x*x)/N;
      }
    printf("       Average: %f\n", m1);
    printf("Std. deviation: %f\n", sqrt(m2-m1*m1));
 }
