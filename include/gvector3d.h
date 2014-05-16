#ifndef GVECTOR3D_H
#define GVECTOR3D_H

#include <math.h>
#include "gmath.h"
#include "gpoint3d.h"

class GMatrix3d;

class GVector3d : public GPoint3d
{
protected:
    GVector3d operator * (const GMatrix3d &m) const;
	void operator *= (const GMatrix3d &m);    
public:
    GVector3d() : GPoint3d() {}
	GVector3d(double x, double y, double z) : GPoint3d(x, y, z) {}
	GVector3d(const double *v) : GPoint3d(v) {}
	GVector3d(const GPoint3d &v) : GPoint3d(v) {}
	//----矢量操作--------------------------------------------------------------
	
	// 向量加法 
	GVector3d operator + (const GVector3d &v) const;
	void operator += (const GVector3d &v);

    // 向量减法 
	GVector3d operator - (const GVector3d &v) const;
	void operator -= (const GVector3d &v);
	GVector3d operator - () const;

    // 向量数乘 
	GVector3d operator * (double d) const;
	void operator *= (double d);

    // 向量数除 
	GVector3d operator / (double d) const;
	void operator /= (double d);

    // 向量叉积 
	GVector3d crossMult(const GVector3d &v) const;
	
	// 向量点积 
	double dotMult(const GVector3d &v) const;

    // 向量长度 
	double getLength() const;
	// 向量在XOY平面上长度 
	double getLengthXY() const;
	// 向量在YOZ平面上长度 
	double getLengthYZ() const;
	// 向量在ZOX平面上长度 
	double getLengthZX() const;
	// 是否为0长度向量 
	bool isZero() const;

    // 得到归一化向量 
	GVector3d getNormal() const;
	// 将向量归一化 
	void normalize();
	
	// 得到经变换矩阵m变换后的向量 
	GVector3d transformTo(const GMatrix3d &m) const;
};

#endif
