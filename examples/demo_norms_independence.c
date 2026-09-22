#include <stdio.h>
#include <math.h>
#include "vector.h"
 
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void print_all_norms(const Vector *v) {
    double l1   = vector_norm_L1(v);
    double l2   = vector_norm_L2(v);
    double linf = vector_norm_Linf(v);
    printf("||v||1 = %.3f   (sum of abs values)\n", l1);
    printf("||v||2 = %.3f   (Euclidean, sqrt of sum of squares)\n", l2);
    printf("||v||INF = %.3f   (max absolute value)\n", linf);
}

void demo_three_norms(void) {
    printf("=== Three norms, same vector ===\n");
 
    double v_arr[3] = { 3.0, 4.0, -2.0 };
    Vector v = vector_from_array(v_arr, 3);
 
    printf("v = (3, 4, -2)\n");
    print_all_norms(&v);
 
    printf("Note: L2 = sqrt(3^2 + 4^2 + 2^2) = sqrt(29) = ~5.385\n");
    printf("      L1 = |3| + |4| + |2| = 9\n");
    printf("      LINF = max(|3|, |4|, |2|) = 4\n");
    printf("Same vector, three different answers. Choice depends on the context.\n\n");
 
    free_vector(&v);
}

void demo_unit_balls_2d(void) {
    printf("=== Unit balls in 2D (where ||v|| = 1) ===\n");
 
    int grid_size = 25;
    double center = (grid_size - 1) / 2.0;
 
    char grids[3][25][25];
 
    /* Generate the three unit balls */
    for (int n = 0; n < 3; n++) {
        for (int r = 0; r < grid_size; r++) {
            for (int c = 0; c < grid_size; c++) {
                grids[n][r][c] = ' ';
            }
        }
 
        for (int r = 0; r < grid_size; r++) {
            for (int c = 0; c < grid_size; c++) {
                double x = (c - center) / 8.0;
                double y = (r - center) / 8.0;
 
                double norm_val;
                if (n == 0) {
                    norm_val = fabs(x) + fabs(y); /* L1 */
                } else if (n == 1) {
                    norm_val = sqrt(x*x + y*y); /* L2 */
                } else {
                    norm_val = fmax(fabs(x), fabs(y)); /* LINF */
                }
 
                if (norm_val <= 1.01) grids[n][r][c] = '*';
                if (r == center && c == center) grids[n][r][c] = '+'; /* origin */
            }
        }
    }
 
    const char *names[3] = { "L1 ball (diamond)", "L2 ball (circle)", "LINF ball (square)" };
    for (int n = 0; n < 3; n++) {
        int min_row = grid_size;
        int max_row = -1;
        int min_col = grid_size;
        int max_col = -1;

        for (int r = 0; r < grid_size; r++) {
            for (int c = 0; c < grid_size; c++) {
                if (grids[n][r][c] != ' ') {
                    if (r < min_row) min_row = r;
                    if (r > max_row) max_row = r;
                    if (c < min_col) min_col = c;
                    if (c > max_col) max_col = c;
                }
            }
        }

        printf("%s:\n", names[n]);
        for (int r = min_row; r <= max_row; r++) {
            for (int c = min_col; c <= max_col; c++) {
                putchar(grids[n][r][c]);
                if (grids[n][r][c] != '+') putchar(grids[n][r][c]);
            }
            putchar('\n');
        }
        if (n < 2) printf("\n");
    }
}
 
void demo_linear_independence(void) {
    printf("=== Linear Independence ===\n");
 
    printf("Test 1: (1, 0) and (0, 1) in R^2\n");
    double v1_arr[2] = {1, 0}, v2_arr[2] = {0, 1};
    Vector v1 = vector_from_array(v1_arr, 2);
    Vector v2 = vector_from_array(v2_arr, 2);
    Vector *set1[] = {&v1, &v2};
    printf("  Result: %s\n", is_vector_set_linearly_dependent(set1, 2) ? "DEPENDENT" : "INDEPENDENT");
    printf("  (They span a 2D plane; neither is a scalar multiple of the other.)\n\n");
 
    printf("Test 2: (1, 2) and (2, 4) in R^2\n");
    double v3_arr[2] = {1, 2}, v4_arr[2] = {2, 4};
    Vector v3 = vector_from_array(v3_arr, 2);
    Vector v4 = vector_from_array(v4_arr, 2);
    Vector *set2[] = {&v3, &v4};
    printf("  Result: %s\n", is_vector_set_linearly_dependent(set2, 2) ? "DEPENDENT" : "INDEPENDENT");
    printf("  (v4 = 2*v3, so v4 is redundant.)\n\n");
 
    printf("Test 3: (1, 1), (1, -1), (2, 0) in R^2\n");
    double v5_arr[2] = {1, 1}, v6_arr[2] = {1, -1}, v7_arr[2] = {2, 0};
    Vector v5 = vector_from_array(v5_arr, 2);
    Vector v6 = vector_from_array(v6_arr, 2);
    Vector v7 = vector_from_array(v7_arr, 2);
    Vector *set3[] = {&v5, &v6, &v7};
    printf("  Result: %s\n", is_vector_set_linearly_dependent(set3, 3) ? "DEPENDENT" : "INDEPENDENT");
    printf("  (v7 = 0.5*v5 + 0.5*v6, so it's a linear combination of the first two.)\n\n");
 
    printf("Test 4: (1, 0, 0), (0, 1, 0), (0, 0, 1) in R^3 (standard basis)\n");
    double v8_arr[3] = {1, 0, 0}, v9_arr[3] = {0, 1, 0}, v10_arr[3] = {0, 0, 1};
    Vector v8 = vector_from_array(v8_arr, 3);
    Vector v9 = vector_from_array(v9_arr, 3);
    Vector v10 = vector_from_array(v10_arr, 3);
    Vector *set4[] = {&v8, &v9, &v10};
    printf("  Result: %s\n", is_vector_set_linearly_dependent(set4, 3) ? "DEPENDENT" : "INDEPENDENT");
    printf("  (Standard basis vectors are mutually orthogonal; all are needed.)\n\n");
 
    free_vector(&v1); free_vector(&v2); free_vector(&v3); free_vector(&v4);
    free_vector(&v5); free_vector(&v6); free_vector(&v7);
    free_vector(&v8); free_vector(&v9); free_vector(&v10);
}
 
int main(void) {
    demo_three_norms();
    demo_unit_balls_2d();
    demo_linear_independence();
    return 0;
}