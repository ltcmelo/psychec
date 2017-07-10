// #include <math.h>
// #include <stdlib.h>
// typedef int numType;
//#define R 1000
numType randNum()
  { return rand() % R; }
main(int argc, char *argv[])
  { int i, N = atoi(argv[1]);
    int *f = malloc(R*sizeof(int));
    float m1 = 0.0, m2 = 0.0, t = 0.0;
    numType x;
    for (i = 0; i < R; i++) f[i] = 0;
    for (i = 0; i < N; i++)
      {
        f[x = randNum()]++;
        m1 += (float) x/N; 
        m2 += (float) x*x/N;
      }
    for (i = 0; i < R; i++) t += f[i]*f[i];
    printf("       Average: %f\n", m1);
    printf("Std. deviation: %f\n", sqrt(m2-m1*m1));
    printf("    Chi-square: %f\n", (R*t/N)-N);
 }
