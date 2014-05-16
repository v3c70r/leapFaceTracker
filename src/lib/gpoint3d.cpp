#include <gpoint3d.h>
#include <gmatrix3d.h>

GPoint3d::GPoint3d()
{
	mVector[0] = mVector[1] = mVector[2] = 0;
}

GPoint3d::GPoint3d(double x, double y, double z)
{
	mVector[0] = x;
	mVector[1] = y;
	mVector[2] = z;
}

GPoint3d::GPoint3d(const double *v)
{
	mVector[0] = v[0];
	mVector[1] = v[1];
	mVector[2] = v[2];
}

GPoint3d::GPoint3d(const GPoint3d &v)
{
	mVector[0] = v.mVector[0];
	mVector[1] = v.mVector[1];
	mVector[2] = v.mVector[2];
}

void GPoint3d::operator = (const GPoint3d &other)
{
    mVector[0] = other.mVector[0];
    mVector[1] = other.mVector[1];
    mVector[2] = other.mVector[2];
}

GPoint3d GPoint3d::transformTo(const GMatrix3d &m) const
{
    GPoint3d v;
	v.mVector[0] = mVector[0]*m.mMatrix[0] + mVector[1]*m.mMatrix[4] +
				  mVector[2]*m.mMatrix[8] + m.mMatrix[12];
	v.mVector[1] = mVector[0]*m.mMatrix[1] + mVector[1]*m.mMatrix[5] +
				  mVector[2]*m.mMatrix[9] + m.mMatrix[13];
	v.mVector[2] = mVector[0]*m.mMatrix[2] + mVector[1]*m.mMatrix[6] +
				  mVector[2]*m.mMatrix[10] + m.mMatrix[14];
	double h = mVector[0]*m.mMatrix[3] + mVector[1]*m.mMatrix[7] +
			   mVector[2]*m.mMatrix[11] + m.mMatrix[15];
	v.mVector[0] /= h;
	v.mVector[1] /= h;
	v.mVector[2] /= h;
	return v;
}
