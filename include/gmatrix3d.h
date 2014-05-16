#ifndef GMATRIX3D_H
#define GMATRIX3D_H

#include "gmath.h"

class GVector3d;

class GMatrix3d
{
public:
	//----OpenGL ¾ØÕó´æ´¢Ä£Ê½---------------------------------------------------
	//  |	a0		a1		a2		a3		|	
	//	|	a4		a5		a6		a7		|
	//	|	a8		a9		a10		a11		|
	//	|	a12		a13		a14		a15		|
	//--------------------------------------------------------------------------
	double mMatrix[16];
public:
	GMatrix3d();
	GMatrix3d(const double *mMatrix);
	GMatrix3d(const GMatrix3d &m);
	
	operator double * ()
	{ return mMatrix; }
	
	double & operator [] (int index)
	{ return mMatrix[index]; }

	void operator = (const GMatrix3d &m);
	GMatrix3d operator * (const GMatrix3d &m) const;
	void operator *= (const GMatrix3d &m);

	GMatrix3d getTranspose() const;
	void transpose();
	GMatrix3d getInverse() const;
	void inverse();

    static GMatrix3d createZero();
	static GMatrix3d createIdentity();
	static GMatrix3d createRotateX(double af);
	static GMatrix3d createRotateY(double af);
	static GMatrix3d createRotateZ(double af);
	static GMatrix3d createRotateX(double sinAF, double cosAF);
	static GMatrix3d createRotateY(double sinAF, double cosAF);
	static GMatrix3d createRotateZ(double sinAF, double cosAF);
	static GMatrix3d createRotate(double af, const GVector3d &v);
	static GMatrix3d createTranslate(double x, double y, double z);
	static GMatrix3d createScale(double sx, double sy, double sz);
	static GMatrix3d createMirror(const GVector3d &v);
};

#endif
