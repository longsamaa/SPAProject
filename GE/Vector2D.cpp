//#include "stdafx.h"
#include "../pch.h"
#include "Vector2D.h"

#include <Math.h>

namespace MapEngine
{
	GEVector2D::GEVector2D() : GEPoint2D()
	{
	}
	
	GEVector2D::GEVector2D(real64 _x, real64 _y) : GEPoint2D(_x, _y)
	{
	}
	
	GEVector2D::~GEVector2D()
	{
	}
	
	GEVector2D GEVector2D::operator-()
	{
		GEVector2D v;
		v.x = -x;
		v.y = -y;
		return v;
	}
	
	GEVector2D GEVector2D::operator~()
	{
		GEVector2D v;
		v.x = -y;
		v.y = x;
		return v;
	}
	
	real64 GEVector2D::operator*(const GEVector2D& w)
	{
		return ((real64)x*w.x+(real64)y*w.y);
	}
	
	real64 GEVector2D::operator|(const GEVector2D& w)
	{
		return ((real64)x*w.y-(real64)y*w.x);
	}
	
	GEVector2D& GEVector2D::operator*=(real64 c)
	{
		x *= c;
		y *= c;
		return *this;
	}
	
	GEVector2D& GEVector2D::operator/=(real64 c)
	{
		x /= c;
		y /= c;
		return *this;
	}
	
	GEVector2D& GEVector2D::operator+=(const GEVector2D& w)
	{
		x += w.x;
		y += w.y;
		return *this;
	}
	
	GEVector2D& GEVector2D::operator-=(const GEVector2D& w)
	{
		x -= w.x;
		y -= w.y;
		return *this;
	}
	
	GEVector2D& GEVector2D::operator =(const GEVector2D &w)
	{
		return Copy(w);
	}
	
	void GEVector2D::Normalize()
	{
		real64 ln = sqrt((real64)x*x+(real64)y*y);
		if (ln == 0)
			return;
		x /= ln;
		y /= ln;
	}
	
	real64 GEVector2D::Length2()
	{
		return ((real64)x*x+(real64)y*y);
	}
	
	real64 GEVector2D::Length()
	{
		return sqrt((real64)x*x+(real64)y*y);
	}
	
	GEVector2D& GEVector2D::Copy(const GEVector2D &w)
	{
		x = w.x;
		y = w.y;
		return *this;
	}
	
	real64 Length2(const GEVector2D& v)
	{
		return ((real64)v.x*v.x+(real64)v.y*v.y);
	}
	
	real64 Length(const GEVector2D& v)
	{
		return sqrt((real64)v.x*v.x+(real64)v.y*v.y);
	}
	
	real64 Alpha(GEVector2D& v, GEVector2D& w)
	{
		real64 L = Length(v)*Length(w);
		real64 V = v*w;
		return acos(V/L);
	}
	
	GEVector2D operator*(real64 c, const GEVector2D& w)
	{
		GEVector2D v;
		v.x = c*w.x;
		v.y = c*w.y;
		return v;
	}
	
	GEVector2D operator*(const GEVector2D& w, real64 c)
	{
		GEVector2D v;
		v.x = c*w.x;
		v.y = c*w.y;
		return v;
	}
	
	GEVector2D operator/(const GEVector2D& w, real64 c)
	{
		GEVector2D v;
		v.x = w.x/c;
		v.y = w.y/c;
		return v;
	}
	
	GEVector2D operator+(const GEVector2D& v, const GEVector2D& w)
	{
		GEVector2D u;
		u.x = v.x+w.x;
		u.y = v.y+w.y;
		return u;
	}
	
	GEVector2D operator-(const GEVector2D& v, const GEVector2D& w)
	{
		GEVector2D u;
		u.x = v.x-w.x;
		u.y = v.y-w.y;
		return u;
	}
} // namespace MapEngine
