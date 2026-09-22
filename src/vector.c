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

void free_vector(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->n = 0;
}


Vector vector_from_array(const double *array, int n) {
    Vector v = create_vector(n);
    memcpy(v.data, array, n * sizeof(double));
    return v;
}

void print_vector(const Vector *v) {
    printf("(");
    for(int i = 0; i < v->n; i++) {
        double value = fabs(v->data[i]) < 0.0005 ? 0.0 : v->data[i];
        printf("%.3f%s", value, (i < v->n - 1) ? ", " : "");
    }
    printf(")\n");
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

double vector_norm_L1(const Vector *v) {
    double sum = 0.0;
    for (int i = 0; i < v->n; i++) {
        sum += fabs(v->data[i]);
    }
    return sum;
}

double vector_norm_L2(const Vector *v) {
    return sqrt(vector_dot(v, v)); // ||v|| = sqrt(v . v)
}

double vector_norm_Linf(const Vector *v) {
    double max_abs = 0.0;
    for (int i = 0; i < v->n; i++) {
        double abs_val = fabs(v->data[i]);
        if (abs_val > max_abs) {
            max_abs = abs_val;
        }
    }
    return max_abs;
}

Vector normalize_vector(const Vector *a) {
    double length = vector_norm_L2(a);
    if (length == 0.0) {
        fprintf(stderr, "normalize_vector: zero vector has no direction");
        exit(1);
    }
    return scale_vector(a, 1.0 / length);
}

/*
 * Linear independence check using Gram-Schmidt orthogonalization.
 * If at any step a vector becomes zero after removing its projections
 * onto the previous vectors, the set is linearly dependent.
 * Otherwise it's independent.
 */
bool is_vector_set_linearly_dependent(Vector *vectors[], int number_of_vectors) {
    double tolerance = 1e-10;
    if (number_of_vectors == 0) return false;
    if (number_of_vectors == 1) {
        double n = vector_norm_L2(vectors[0]);
        return n < tolerance; // numerical tolerance
    }

    Vector *workspace = (Vector *) malloc((size_t)number_of_vectors * sizeof(Vector));
    for (int i = 0; i < number_of_vectors; i++) {
        workspace[i] = vector_from_array(vectors[i]->data, vectors[i]->n);
    }

    for (int i = 0; i < number_of_vectors; i++) {
        for (int j = 0; j < i; j++) {
            double project_coefficient = vector_dot(&workspace[i], &workspace[j]) /
                                         vector_dot(&workspace[j], &workspace[j]);
            Vector projected_vector = scale_vector(&workspace[j], project_coefficient);
            Vector new_vi = sub_vector(&workspace[i], &projected_vector);
            free_vector(&workspace[i]);
            workspace[i] = new_vi;
            free_vector(&projected_vector);
        }

        double norm_i = vector_norm_L2(&workspace[i]);
        if (norm_i < tolerance) {
            for (int k = 0; k < number_of_vectors; k++) {
                free_vector(&workspace[k]);
            }
            return true;
        }
    }

    for (int k = 0; k < number_of_vectors; k++) {
        free_vector(&workspace[k]);
    }

    free(workspace);

    return false;
}
