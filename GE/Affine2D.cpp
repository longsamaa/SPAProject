//#include "stdafx.h"
#include "../pch.h"
#include "Affine2D.h"
#include "math.h"

namespace MapEngine
{
	GEAffine2D::GEAffine2D()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3;j++)
				if (i != j)
					m_Mat[i][j] = 0.0;
				else
					m_Mat[i][j] = 1.0;
	}

	GEAffine2D::~GEAffine2D()
	{
	}

	GEAffine2D::GEAffine2D(double m11, double m12, double m21, double m22, double dx, double dy)
	{
		m_Mat[0][0] = m11;
		m_Mat[0][1] = m12;
		m_Mat[0][2] = 0.0;
		m_Mat[1][0] = m21;
		m_Mat[1][1] = m22;
		m_Mat[1][2] = 0.0;
		m_Mat[2][0] = dx;
		m_Mat[2][1] = dy;
		m_Mat[2][2] = 1.0;
	}

	bool GEAffine2D::IsIdentity()
	{
		bool bRet = true;

		for (int i = 0; (i < 3) && (bRet); i++)
			for (int j = 0; (j < 3) && (bRet); j++)
				if (i != j)
					bRet ^= (m_Mat[i][j]==0.0);
				else
					bRet ^= (m_Mat[i][j]==1.0);
		return bRet;
	}

	bool GEAffine2D::IsInvertible()
	{
		return Determinant() != 0.0;
	}

	double GEAffine2D::OffsetX()
	{
		return m_Mat[2][0];
	}

	double GEAffine2D::OffsetY()
	{
		return m_Mat[2][1];
	}

	bool GEAffine2D::Invert()
	{
		double flDet = Determinant();

		if(flDet!=0.0)
		{
			double a00 = m_Mat[1][1]*m_Mat[2][2]-m_Mat[1][2]*m_Mat[2][1];
			double a01 = m_Mat[0][1]*m_Mat[2][2]-m_Mat[0][2]*m_Mat[2][1];
			double a02 = m_Mat[0][1]*m_Mat[1][2]-m_Mat[0][2]*m_Mat[1][1];
			double a10 = m_Mat[1][0]*m_Mat[2][2]-m_Mat[1][2]*m_Mat[2][0];
			double a11 = m_Mat[0][0]*m_Mat[2][2]-m_Mat[0][2]*m_Mat[2][0];
			double a12 = m_Mat[0][0]*m_Mat[1][2]-m_Mat[0][2]*m_Mat[1][0];
			double a20 = m_Mat[1][0]*m_Mat[2][1]-m_Mat[1][1]*m_Mat[2][0];
			double a21 = m_Mat[0][0]*m_Mat[2][1]-m_Mat[0][1]*m_Mat[2][0];
			double a22 = m_Mat[0][0]*m_Mat[1][1]-m_Mat[0][1]*m_Mat[1][0];
			m_Mat[0][0] = a00/flDet;
			m_Mat[0][1] = -a01/flDet;
			m_Mat[0][2] = a02/flDet;
			m_Mat[1][0] = -a10/flDet;
			m_Mat[1][1] = a11/flDet;
			m_Mat[1][2] = -a12/flDet;
			m_Mat[2][0] = a20/flDet;
			m_Mat[2][1] = -a21/flDet;
			m_Mat[2][2] = a22/flDet;
			return true;
		}
		return false;
	}

	void GEAffine2D::Multiply(const GEAffine2D &mt)
	{
		double a00 = m_Mat[0][0]*mt.m_Mat[0][0]+m_Mat[0][1]*mt.m_Mat[1][0]+m_Mat[0][2]*mt.m_Mat[2][0];
		double a01 = m_Mat[0][0]*mt.m_Mat[0][1]+m_Mat[0][1]*mt.m_Mat[1][1]+m_Mat[0][2]*mt.m_Mat[2][1];
		double a02 = m_Mat[0][0]*mt.m_Mat[0][2]+m_Mat[0][1]*mt.m_Mat[1][2]+m_Mat[0][2]*mt.m_Mat[2][2];
		double a10 = m_Mat[1][0]*mt.m_Mat[0][0]+m_Mat[1][1]*mt.m_Mat[1][0]+m_Mat[1][2]*mt.m_Mat[2][0];
		double a11 = m_Mat[1][0]*mt.m_Mat[0][1]+m_Mat[1][1]*mt.m_Mat[1][1]+m_Mat[1][2]*mt.m_Mat[2][1];
		double a12 = m_Mat[1][0]*mt.m_Mat[0][2]+m_Mat[1][1]*mt.m_Mat[1][2]+m_Mat[1][2]*mt.m_Mat[2][2];
		double a20 = m_Mat[2][0]*mt.m_Mat[0][0]+m_Mat[2][1]*mt.m_Mat[1][0]+m_Mat[2][2]*mt.m_Mat[2][0];
		double a21 = m_Mat[2][0]*mt.m_Mat[0][1]+m_Mat[2][1]*mt.m_Mat[1][1]+m_Mat[2][2]*mt.m_Mat[2][1];
		double a22 = m_Mat[2][0]*mt.m_Mat[0][2]+m_Mat[2][1]*mt.m_Mat[1][2]+m_Mat[2][2]*mt.m_Mat[2][2];

		m_Mat[0][0] = a00;
		m_Mat[0][1] = a01;
		m_Mat[0][2] = a02;
		m_Mat[1][0] = a10;
		m_Mat[1][1] = a11;
		m_Mat[1][2] = a12;
		m_Mat[2][0] = a20;
		m_Mat[2][1] = a21;
		m_Mat[2][2] = a22;
	}

	void GEAffine2D::Reset()
	{
		for(int i=0; (i < 3); i++)
			for(int j=0; (j < 3);j++)
				if(i!=j)
					m_Mat[i][j]=0.0;
				else
					m_Mat[i][j]=1.0;
	}

	void GEAffine2D::Initialize(double m11, double m12, double m21, double m22, double dx, double dy)
	{
		m_Mat[0][0] = m11;
		m_Mat[0][1] = m12;
		m_Mat[0][2] = 0.0;
		m_Mat[1][0] = m21;
		m_Mat[1][1] = m22;
		m_Mat[1][2] = 0.0;
		m_Mat[2][0] = dx;
		m_Mat[2][1] = dy;
		m_Mat[2][2] = 1.0;
	}

	void GEAffine2D::Rotate(double angle)
	{
		angle*= 3.141592/180.0;

		GEAffine2D mt(cos(angle),0.0,-sin(angle),0.0,0.0,0.0);

		Multiply(mt);
	}

	void GEAffine2D::RotateAt(double angle, const GEPoint2D &p)
	{
		angle*=3.141592/180.0;

		double cosx = cos(angle);
		double sinx = sin(angle);
		
		GEAffine2D mt(cosx, sinx, -sinx, cosx, p.x*(1-cosx)+p.y*sinx, p.y*(1-cosx)-p.x*sinx);
		Multiply(mt);
	}

	void GEAffine2D::Scale(double scaleX, double scaleY)
	{
		GEAffine2D mt(scaleX, 0.0,0.0,scaleY,0.0,0.0);
		Multiply(mt);
	}

	void GEAffine2D::Scale(double scaleX, double scaleY, const GEPoint2D &p)
	{
		GEAffine2D mt(1.0, 0.0, 0.0, 1.0, -p.x, -p.y);
		Multiply(mt);
		mt.Initialize(scaleX, 0.0, 0.0, scaleY, 0.0, 0.0);
		Multiply(mt);
		mt.Initialize(1.0, 0.0, 0.0, 1.0, p.x, p.y);
		Multiply(mt);
	}

	void GEAffine2D::Shear(double shearX, double shearY, const GEPoint2D &p)
	{
		GEAffine2D mt(1.0,shearY,shearX,1.0,-p.y*shearX,-p.x*shearY);
		Multiply(mt);
	}

	void GEAffine2D::Translate(double offsetX, double offsetY)
	{
		GEAffine2D mt(1.0,0.0,0.0,1.0,offsetX, offsetY);
		Multiply(mt);
	}

	double GEAffine2D::Determinant()
	{
		return m_Mat[0][0]*m_Mat[1][1]*m_Mat[2][2]-
			   m_Mat[0][0]*m_Mat[1][2]*m_Mat[2][1]-
			   m_Mat[1][0]*m_Mat[0][1]*m_Mat[2][2]+
			   m_Mat[1][0]*m_Mat[0][2]*m_Mat[2][1]+
			   m_Mat[2][0]*m_Mat[0][1]*m_Mat[1][2]-
			   m_Mat[2][0]*m_Mat[0][2]*m_Mat[1][1];		   	
	}

	GEAffine2D GEAffine2D::Clone()
	{
		GEAffine2D	mt;

		mt.m_Mat[0][0] = m_Mat[0][0];
		mt.m_Mat[0][1] = m_Mat[0][1];
		mt.m_Mat[0][2] = m_Mat[0][2];
		mt.m_Mat[1][0] = m_Mat[1][0];
		mt.m_Mat[1][1] = m_Mat[1][1];
		mt.m_Mat[1][2] = m_Mat[1][2];
		mt.m_Mat[2][0] = m_Mat[2][0];
		mt.m_Mat[2][1] = m_Mat[2][1];
		mt.m_Mat[2][2] = m_Mat[2][2];

		return mt;
	}

	void GEAffine2D::TransformPoint(GEPoint2D &p)
	{
		GEPoint2D q;

		q.x = p.x*m_Mat[0][0]+p.y*m_Mat[1][0]+m_Mat[2][0];
		q.y = p.x*m_Mat[0][1]+p.y*m_Mat[1][1]+m_Mat[2][1];
		p.x = q.x;
		p.y = q.y;
	}

	void GEAffine2D::TransformPoint(double &x, double &y)
	{
		GEPoint2D q;

		q.x = x*m_Mat[0][0]+y*m_Mat[1][0]+m_Mat[2][0];
		q.y = x*m_Mat[0][1]+y*m_Mat[1][1]+m_Mat[2][1];
		x = q.x;
		y = q.y;
	}
}
