#ifndef GPOINT3D_H
#define GPOINT3D_H

class GMatrix3d;

class GPoint3d
{
public:
	double mVector[3];			//坐标数组分别表示x, y, z
public:
	GPoint3d();
	GPoint3d(double x, double y, double z);
	GPoint3d(const double *v);
	GPoint3d(const GPoint3d &v);

	double x() const	{ return mVector[0]; }
	double y() const	{ return mVector[1]; }
	double z() const	{ return mVector[2]; }

	void setX(double x) { mVector[0] = x; }
	void setY(double y) { mVector[1] = y; }
	void setZ(double z) { mVector[2] = z; }
	void set(double x, double y, double z)
	{ mVector[0] = x;	mVector[1] = y;		mVector[2] = z; }
	void set(const double *v)
	{ mVector[0] = v[0];		mVector[1] = v[1];	mVector[2] = v[2]; }
	
	operator double * ()
	{ return mVector; }
	
	double & operator [] (int index)
    { return mVector[index]; } 
	
	void operator = (const GPoint3d &other);
	
	GPoint3d transformTo(const GMatrix3d &tm) const;
};

#endif
