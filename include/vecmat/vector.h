#ifndef VECMAT_VECTOR_H
#define VECMAT_VECTOR_H

typedef struct {
    int     n;      /* dimension */
    double *data;   /* n doubles */
} Vector;

Vector create_vector(int n);
Vector vector_from_array(const double *array, int n);

Vector add_vector(const Vector *a, const Vector *b);
Vector sub_vector(const Vector *a, const Vector *b);
Vector scale_vector(const Vector *a, double c);
Vector normalize_vector(const Vector *a);

double vector_dot(const Vector *a, const Vector *b);
double vector_norm(const Vector *a);

void   free_vector(Vector *v);
void   print_vector(const Vector *v);

#endif
