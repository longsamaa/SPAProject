#include "../pch.h"
//#include "stdafx.h"
#include "Point2D.h"
#include "Vector2D.h"
#include "Define.h"
#include <math.h>

namespace MapEngine
{
	GEPoint2D::GEPoint2D()
	{
		x = 0;
		y = 0;
	}
	
	GEPoint2D::GEPoint2D(real64 _x, real64 _y)
	{
		x = _x;
		y = _y;
	}
	
	GEPoint2D::GEPoint2D(const GEPoint2D& p)
	{
		x = p.x;
		y = p.y;
	}
	
	GEPoint2D::GEPoint2D(const Point2D& p)
	{
		x = p.x;
		y = p.y;
	}
	
	GEPoint2D::~GEPoint2D()
	{
	}
	
	void GEPoint2D::SetPoint(real64 _x, real64 _y)
	{
		x = _x;
		y = _y;
	}
	
	void GEPoint2D::SetPoint(const GEPoint2D& p)
	{
		x = p.x;
		y = p.y;
	}
	
	GEPoint2D& GEPoint2D::operator=(const GEPoint2D& p)
	{
		return Copy(p);
	}
	
	GEPoint2D& GEPoint2D::operator+=(const GEVector2D& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	
	GEPoint2D& GEPoint2D::operator-=(const GEVector2D& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	bool GEPoint2D::IsInCircle(const GEPoint2D& c, real64 r) const
	{
		real64 v = ((real64)x-c.x)*(x-c.x)+((real64)y-c.y)*(y-c.y)-(real64)r*r;
		return v <= 0;
	}
	
	bool GEPoint2D::IsEqual(const GEPoint2D& p) const
	{
		if (x != p.x)
			return false;
		if (y != p.y)
			return false;
		return true;
	}
	
	bool GEPoint2D::IsNear(const GEPoint2D& p, real64 r) const
	{
		if (fabs(x-p.x) > r)
			return false;
		if (fabs(y-p.y) > r)
			return false;
		return true;
	}

	bool GEPoint2D::operator==(const GEPoint2D& p) const
	{
		return IsEqual(p);
	}
	
	bool GEPoint2D::operator!=(const GEPoint2D &p) const
	{
		return !IsEqual(p);
	}
	
	GEPoint2D& GEPoint2D::Copy(const GEPoint2D &p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	GEPoint2D operator*(real64 c, const GEPoint2D& q)
	{
		GEPoint2D p;
		p.x = c*q.x;
		p.y = c*q.y;
		return p;
	}
	
	GEPoint2D operator*(const GEPoint2D& q, real64 c)
	{
		GEPoint2D p;
		p.x = c*q.x;
		p.y = c*q.y;
		return p;
	}
	
	GEPoint2D operator/(const GEPoint2D& q, real64 c)
	{
		GEPoint2D p;
		p.x = q.x/c;
		p.y = q.y/c;
		return p;
	}
	
	real64 IsLeft(const GEPoint2D& p0, const GEPoint2D& p1, const GEPoint2D& p2)
	{
		return ((real64)p1.x-p0.x)*(p2.y-p0.y)-((real64)p2.x-p0.x)*(p1.y-p0.y);
	}
	
	real64 Distance(const GEPoint2D& p1, const GEPoint2D& p2)
	{

		real64 d;
		d = ((real64)p1.x-p2.x)*(p1.x-p2.x)+((real64)p1.y-p2.y)*(p1.y-p2.y);
		if (d)
			return sqrt(d);
		return 0;
/*
		double dbTheta, dbDist, dbDistInRad;
		double dbLat1InRad;
		double dbLong1InRad;
		double dbLat2InRad;
		double dbLong2InRad;
		double dbThetaInRad;
		
		dbLat1InRad = p1.y * (PI / 180.0);
		dbLong1InRad = p1.x * (PI / 180.0);
		dbLat2InRad = p2.y * (PI / 180.0);
		dbLong2InRad = p2.x * (PI / 180.0);
		
		dbTheta = p1.x - p2.x;
		dbThetaInRad = dbTheta * (PI / 180.0);
		
		dbDistInRad = sin(dbLat1InRad) * sin(dbLat2InRad) + 
			cos(dbLat1InRad) * cos(dbLat2InRad) * cos(dbThetaInRad);
		
		dbDistInRad = acos(dbDistInRad);
		dbDist = dbDistInRad * 180.0 / PI;
		dbDist = dbDist * 60.0 * 1.1515;
		dbDist = dbDist * 1.609344 * 1000;

		return dbDist;
*/
	}
	
	int32 XYOrder(const GEPoint2D& p1, const GEPoint2D& p2) 
	{
		if (p1.x > p2.x)
			return 1; 
		if (p1.x < p2.x)
			return -1;
		if (p1.y > p2.y)
			return 1; 
		if (p1.y < p2.y)
			return -1;
		return 0;
	}
	
	int32 YXOrder(const GEPoint2D& p1, const GEPoint2D& p2) 
	{
		if (p1.y > p2.y)
			return 1; 
		if (p1.y < p2.y)
			return -1;
		if (p1.x > p2.x)
			return 1; 
		if (p1.x < p2.x)
			return -1;
		return 0;
	}
	
	GEVector2D operator-(const GEPoint2D& q, const GEPoint2D& p)
	{
		GEVector2D v;
		v.x = q.x-p.x;
		v.y = q.y-p.y;
		return v;
	}
	
	GEPoint2D operator+(const GEPoint2D& q, const GEVector2D& v)
	{
		GEPoint2D p;
		p.x = q.x+v.x;
		p.y = q.y+v.y;
		return p;
	}
	
	GEPoint2D operator-(const GEPoint2D& q, const GEVector2D& v)
	{
		GEPoint2D p;
		p.x = q.x-v.x;
		p.y = q.y-v.y;
		return p;
	}
	
	bool operator<(const GEIPoint2D& p1, const GEIPoint2D& p2)
	{
		return p1.d < p2.d;
	}

	bool operator<(const GEIJPoint2D& p1, const GEIJPoint2D& p2)
	{
		return p1.d < p2.d;
	}
} // namespace MapEngine
