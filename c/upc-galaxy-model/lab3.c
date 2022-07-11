#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include "upc_relaxed.h"

#define N 10000
#define NSTEPS 10
#define G 6.67e-11
#define TIMESTEP 0.25

/*
 * body data structure
*/
struct body_s {
	double x;
	double y;
	double z;
	double dx;
	double dy;
	double dz;
	double mass;
};
typedef struct body_s body_t;


/*
 * function prototypes
 */
void init(void);
double dist(double dx, double dy, double dz);


shared body_t bodies[N];  // array of N-bodies at timestep t
body_t next[N];// array of N-bodies at timestep t+1
shared double start, end;
shared int threadCount = 0;

/**
 * init - give the planets initial values for position, velocity, mass
 */
void init(void) {
    if (MYTHREAD == 0){
        for (int i=0; i<N; i++) {
            bodies[i].x = 100.0 * (i + 0.1);
            bodies[i].y = 200.0 * (i + 0.1);
            bodies[i].z = 300.0 * (i + 0.1);
            bodies[i].dx = i + 400.0;
            bodies[i].dy = i + 500.0;
            bodies[i].dz = i + 600.0;
            bodies[i].mass = 10e6 * (i + 100.2);
        }
    }
}


/**
 * eprintf - error printing wrapper (only prints once)
 * @param format printf-like format string
 * @param ... arguments to printf format string
 * @return number of bytes written to stderr
 */
int eprintf(int x, const char *format, ...) {
    va_list ap;
    int ret;

    if (MYTHREAD == x) {
        va_start(ap, format);
        ret = vfprintf(stdout, format, ap);
        va_end(ap);
        return ret;
    }
    else
        return 0;
}



/**
 * dist - determine the distance between two bodies
 *    @param dx - distance in the x dimension
 *    @param dy - distance in the y dimension
 *    @param dz - distance in the z dimension
 *    @return distance
 */
double dist(double dx, double dy, double dz) {
        return sqrt((dx*dx) + (dy*dy) + (dz*dz));;
}




/**
 * computeforce - compute the superposed forces on one body
 *   @param me     - the body to compute forces on at time t
 *   @param nextme - the body at time t+1
 */
void computeforce(body_t *me, body_t *nextme) {
    double d, f, dx, temp_dx, myDx, dy, temp_dy, myDy, dz, temp_dz, myDz, fx, fy, fz, ax, ay, az, myMass; // distance, force, position deltas, force components, acceleration components

    fx = fy = fz = 0.0;
    dx = me->x;
    dy = me->y;
    dz = me->z;
    myMass = me->mass;
    myDx = me->dx;
    myDy = me->dy;
    myDz = me->dz;


    for (int i=0; i<N; i++) {

        // compute the distances in each dimension

        body_t lbody = bodies[i];
        temp_dx = dx - lbody.x;
        temp_dy = dy - lbody.y;
        temp_dz = dz - lbody.z;

        // compute the distance magnitude
        d = dist(temp_dx, temp_dy, temp_dz);

        // skip over ourselves (d==0)
        if (d != 0) {

            // F = G m1 m2 / r^2
            f = (G * myMass * bodies[i].mass) / (d * d);

            // compute force components in each dimension
            fx += (f * dx) / d;
            fy += (f * dy) / d;
            fz += (f * dz) / d;

            // acc = force / mass (F=ma)
            ax = fx / myMass;
            ay = fy / myMass;
            az = fz / myMass;

            // update the body velocity at time t+1
            nextme->dx = myDx + (TIMESTEP * ax);
            nextme->dy = myDy + (TIMESTEP * ay);
            nextme->dz = myDz + (TIMESTEP * az);

            // update the body position at t+1
            nextme->x = dx + (TIMESTEP * myDx);
            nextme->y = dy + (TIMESTEP * myDy);
            nextme->z = dz + (TIMESTEP * myDz);

            // copy over the mass
            nextme->mass = myMass;
        }
    }
}



/**
 *  print_body - prints a body for debugging
 *    @param b - body to print
 */
void print_body(body_t *b) {
    printf("x: %7.3f y: %7.3f z: %7.3f dx: %7.3f dy: %7.3f dz: %7.3f\n",
        b->x, b->y, b->z, b->dx, b->dy, b->dz);
}

void print_shared(shared body_t *b) {
    printf("x: %7.3f y: %7.3f z: %7.3f\n", b->x, b->y, b->z);
}

/*
 * get_wctime - returns wall clock time as double
 *   @return double representation of wall clock time
 */
double get_wctime(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec + 1E-6 * tv.tv_usec);
}

/**
 * main
 */
int main(int argc, char **argv) {
    int nThreads = (N/THREADS);
    body_t *bodies_local = malloc(nThreads * sizeof(body_t));
    body_t *next_local = malloc(nThreads * sizeof(body_t));
    double tsstart, tsfinish;
    // setup initial conditions
    init();
    int initializer = ((nThreads)*MYTHREAD);
    int localInitializer = 0;
    int endpoint = (initializer+(nThreads));
    int threadp1 = (THREADS+1);

    upc_barrier;
    for (int i = initializer; i < endpoint; i++) {
                upc_memget(&bodies_local[localInitializer], &bodies[i], sizeof(body_t));
                localInitializer++;
    }
    upc_barrier;

    eprintf(0, "beginning N-body simulation of %d bodies with %d processes.\n", N, THREADS);
    if (MYTHREAD == 0) {start = get_wctime();}

    upc_barrier;

    // for each timestep in the simulation
    upc_forall (int ts = 0; ts < NSTEPS; ts++; (MYTHREAD+ts)) {
                tsstart = get_wctime();

                // for every body in the universe
                for (int i = 0; i < (nThreads); i++) {
                        computeforce(&bodies_local[i], &next_local[i]);
                }

                free(bodies_local);

                localInitializer = 0;

                upc_barrier;

                // copy the t+1 state to be the new time t
                for (int i = initializer; i < endpoint; i++) {
                        upc_memput(&bodies[i], &next_local[localInitializer], sizeof(body_t));
                        localInitializer++;
                }

                upc_barrier;

                free(next_local);

                tsfinish = get_wctime();
    }

    upc_barrier;

    if (MYTHREAD == (THREADS-1)){end = get_wctime();}

    for (int i = 0; i < NSTEPS; i++) {
                eprintf(i, "timestep %d complete: %7.3f ms\n", i, (tsfinish-tsstart)*1000);
    }

    eprintf((THREADS-1), "execution time: %7.4f ms\n", (end-start)*1000);
    return 0;
}