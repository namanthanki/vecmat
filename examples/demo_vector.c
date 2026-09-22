#include <stdio.h>
#include <math.h>
#include "vecmat/vector.h"
 
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
 
#define GRID_W 60
#define GRID_H 20
 
void print_worked_example(void) {
    double u_arr[2] = {2.0, 1.0};
    double v_arr[2] = {-1.0, 2.0};
    Vector u = vector_from_array(u_arr, 2);
    Vector v = vector_from_array(v_arr, 2);
 
        printf("+------------------------------------------------------------+\n");
        printf("| Vector relationships                                       |\n");
        printf("| u = (2, 1), v = (-1, 2)                                    |\n");
        printf("+------------------------------------------------------------+\n\n");
 
        printf("  u                 = "); print_vector(&u);
        printf("  v                 = "); print_vector(&v);
 
    Vector sum = add_vector(&u, &v);
        printf("  u + v             = "); print_vector(&sum);
 
    double dot = vector_dot(&u, &v);
        printf("  dot(u, v)         = %.3f  (%s)\n", dot,
            dot == 0.0 ? "orthogonal" : "not orthogonal");
 
    double nu = vector_norm_L2(&u);
    double nv = vector_norm_L2(&v);
    printf("  ||u||             = %.3f\n", nu);
    printf("  ||v||             = %.3f\n", nv);
 
    /* angle from u.v = ||u|| ||v|| cos(theta) */
    double cos_theta = dot / (nu * nv);
    double theta_deg = acos(cos_theta) * 180.0 / M_PI;
        printf("  angle(u, v)       = %.2f degrees  [cos(theta) = %.5f]\n",
            theta_deg, cos_theta);
 
    /* projection of u onto v: (u.v / v.v) * v */
    double vv = vector_dot(&v, &v);
    Vector proj = scale_vector(&v, dot / vv);
    printf("  projection of u   = "); print_vector(&proj);
 
    Vector u_hat = normalize_vector(&u);
    printf("  unit vector of u  = "); print_vector(&u_hat);
 
    free_vector(&u); free_vector(&v); free_vector(&sum); free_vector(&proj); free_vector(&u_hat);
    printf("\n");
}
 
void plot(char grid[GRID_H][GRID_W], double x, double y) {
    int gx = (int)round(x);
    int gy = GRID_H - 1 - (int)round(y); /* flip so up is up */
    if (gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H) {
        grid[gy][gx] = '*';
    }
}
 
void simulate_projectile(double speed, double angle_deg) {
        printf("+------------------------------------------------------------+\n");
        printf("| Projectile trajectory                                      |\n");
        printf("| launch speed: %5.1f units/s    angle: %5.1f degrees        |\n",
            speed, angle_deg);
        printf("+------------------------------------------------------------+\n\n");
 
    char grid[GRID_H][GRID_W];
    for (int r = 0; r < GRID_H; r++)
        for (int c = 0; c < GRID_W; c++)
            grid[r][c] = (r == GRID_H - 1) ? '_' : ' ';
 
    double angle_rad = angle_deg * M_PI / 180.0;
    double vel_arr[2] = { speed * cos(angle_rad), speed * sin(angle_rad) };
    double pos_arr[2] = { 0.0, 0.0 };
    double grav_arr[2] = { 0.0, -9.8 };
 
    Vector position = vector_from_array(pos_arr, 2);
    Vector velocity = vector_from_array(vel_arr, 2);
    Vector gravity  = vector_from_array(grav_arr, 2);
 
    double dt = 0.1;
    int step = 0;
 
    /* Stop once it comes back down to ground level (y <= 0), after having left it. */
    while (step < 500) {
        plot(grid, position.data[0], position.data[1]);
 
        Vector g_dt = scale_vector(&gravity, dt);
        Vector new_velocity = add_vector(&velocity, &g_dt);
        free_vector(&velocity);
        velocity = new_velocity;
        free_vector(&g_dt);
 
        Vector v_dt = scale_vector(&velocity, dt);
        Vector new_position = add_vector(&position, &v_dt);
        free_vector(&position);
        position = new_position;
        free_vector(&v_dt);
 
        step++;
        if (step > 3 && position.data[1] <= 0.0) break; /* landed */
    }
 
    printf("  y ^\n");
    for (int r = 0; r < GRID_H; r++) {
        printf("    |");
        for (int c = 0; c < GRID_W; c++) putchar(grid[r][c]);
        putchar('\n');
    }
    printf("    +%.*s> x\n", GRID_W, "------------------------------------------------------------");
    printf("\n  Landed after %d steps (%.1f simulated seconds)\n",
           step, step * dt);
        printf("  Range: %.2f units\n\n", position.data[0]);
 
    free_vector(&position); free_vector(&velocity); free_vector(&gravity);
}
 
int main(void) {
    print_worked_example();
    simulate_projectile(15.0, 45.0);
    return 0;
}
