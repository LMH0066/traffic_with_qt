#ifndef DECIDE_H
#define DECIDE_H

#include <cmath>
#include <string.h>

// Function Declarations
extern void allot(const double plaza[4][500], double timeRow,
                  double *rowTime, double *colTime);

extern void new_cars(double plaza[500], double v[500], double probc, const
                     double probv[2], const double probw[3], const double
                     VTypes[2], double vmax[500], double ways[500]);

extern void simulation_and_decision(double probc[4], const double probv[4][2], const double probw[4][3],
                                    const double VTypes[4][2], double *rowTime, double *colTime);

extern void begin_decide(int car_number[4], double &rowTime, double &colTime);

#endif
