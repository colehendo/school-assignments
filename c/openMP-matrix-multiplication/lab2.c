#include <omp.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef double fp_t;

/*
 *  * prototypes
 *   */

void init2d(double *array, int size, double base){
  int i, ii, j;

  #pragma omp parallel for shared(array) private(i, ii, j)
  for(i = 0; i < size; i++){
    ii = i * size;
    for(j = 0; j < size; j++){
      array[ii + j] = base * (i + (0.1 * j));
    }
  }
}

void mxmIJK(double *m1, double *m2, double *m3, int size) {
  int i, ii, j, k;

  #pragma omp parallel for shared(m1, m2, m3) private(i, ii, j, k)
  for(i = 0; i < size; i++){
    ii = i * size;
    for(j = 0; j < size; j++){
      m3[ii + j] = 0.0;
      for(k = 0; k < size; k++){
        m3[ii + j] += m1[ii + k] * m2[(k * size) + j];
      }
    }
  }
}

void mxmIKJ(double *m1, double *m2, double *m3, int size) {
  int i, ii, k, kk, j;

  #pragma omp parallel for shared(m1, m2, m3) private(i, ii, k, kk, j)
  for(i = 0; i < size; i++){
    ii = i * size;
    for(k = 0; k < size; k++){
      kk = k * size;
      for(j = 0; j < size; j++){
        m3[ii + j] = 0.0;
        m3[ii + j] += m1[ii + k] * m2[kk + j];
      }
    }
  }
}

void mxmJKI(double *m1, double *m2, double *m3, int size) {
  int j, k, kk, i;

  #pragma omp parallel for shared(m1, m2, m3) private(j, k, kk, i)
  for(j = 0; j < size; j++){
    for(k = 0; k < size; k++){
      kk = k * size;
      for(i = 0; i < size; i++){
        m3[(i * size) + j] = 0.0;
        m3[(i * size) + j] += m1[(i * size) + k] * m2[kk + j];
      }
    }
  }
}

void mmT(double *array, int size) {
  double temp;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < i; j++){
      temp = array[i*size + j];
      array[i*size + j] = array[(j * size) + i];
      array[(j * size) + i] = temp;
    }
  }
}

void mxm2(double *m1, double *m2, double *m3, int size) {
  int i, j, k;

  mmT(m2, size);
  #pragma omp parallel for shared(m1, m2, m3) private(i, j, k)
  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      for(k = 0; k < size; k++){
        m3[(i * size) + j] += m1[(i * size) + k] * m2[(j * size) + k];
      }
    }
  }
}

double normf(double *array, int size) {
  double norm = 0.0;
  int i, j;

  #pragma omp parallel for shared(array, norm) private(i, j)
  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      norm += (fabs(array[(i * size) + j]) * fabs(array[(i * size) + j]));
    }
  }

  return sqrt(norm);
}

void print_matrix(double *array, int size) {
  for(int i = 0; i < size; i++){
    int ii = i * size;
    printf("| ");
    for(int j = 0; j < size; j++){
      printf("%0.2lf ", array[ii + j]);
    }
    printf("|\n");
  }
  double norm = normf(array, size);
  printf("norm: %0.3lf\n\n", norm);
}

int main(int argc, char **argv) {
  double start, end;
  int s;
  switch (argc) {
  case 3:
     omp_set_num_threads(atoi(argv[2]));
     s = atoi(argv[1]);
     break;
  case 2:
     omp_set_num_threads(1);
     s = atoi(argv[1]);
     break;
  default:
     omp_set_num_threads(1);
     s = 10;
  }

  double * A = malloc(s * s * sizeof(double));
  double * B = malloc(s * s * sizeof(double));
  double * C = malloc(s * s * sizeof(double));

  init2d(A, s, 1);
  init2d(B, s, 2);
  init2d(C, s, 0);

  printf("\nIJK:\n\n");
  start = omp_get_wtime();
  mxmIJK(A, B, C, s);
  end = omp_get_wtime();
  printf("Time: ");
  printf("%10.7f ms\n", (end-start)*1000.0);

  printf("\n\nIKJ:\n\n");
  start = omp_get_wtime();
  mxmIKJ(A, B, C, s);
  end = omp_get_wtime();
  printf("Time: ");
  printf("%10.7f ms\n", (end-start)*1000.0);

  printf("\n\nJKI:\n\n");
  start = omp_get_wtime();
  mxmJKI(A, B, C, s);
  end = omp_get_wtime();
  printf("Time: ");
  printf("%10.7f ms\n", (end-start)*1000.0);


  init2d(C,s,0);
  printf("\n\nTranspose:\n\n");
  start = omp_get_wtime();
  mxm2(A, B, C, s);
  end = omp_get_wtime();
  printf("Time: ");
  printf("%10.7f ms\n", (end-start)*1000.0);

  free(A);
  free(B);
  free(C);
}