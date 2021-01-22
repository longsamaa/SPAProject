#include "../pch.h"
//#include "stdafx.h"
#include "Rect2D.h"
#include <algorithm>

using namespace std;

namespace MapEngine
{
	GERect2D::GERect2D()
	{
		left   = 0;
		top    = 0;
		right  = 0;
		bottom = 0;
	}

	GERect2D::~GERect2D()
	{
	}
	
	GERect2D::GERect2D(real64 x1, real64 y1, real64 x2, real64 y2)
	{
// 		left	=	min(x1, x2);
// 		top		=	min(y1, y2);
// 		right	=	max(x1, x2);
// 		bottom	=	max(y1, y2);
		left	=	x1;
		top		=	y1;
		right	=	x2;
		bottom	=	y2;
	}
	
	GERect2D::GERect2D(const GEPoint2D& c1, const GEPoint2D& c2)
	{
		left	=	min(c1.x, c2.x);
		top		=	min(c1.y, c2.y);
		right	=	max(c1.x, c2.x);
		bottom	=	max(c1.y, c2.y);
	}
	
	GERect2D::GERect2D(const GERect2D& Rect)
	{
		left	=	Rect.left;
		top		=	Rect.top;
		right	=	Rect.right;
		bottom	=	Rect.bottom;
	}
	
	GERect2D::GERect2D(const Point2D& c1, const Point2D& c2)
	{
		left	=	min(c1.x, c2.x);
		top		=	min(c1.y, c2.y);
		right	=	max(c1.x, c2.x);
		bottom	=	max(c1.y, c2.y);
	}
	
	GERect2D::GERect2D(const Rect2D& Rect)
	{
		left	=	Rect.left;
		top		=	Rect.top;
		right	=	Rect.right;
		bottom	=	Rect.bottom;
	}
	
	void GERect2D::ExpandTo(real64 x, real64 y)
	{
		left	=	min(left, x);
		top		=	min(top, y);
		right	=	max(right, x);
		bottom	=	max(bottom, y);
	}
	
	void GERect2D::ExpandTo(const GERect2D& Rect)
	{
		left	=	min(left, Rect.left);
		top		=	min(top, Rect.top);
		right	=	max(right, Rect.right);
		bottom	=	max(bottom, Rect.bottom);
	}
	
	void GERect2D::ExpandTo(const GEPoint2D& c)
	{
		left	=	min(left, c.x);
		top		=	min(top, c.y);
		right	=	max(right, c.x);
		bottom	=	max(bottom, c.y);
	}
	
	GERect2D& GERect2D::operator=(const GERect2D& Rect)
	{
		return Copy(Rect);
	}
	
	bool GERect2D::IsIntersect(const GERect2D& Rect) const
	{
		if (left > Rect.right || Rect.left > right)
			return false;
		if (top > Rect.bottom || Rect.top > bottom)
			return false;
		return true;
	}
	
	bool GERect2D::Intersect(const GERect2D& Rect, GERect2D& Ret)
	{
		if (!IsIntersect(Rect))
			return false;
		Ret.left = max(left, Rect.left);
		Ret.top = max(top, Rect.top);
		Ret.right = min(right, Rect.right);
		Ret.bottom = min(bottom, Rect.bottom);
		return true;
	}
	
	GERect2D GERect2D::Union(const GERect2D& Rect)
	{
		GERect2D Ret;
		Ret.left	=	min(left, Rect.left);
		Ret.top		=	min(top, Rect.top);
		Ret.right	=	max(right, Rect.right);
		Ret.bottom	=	max(bottom, Rect.bottom);
		return Ret;
	}

	bool GERect2D::IsContain(real64 x, real64 y) const
	{
		if (x < left || x > right)
			return false;
		if (y < top || y > bottom)
			return false;
		return true;
	}
	
	bool GERect2D::IsContain(const GEPoint2D& c) const
	{
		if (c.x < left || c.x > right)
			return false;
		if (c.y < top || c.y > bottom)
			return false;
		return true;
	}
	
	bool GERect2D::IsContain(const GERect2D& Rect) const
	{
		if (!IsContain(Rect.left, Rect.top))
			return false;
		if (!IsContain(Rect.right, Rect.bottom))
			return false;
		return true;
	}
	
	bool GERect2D::IsWithin(const GERect2D& Rect) const
	{
		if (!Rect.IsContain(left, top))
			return false;
		if (!Rect.IsContain(right, bottom))
			return false;
		return true;
	}
	
	bool GERect2D::IsValid() const
	{
		if (left > right || top > bottom)
			return false;
		return true;
	}
	
	real64 GERect2D::Width() const
	{
		return right-left;
	}
	
	real64 GERect2D::Height() const
	{
		return bottom-top;
	}
	
	void GERect2D::SetRect(const GEPoint2D& c1, const GEPoint2D c2) 
	{
		left	=	min(c1.x, c2.x);
		top		=	min(c1.y, c2.y);
		right	=	max(c1.x, c2.x);
		bottom	=	max(c1.y, c2.y);
	}
	
	void GERect2D::SetRect(real64 x1, real64 y1, real64 x2, real64 y2)
	{
		left	=	min(x1, x2);
		top		=	min(y1, y2);
		right	=	max(x1, x2);
		bottom	=	max(y1, y2);
	}
	
	real64 GERect2D::CenterX() const
	{
		return (right+left)/2;
	}
	
	real64 GERect2D::CenterY() const
	{
		return (bottom+top)/2;
	}
	
	GEPoint2D GERect2D::Center() const
	{
		GEPoint2D c;
		c.x = (right+left)/2;
		c.y = (bottom+top)/2;
		return c;
	}
	
	bool GERect2D::operator==(const GERect2D& Rect)
	{
		return left == Rect.left && top == Rect.top &&
			   right == Rect.right && bottom == Rect.bottom;
	}
	
	GERect2D& GERect2D::Copy(const GERect2D& Rect)
	{
		left	=	Rect.left;
		top		=	Rect.top;
		right	=	Rect.right;
		bottom	=	Rect.bottom;
		return *this;
	}
} // namespace MapEngine
