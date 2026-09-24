#ifndef VECMAT_MATRIX_H
#define VECMAT_MATRIX_H

#include "vector.h"

typedef struct {
    int    rows, cols;
    double *data;
} Matrix;

Matrix create_matrix(int rows, int cols);
void   free_matrix(Matrix *mat);

Matrix matrix_identity(int n);

double get_matrix(const Matrix *m, int i, int j);
void   set_matrix(Matrix *m, int i, int j, double val);
void   print_matrix(const Matrix *m);

Matrix matrix_add(const Matrix *a, const Matrix *b);
Matrix matrix_scale(const Matrix *a, double c);
Matrix matrix_transpose(const Matrix *m);

Vector matrix_vector_multiplication(const Matrix *a, const Vector *v);
Matrix matrix_multiplication(const Matrix *a, const Matrix *b);

#endif