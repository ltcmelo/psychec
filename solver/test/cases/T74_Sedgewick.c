// #include <stdio.h>
// #include <math.h>
// #include "COMPLEX.h"
main(int argc, char *argv[])
  { int i, j, N = atoi(argv[1]);
    Complex t, x;
    printf("%dth complex roots of unity\n", N);
    for (i = 0; i < N; i++)
      { float r = 2.0*3.141592625*i/N;
        t = COMPLEXinit(cos(r), sin(r));
        printf("%2d %6.3f %6.3f ", i, Re(t), Im(t));
        for (x = t, j = 0; j < N-1; j++)
          x = COMPLEXmult(t, x);
        printf("%6.3f %6.3f\n", Re(x), Im(x));
      }
 }