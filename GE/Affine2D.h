#if !defined(_AFFINE2D_H_)
#define _AFFINE2D_H_

#include "Point2D.h"

namespace MapEngine
{
	class GEAffine2D
	{
	public:
		void Initialize(double m11, double m12, double m21, double m22, double dx, double dy);
		void Translate(double offsetX, double offsetY);
		void Shear(double shearX, double shearY, const GEPoint2D& p);
		void RotateAt(double angle, const GEPoint2D& p);
		void Scale(double scaleX, double scaleY);
		void Scale(double scaleX, double scaleY, const GEPoint2D& p);
		void Rotate(double angle);
		void Reset();
		void Multiply(const GEAffine2D& mt);
		bool Invert();
		double OffsetY();
		double OffsetX();
		bool IsInvertible();
		bool IsIdentity();
		void TransformPoint(GEPoint2D& p);
		void TransformPoint(double& x, double& y);
		GEAffine2D Clone();
		GEAffine2D(double m11, double m12, double m21, double m22, double dx, double dy);
		GEAffine2D();
		virtual ~GEAffine2D();

	public:
		double m_Mat[3][3];
	private:
		double Determinant();
	};
}

#endif // !defined(_AFFINE2D_H_)
