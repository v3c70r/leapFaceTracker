#include <gvector3d.h>
#include <gmatrix3d.h>

GVector3d GVector3d::operator + (const GVector3d &v) const
{
	return GVector3d(mVector[0]+v.mVector[0],
					 mVector[1]+v.mVector[1],
					 mVector[2]+v.mVector[2]);
}

void GVector3d::operator += (const GVector3d &v)
{
	mVector[0] += v.mVector[0];
	mVector[1] += v.mVector[1];
	mVector[2] += v.mVector[2];
}

GVector3d GVector3d::operator - (const GVector3d &v) const
{
	return GVector3d(mVector[0]-v.mVector[0],
					 mVector[1]-v.mVector[1],
					 mVector[2]-v.mVector[2]);
}

void GVector3d::operator -= (const GVector3d &v)
{
	mVector[0] -= v.mVector[0];
	mVector[1] -= v.mVector[1];
	mVector[2] -= v.mVector[2];
}

GVector3d GVector3d::operator - () const
{
    return GVector3d(-mVector[0], -mVector[1], -mVector[2]);
}

GVector3d GVector3d::operator * (double d) const
{
	return GVector3d(mVector[0]*d,
					 mVector[1]*d,
					 mVector[2]*d);
}

void GVector3d::operator *= (double d)
{
	mVector[0] *= d;
	mVector[1] *= d;
	mVector[2] *= d;
}

GVector3d GVector3d::operator * (const GMatrix3d &m) const
{
	GVector3d v;
	v.mVector[0] = mVector[0]*m.mMatrix[0] + mVector[1]*m.mMatrix[4] +
				  mVector[2]*m.mMatrix[8] + m.mMatrix[12];
	v.mVector[1] = mVector[0]*m.mMatrix[1] + mVector[1]*m.mMatrix[5] +
				  mVector[2]*m.mMatrix[9] + m.mMatrix[13];
	v.mVector[2] = mVector[0]*m.mMatrix[2] + mVector[1]*m.mMatrix[6] +
				  mVector[2]*m.mMatrix[10] + m.mMatrix[14];
	double h = mVector[0]*m.mMatrix[3] + mVector[1]*m.mMatrix[7] +
			   mVector[2]*m.mMatrix[11] + m.mMatrix[15];
	v /= h;
	return v;
}

GVector3d GVector3d::transformTo(const GMatrix3d &m) const
{
    GVector3d o(0, 0, 0);
    o *= m;
    GVector3d v = *this * m;
	return v - o;
}

void GVector3d::operator *= (const GMatrix3d &m)
{
	(*this) = (*this)*m;
}

GVector3d GVector3d::operator / (double d) const
{
	return GVector3d(mVector[0]/d,
					 mVector[1]/d,
					 mVector[2]/d);
}

void GVector3d::operator /= (double d)
{
	mVector[0] /= d;
	mVector[1] /= d;
	mVector[2] /= d;
}

GVector3d GVector3d::crossMult(const GVector3d &v) const
{
	return GVector3d(mVector[1]*v.mVector[2] - mVector[2]*v.mVector[1],
					 mVector[2]*v.mVector[0] - mVector[0]*v.mVector[2],
					 mVector[0]*v.mVector[1] - mVector[1]*v.mVector[0]);
}

double GVector3d::dotMult(const GVector3d &v) const
{
	return mVector[0]*v.mVector[0] + 
		   mVector[1]*v.mVector[1] +
		   mVector[2]*v.mVector[2];
}

double GVector3d::getLength() const
{
	return sqrt(mVector[0]*mVector[0] + 
				mVector[1]*mVector[1] +
				mVector[2]*mVector[2]);
}

GVector3d GVector3d::getNormal() const
{
	double len = getLength();
	return GVector3d(mVector[0]/len, mVector[1]/len, mVector[2]/len);
}

void GVector3d::normalize()
{
	double len = getLength();
	mVector[0] /= len;
	mVector[1] /= len;
	mVector[2] /= len;
}

double GVector3d::getLengthXY() const
{
	return sqrt(mVector[0]*mVector[0] + mVector[1]*mVector[1]);
}

double GVector3d::getLengthYZ() const
{
	return sqrt(mVector[1]*mVector[1] + mVector[2]*mVector[2]);
}

double GVector3d::getLengthZX() const
{
	return sqrt(mVector[0]*mVector[0] + mVector[2]*mVector[2]);
}

bool GVector3d::isZero() const
{
	return IS_ZERO(getLength());
}
