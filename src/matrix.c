#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (double *) calloc((size_t) rows * cols, sizeof(double));

    if (!m.data) {
        fprintf(stderr, "create_matrix: allocation failed\n");
        exit(1);
    }
    return m;
}

void free_matrix(Matrix *m) {
    free(m->data);
    m->data = NULL;
    m->rows = m->cols = 0;
}

Matrix matrix_identity(int n) {
    Matrix m = create_matrix(n, n);
    for (int i = 0; i < n; i++) set_matrix(&m, i, i, 1.0);
    return m;
}

double get_matrix(const Matrix *m, int i, int j) {
    return m->data[i * m->cols + j];
}

void set_matrix(Matrix *m, int i, int j, double val) {
    m->data[i * m->cols + j] = val;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        printf("[ ");
        for (int j = 0; j < m->cols; j++) {
            printf("%7.3f ", get_matrix(m, i, j));
        }
        printf("]\n");
    }
}

Matrix matrix_add(const Matrix *a, const Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "matrix_add: dimension mismatch\n");
        exit(1);
    }
    Matrix result = create_matrix(a->rows, a->cols);
    for (int i = 0; i < a->rows * a->cols; i++) {
        result.data[i] = a->data[i] + b->data[i];
    }
    return result;
}

Matrix matrix_scale(const Matrix *m, double c) {
    Matrix result = create_matrix(m->rows, m->cols);
    for(int i = 0; i < m->rows * m->cols; i++) {
        result.data[i] = m->data[i] * c;
    }
    return result;
} 

Matrix matrix_transpose(const Matrix *m) {
    Matrix result = create_matrix(m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            set_matrix(&result, j, i, get_matrix(m, i, j));
        }
    }
    return result;
}

Vector matrix_vector_multiplication(const Matrix *m, const Vector *v) {
    if (m->cols != v->n) {
        fprintf(stderr, "matrix_vector_multiplication: dimension mismatch (%d cols vs vector of %d)\n",
                m->cols, v->n);
        exit(1);
    }
    Vector result = create_vector(m->rows);
    for (int i = 0; i < m->rows; i++) {
        double sum = 0.0;
        for (int j = 0; j < m->cols; j++) {
            sum += get_matrix(m, i, j) * v->data[j];
        }
        result.data[i] = sum;
    }
    return result;
}

Matrix matrix_multiplication(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        fprintf(stderr, "matrix_multiplication: dimension mismatch (%dx%d) * (%dx%d)\n",
                a->rows, a->cols, b->rows, b->cols);
        exit(1);
    }
    Matrix result = create_matrix(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += get_matrix(a, i, k) * get_matrix(b, k, j);
            }
            set_matrix(&result, i, j, sum);
        }
    }
    return result;
}