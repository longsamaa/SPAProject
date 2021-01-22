// Point2D.h: interface for the GEPoint2D class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Basics.h"

namespace MapEngine
{
	struct Point2D
	{
		real64 x;
		real64 y;
	};

	class GEPoint2D : public Point2D
	{
	friend class GEVector2D;
	public:
		// constructors
		GEPoint2D();
		GEPoint2D(real64 _x, real64 _y);
		GEPoint2D(const GEPoint2D& p);
		GEPoint2D(const Point2D& p);
		// destructors
		virtual ~GEPoint2D();
		void SetPoint(real64 _x, real64 _y);
		void SetPoint(const GEPoint2D& p);
		GEPoint2D& operator=(const GEPoint2D& p);
		GEPoint2D& operator+=(const GEVector2D& v);
		GEPoint2D& operator-=(const GEVector2D& v);
		bool IsInCircle(const GEPoint2D& c, real64 r) const;
		bool IsEqual(const GEPoint2D& p) const;
		bool IsNear(const GEPoint2D& p, real64 r) const;
		bool operator==(const GEPoint2D& p) const;
		bool operator!=(const GEPoint2D &p) const;
		friend GEPoint2D operator*(real64 c, const GEPoint2D& q);
		friend GEPoint2D operator*(const GEPoint2D& q, real64 c);
		friend GEPoint2D operator/(const GEPoint2D& q, real64 c);
		friend real64 IsLeft(const GEPoint2D& p0, const GEPoint2D& p1, const GEPoint2D& p2);
		friend real64 Distance(const GEPoint2D& p1, const GEPoint2D& p2);
		friend int32 XYOrder(const GEPoint2D& p1, const GEPoint2D& p2);
		friend int32 YXOrder(const GEPoint2D& p1, const GEPoint2D& p2);
		friend GEVector2D operator-(const GEPoint2D& q, const GEPoint2D& p);
		friend GEPoint2D operator+(const GEPoint2D& q, const GEVector2D& v);
		friend GEPoint2D operator-(const GEPoint2D& q, const GEVector2D& v);
	private:
		GEPoint2D& Copy(const GEPoint2D &p);
	};

	class GEIPoint2D : public GEPoint2D
	{
	public:
		int32 i;
		real64 d;
	public:
		friend bool operator<(const GEIPoint2D& p1, const GEIPoint2D& p2);
	};

	class GEIJPoint2D : public GEPoint2D
	{
	public:
		int32 i;
		int32 j;
		real64 d;
	public:
		friend bool operator<(const GEIJPoint2D& p1, const GEIJPoint2D& p2);
	};
} // namespace MapEngine
