#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "vecmat/vector.h"

Vector create_vector(int n) {
    Vector v;
    v.n = n;
    v.data = (double *) calloc(n, sizeof(double));
    if(!v.data) {
        fprintf(stderr, "create_vector: allocation failed\n");
        exit(1);
    }
    return v;
}

Vector vector_from_array(const double *array, int n) {
    Vector v = create_vector(n);
    memcpy(v.data, array, n * sizeof(double));
    return v;
}

static void check_dimensions(const Vector *a, const Vector *b, const char *operation) {
    if (a->n != b->n) {
        fprintf(stderr, "%s: dimension mismatch (%d vs %d)\n", operation, a->n, b->n);
        exit(1);
    }
}

Vector add_vector(const Vector *a, const Vector *b) {
    check_dimensions(a, b, "add_vector");
    Vector c = create_vector(a->n);
    for (int i = 0; i < a->n; i++) {
        c.data[i] = a->data[i] + b->data[i];
    }
    return c;
}

Vector sub_vector(const Vector *a, const Vector *b) {
    check_dimensions(a, b, "sub_vector");
    Vector c = create_vector(a->n);
    for (int i = 0; i < a->n; i++) {
        c.data[i] = a->data[i] - b->data[i];
    }
    return c;
}

Vector scale_vector(const Vector *a, double c) {
    Vector scaled = create_vector(a->n);
    for(int i = 0; i < a->n; i++) {
        scaled.data[i] = a->data[i] * c;
    }
    return scaled;
}

double vector_dot(const Vector *a, const Vector *b) {
    check_dimensions(a, b, "vector_dot");
    double sum = 0.0;
    for(int i = 0; i < a->n; i++) {
        sum += a->data[i] * b->data[i];
    }
    return sum;
}

double vector_norm(const Vector *a) {
    return sqrt(vector_dot(a, a)); // ||v|| = sqrt(v x v)
}

Vector normalize_vector(const Vector *a) {
    double length = vector_norm(a);
    if (length == 0.0) {
        fprintf(stderr, "normalize_vector: zero vector has no direction");
        exit(1);
    }
    return scale_vector(a, 1.0 / length);
}

void print_vector(const Vector *v) {
    printf("(");
    for(int i = 0; i < v->n; i++) {
        double value = fabs(v->data[i]) < 0.0005 ? 0.0 : v->data[i];
        printf("%.3f%s", value, (i < v->n - 1) ? ", " : "");
    }
    printf(")\n");
}

void free_vector(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->n = 0;
}
