// Rect2D.h: interface for the GERect2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_RECT2D_H_)
#define _RECT2D_H_

#include "Point2D.h"

namespace MapEngine
{
	struct Rect2D
	{
		real64 left;
		real64 top;
		real64 right;
		real64 bottom;
	};
	
	class GERect2D : public Rect2D  
	{
	public:
		// constructor
		GERect2D();
		GERect2D(real64 x1, real64 y1, real64 x2, real64 y2);
		GERect2D(const GEPoint2D& c1, const GEPoint2D& c2);
		GERect2D(const GERect2D& Rect);
		GERect2D(const Point2D& c1, const Point2D& c2);
		GERect2D(const Rect2D& Rect);
		// destructor
		virtual ~GERect2D();
		void ExpandTo(real64 x, real64 y);
		void ExpandTo(const GERect2D& Rect);
		void ExpandTo(const GEPoint2D& c);
		GERect2D& operator=(const GERect2D& Rect);
		bool Intersect(const GERect2D& Rect, GERect2D& Ret);
		GERect2D Union(const GERect2D& Rect);
		bool IsIntersect(const GERect2D& Rect) const;
		bool IsContain(real64 x, real64 y) const;
		bool IsContain(const GEPoint2D& c) const;
		bool IsContain(const GERect2D& Rect) const;
		bool IsWithin(const GERect2D& Rect) const;
		bool IsValid() const;
		real64 Width() const;
		real64 Height() const;
		void SetRect(const GEPoint2D& c1, const GEPoint2D c2); 
		void SetRect(real64 x1, real64 y1, real64 x2, real64 y2);
		real64 CenterX() const;
		real64 CenterY() const;
		GEPoint2D Center() const;
		bool operator==(const GERect2D& Rect);
	private:
		GERect2D& Copy(const GERect2D& Rect);
	};
} // namespace MapEngine

#endif // !defined(_RECT2D_H_)
