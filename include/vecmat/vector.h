#ifndef VECMAT_VECTOR_H
#define VECMAT_VECTOR_H

#include <stdbool.h>

typedef struct {
    int     n;      /* dimension */
    double *data;   /* n doubles */
} Vector;

Vector create_vector(int n);
void   free_vector(Vector *v);
Vector vector_from_array(const double *array, int n);

void   print_vector(const Vector *v);

Vector add_vector(const Vector *a, const Vector *b);
Vector sub_vector(const Vector *a, const Vector *b);
Vector scale_vector(const Vector *a, double c);

double vector_dot(const Vector *a, const Vector *b);

double vector_norm_L1(const Vector *v);         /* sum of absolute values */
double vector_norm_L2(const Vector *v);         /* Euclidean norm */
double vector_norm_Linf(const Vector *v);       /* max absolute value */

Vector normalize_vector(const Vector *a);

/* Linear independence: check if a set of vectors can be linearly combined
 * (other than trivially) to produce zero. Returns true if dependent, false if independent. */
bool is_vector_set_linearly_dependent(Vector *vectors[], int number_of_vectors);

#endif
