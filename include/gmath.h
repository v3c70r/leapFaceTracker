#ifndef GMATH_H
#define GMATH_H

#include <math.h>

class GVector3d;
class GMatrix3d;
class GPoint3d;

#define PI 3.1415926535
#define RADIAN_TO_ANGLE 57.29577958
#define ANGLE_TO_RADIAN 0.01745329
#define GMATH_ZERO		1.0E-6
#define IS_ZERO(x)		(fabs(x) < GMATH_ZERO)

template <class TData> void gswap(TData &x, TData &y)
{
    TData tmp = x;
    x = y;
    y = tmp;
}

int gRound( double x );

int gTrunc( double x );

double gSqr( double x );

double gDistance(const GPoint3d &v0, const GPoint3d &v1);

bool gInverseMatrix(double *m, int n);

#endif
