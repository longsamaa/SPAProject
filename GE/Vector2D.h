// Vector2D.h: interface for the GEVector2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VECTOR2D_H_)
#define _VECTOR2D_H_

#include "Point2D.h"

namespace MapEngine
{
	class GEVector2D : public GEPoint2D
	{
	public:
		// constructor
		GEVector2D();
		GEVector2D(real64 _x, real64 _y);
		// destructor
		~GEVector2D();
		GEVector2D operator-();
		GEVector2D operator~();
		real64 operator*(const GEVector2D& w);
		real64 operator|(const GEVector2D& w);
		GEVector2D& operator*=(real64 c);
		GEVector2D& operator/=(real64 c);
		GEVector2D& operator+=(const GEVector2D& w);
		GEVector2D& operator-=(const GEVector2D& w);
		GEVector2D& operator =(const GEVector2D &w);
		void Normalize();
		real64 Length2();
		real64 Length();
		// friend functions
		friend real64 Length2(const GEVector2D& v);
		friend real64 Length(const GEVector2D& v);
		friend real64 Alpha(const GEVector2D& v, const GEVector2D& w);
		// friend operators
		friend GEVector2D operator*(real64 c, const GEVector2D& w);
		friend GEVector2D operator*(const GEVector2D& w, real64 c);
		friend GEVector2D operator/(const GEVector2D& w, real64 c);
		friend GEVector2D operator+(const GEVector2D& v, const GEVector2D& w);
		friend GEVector2D operator-(const GEVector2D& v, const GEVector2D& w);
	private:
		GEVector2D& Copy(const GEVector2D &w);
	};
} // namespace MapEngine

#endif // !defined(_VECTOR2D_H_)
