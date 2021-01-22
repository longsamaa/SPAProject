#include "../pch.h"
//#include "stdafx.h"
#include "Transform2D.h"

namespace MapEngine
{
	GETransform2D::GETransform2D()
	{
		m_btMapUnit = 0;
	}
	GETransform2D::~GETransform2D()
	{
	}

	void GETransform2D::ZoomFit()
	{
		ZoomWindow(m_rcMapMBR);
	}

	void GETransform2D::ZoomCenter(const GEPoint2D& p)
	{
		double Xc = (m_rcDisplay.left+m_rcDisplay.right)/2;
		double Yc = (m_rcDisplay.top+m_rcDisplay.bottom)/2;

		Xc-=p.x;
		Yc-=p.y;
		m_mtR2V.Translate(Xc, Yc);
		AssignInvertionMatrix();
	}

	void GETransform2D::ZoomPOut(const GEPoint2D& p)
	{
		m_mtR2V.Translate(-p.x, -p.y);
		m_mtR2V.Scale(0.5, 0.5);
		m_mtR2V.Translate(p.x, p.y);	
		AssignInvertionMatrix();
	}

	void GETransform2D::ZoomPIn(const GEPoint2D& p)
	{
		m_mtR2V.Translate(-p.x, -p.y);
		m_mtR2V.Scale(2, 2);
		m_mtR2V.Translate(p.x, p.y);
		AssignInvertionMatrix();
	}

	//  [6/22/2006 Luong Cam Vi 10:43:07]
	void GETransform2D::ZoomPOut(const GEPoint2D& p, int Scale)
	{
		double Sc;

		Sc = Scale / 100.0;
		m_mtR2V.Translate(-p.x, -p.y);
		m_mtR2V.Scale(1-Sc, 1-Sc);
		m_mtR2V.Translate(p.x, p.y);
		AssignInvertionMatrix();
	}

	void GETransform2D::ZoomPIn(const GEPoint2D& p, int Scale)
	{
		double Sc;

		Sc = Scale / 100.0;
		m_mtR2V.Translate(-p.x, -p.y);
		m_mtR2V.Scale(1+Sc, 1+Sc);
		m_mtR2V.Translate(p.x, p.y);
		AssignInvertionMatrix();
	}

	bool GETransform2D::AssignInvertionMatrix()
	{
		bool bRet = m_mtR2V.IsInvertible();
		if(bRet)
		{
			m_mtV2R = m_mtR2V.Clone();
			m_mtV2R.Invert();
		}

		return bRet;
	}

	void GETransform2D::ZoomWindow(GERect2D &rc)
	{
		if((rc.Height()==0.0)||(rc.Width()==0.0))
			return;

		double	scaleX, scaleY;
		double  scale;
		
		scaleX = m_rcDisplay.Width()/rc.Width();
		scaleY = m_rcDisplay.Height()/rc.Height();

		scale = (scaleX > scaleY)?scaleY:scaleX;
		
		double flW = m_rcDisplay.Width()/scale;
		double flH = m_rcDisplay.Height()/scale;

		double Xc, Yc;

		Xc = rc.CenterX();
		Yc = rc.CenterY();

		m_mtR2V.Reset();
		GEAffine2D mt(1, 0, 0, -1, 0, 0);

		m_mtR2V.Translate(-Xc, -Yc);
		m_mtR2V.Multiply(mt);
		m_mtR2V.Scale(scale, scale);

		Xc = (m_rcDisplay.left+m_rcDisplay.right)/2;
		Yc = (m_rcDisplay.top+m_rcDisplay.bottom)/2;
				
		m_mtR2V.Translate(Xc, Yc);
		AssignInvertionMatrix();
	}

	void GETransform2D::SetMapMBR(const GERect2D& rcMBR)
	{
		m_rcMapMBR = rcMBR;
	}

	GERect2D GETransform2D::GetMapMBR()
	{
		return m_rcMapMBR;
	}

	void GETransform2D::SetViewMBR(const GERect2D& rcMBR)
	{
		m_rcDisplay.left   = rcMBR.left;
		m_rcDisplay.top    = rcMBR.top;
		m_rcDisplay.right  = rcMBR.right;
		m_rcDisplay.bottom = rcMBR.bottom;
	}

	GERect2D GETransform2D::GetViewMBR()
	{
		return m_rcDisplay;
	}

	GEPoint2D GETransform2D::RP2VP(const double &x, const double &y)
	{
		GEPoint2D q;
		q.x = x*m_mtR2V.m_Mat[0][0]+y*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		q.y = x*m_mtR2V.m_Mat[0][1]+y*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
		return q;
	}

	void GETransform2D::RP2VP(const double &x, const double &y, GEPoint2D &P)
	{
		P.x = x*m_mtR2V.m_Mat[0][0]+y*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		P.y = x*m_mtR2V.m_Mat[0][1]+y*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
	}

	GEPoint2D GETransform2D::RP2VP(const GEPoint2D &p)
	{
		GEPoint2D q;
		q.x = p.x*m_mtR2V.m_Mat[0][0]+p.y*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		q.y = p.x*m_mtR2V.m_Mat[0][1]+p.y*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
		return q;
	}

	void GETransform2D::RP2VP(const GEPoint2D &p, GEPoint2D &P)
	{
		P.x = p.x*m_mtR2V.m_Mat[0][0]+p.y*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		P.y = p.x*m_mtR2V.m_Mat[0][1]+p.y*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
	}

	GEPoint2D GETransform2D::VP2RP(const GEPoint2D &p)
	{
		GEPoint2D q;
		q.x = p.x*m_mtV2R.m_Mat[0][0]+p.y*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		q.y = p.x*m_mtV2R.m_Mat[0][1]+p.y*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
		return q;
	}

	GEPoint2D GETransform2D::VP2RP(const double &x, const double &y)
	{
		GEPoint2D q;
		q.x = x*m_mtV2R.m_Mat[0][0]+y*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		q.y = x*m_mtV2R.m_Mat[0][1]+y*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
		return q;
	}

	void GETransform2D::VP2RP(const GEPoint2D &p, GEPoint2D &P)
	{
		P.x = p.x*m_mtV2R.m_Mat[0][0]+p.y*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		P.y = p.x*m_mtV2R.m_Mat[0][1]+p.y*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
	}

	void GETransform2D::VP2RP(const double &x, const double &y, GEPoint2D &P)
	{
		P.x = x*m_mtV2R.m_Mat[0][0]+y*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		P.y = x*m_mtV2R.m_Mat[0][1]+y*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
	}

	GERect2D GETransform2D::RR2VR(const GERect2D& rc)
	{
		GERect2D r;
		double t;

		r.left = rc.left*m_mtR2V.m_Mat[0][0]+rc.top*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		r.top  = rc.left*m_mtR2V.m_Mat[0][1]+rc.top*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
		r.right  = rc.right*m_mtR2V.m_Mat[0][0]+rc.bottom*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		r.bottom = rc.right*m_mtR2V.m_Mat[0][1]+rc.bottom*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
		if (r.top > r.bottom)
		{
			t = r.top;
			r.top = r.bottom;
			r.bottom = t;
		}
		return r;
	}

	void GETransform2D::RR2VR(const GERect2D& rc, GERect2D &r)
	{
		double t;

		r.left = rc.left*m_mtR2V.m_Mat[0][0]+rc.top*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		r.top  = rc.left*m_mtR2V.m_Mat[0][1]+rc.top*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
		r.right  = rc.right*m_mtR2V.m_Mat[0][0]+rc.bottom*m_mtR2V.m_Mat[1][0]+m_mtR2V.m_Mat[2][0];
		r.bottom = rc.right*m_mtR2V.m_Mat[0][1]+rc.bottom*m_mtR2V.m_Mat[1][1]+m_mtR2V.m_Mat[2][1];
		if (r.top > r.bottom)
		{
			t = r.top;
			r.top = r.bottom;
			r.bottom = t;
		}
	}

	void GETransform2D::Translate(GERect2D& rc)
	{
		m_mtR2V.Translate(rc.Width(), rc.Height());
		AssignInvertionMatrix();
	}

	void GETransform2D::ZoomWIn(GERect2D &rc)
	{
		double CVX, CVY, SX, SY, S;

		CVX = rc.CenterX();
		CVY = rc.CenterY();
		
		SX  = m_rcDisplay.Width()/rc.Width();
		SY  = m_rcDisplay.Height()/rc.Height();
		S   = (SX > SY)?SY:SX;

		m_mtR2V.Translate(-CVX, -CVY);
		m_mtR2V.Scale(S, S);
		m_mtR2V.Translate(CVX, CVY);
		AssignInvertionMatrix();
	}

	void GETransform2D::ZoomWOut(GERect2D &rc)
	{
		double CVX, CVY, SX, SY, S;

		CVX = rc.CenterX();
		CVY = rc.CenterY();
		
		SX  = rc.Width()/m_rcDisplay.Width();
		SY  = rc.Height()/m_rcDisplay.Height();
		S   = (SX>SY)?SY:SX;

		m_mtR2V.Translate(-CVX, -CVY);
		m_mtR2V.Scale(S, S);
		m_mtR2V.Translate(CVX, CVY);
		AssignInvertionMatrix();
	}

	GERect2D GETransform2D::VR2RR(const GERect2D &rc)
	{
		GERect2D rcDest; 
		double dbTemp;

		rcDest.left   = rc.left*m_mtV2R.m_Mat[0][0]+rc.top*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		rcDest.top    = rc.left*m_mtV2R.m_Mat[0][1]+rc.top*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
		rcDest.right  = rc.right*m_mtV2R.m_Mat[0][0]+rc.bottom*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		rcDest.bottom = rc.right*m_mtV2R.m_Mat[0][1]+rc.bottom*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
		if (rcDest.top > rcDest.bottom)
		{
			dbTemp = rcDest.top;
			rcDest.top = rcDest.bottom;
			rcDest.bottom = dbTemp;
		}
		return rcDest;
	}

	void GETransform2D::VR2RR(const GERect2D &rc, GERect2D &rcDest)
	{
		double dbTemp;

		rcDest.left   = rc.left*m_mtV2R.m_Mat[0][0]+rc.top*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		rcDest.top    = rc.left*m_mtV2R.m_Mat[0][1]+rc.top*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
		rcDest.right  = rc.right*m_mtV2R.m_Mat[0][0]+rc.bottom*m_mtV2R.m_Mat[1][0]+m_mtV2R.m_Mat[2][0];
		rcDest.bottom = rc.right*m_mtV2R.m_Mat[0][1]+rc.bottom*m_mtV2R.m_Mat[1][1]+m_mtV2R.m_Mat[2][1];
		if (rcDest.top > rcDest.bottom)
		{
			dbTemp = rcDest.top;
			rcDest.top = rcDest.bottom;
			rcDest.bottom = dbTemp;
		}
	}

	GERect2D GETransform2D::GetRealViewport()
	{
		GERect2D rc;

		rc.left   = m_rcDisplay.left;
		rc.top    = m_rcDisplay.top;
		rc.right  = m_rcDisplay.right;
		rc.bottom = m_rcDisplay.bottom;
		GERect2D rcRet = VR2RR(rc);
		
		return rcRet;
	}

	void GETransform2D::GetRealViewport(GERect2D &rR)
	{
		GERect2D rc;

		rc.left   = m_rcDisplay.left;
		rc.top    = m_rcDisplay.top;
		rc.right  = m_rcDisplay.right;
		rc.bottom = m_rcDisplay.bottom;
		VR2RR(rc, rR);
	}

	double GETransform2D::GetScaleFit()
	{
		double ScaleX, ScaleY;

		ScaleX = m_rcDisplay.Width()/m_rcMapMBR.Width();
		ScaleY = m_rcDisplay.Height()/m_rcMapMBR.Height();

		return (ScaleX > ScaleY) ? ScaleY : ScaleX;
	}


	double GETransform2D::GetScale()
	{
		double ScaleX, ScaleY;

		GERect2D  rcView = GetViewMBR();
		GERect2D rc;
		rc = VR2RR(rcView);
		ScaleX = m_rcDisplay.Width()/rc.Width();
		ScaleY = m_rcDisplay.Height()/rc.Height();
		
		return (ScaleX > ScaleY) ? ScaleY : ScaleX; 

	}

	void GETransform2D::SetScale(double Scale)
	{
	}


	void GETransform2D::ZoomByScale(double dbScale)
	{
		double Xc = m_rcDisplay.Width()/2.0;
		double Yc = m_rcDisplay.Height()/2.0;

		m_mtR2V.Translate(-Xc, -Yc);
		m_mtR2V.Scale(dbScale, dbScale);
		m_mtR2V.Translate(Xc,Yc);
		AssignInvertionMatrix();
	}

	void GETransform2D::SetRatio(double dbRatio) //add
	{
 		double	dbScale, dbRatioFit;

		if (dbRatio > 0)
		{
			dbRatioFit	=	Scale2Ratio(GetScaleFit());
			if (dbRatio > dbRatioFit)
				dbRatio	=	dbRatioFit;
			else if (dbRatio < m_dbLimitRatio)
				dbRatio	=	m_dbLimitRatio;
			dbScale	=	Ratio2Scale(dbRatio);
			ZoomByScale(dbScale/GetScale());
		}
	}

	double GETransform2D::Scale2Ratio(double dbScale)//add
	{
		double dbUnitCm	=	Unit2Cm(m_btMapUnit)/Unit2Cm(H_CM_UNIT);
 		double dbRatio	=	(m_PixelPerCm*dbUnitCm)/dbScale;
		return	(dbRatio + 0.5);
	}

	double GETransform2D::Ratio2Scale(double dbRatio)//add
	{
		double dbUnitCm	=	Unit2Cm(m_btMapUnit)/Unit2Cm(H_CM_UNIT);
		double dbScale	=	(m_PixelPerCm*dbUnitCm)/dbRatio;
		return	dbScale; //
	}

	int GETransform2D::GetRatio()//add
	{
		return (int)Scale2Ratio(GetScale());
	}

	int GETransform2D::GetRatioFit()//add
	{
		return (int)Scale2Ratio(GetScaleFit());
	}

	double GETransform2D::Unit2Cm(BYTE btMapUnit)//add
	{
		switch (btMapUnit)
		{
			case H_MM_UNIT:				return 0.1;
			case H_M_UNIT:				return 100.0;
			case H_KM_UNIT:				return 100000.0;
			case H_INCH_UNIT:			return 100.0/39.3696;
			case H_LINK_UNIT:			return 100.0/4.970909090909;
			case H_USFOOT_UNIT:			return 100.0/3.28083333333333;
			case H_FOOT_UNIT:			return 100.0/3.2808;
			case H_YARD_UNIT:			return 100.0/1.0936;
			case H_ROD_UNIT:			return 100.0/0.1988363636364;
			case H_CHAIN_UNIT:			return 100.0/0.04970909090909;
			case H_MILE_UNIT:			return 100.0/0.000621363636364;
			case H_NAUTICAL_MILE_UNIT:	return 100.0/0.000539956803456;
			case H_DEGREE_UNIT:			return 11130446.75;
		}
		return 1.0;
	}

	void GETransform2D::SetLimitRatio(double dbLimitRatio)
	{
		m_dbLimitRatio = dbLimitRatio;
	}

	void GETransform2D::SetMapUnit(int btMapUnit)//add
	{
		m_btMapUnit = btMapUnit;
	}

	void GETransform2D::SetPixelPerCm(int nPixelPerCm)//add
	{
		m_PixelPerCm = nPixelPerCm;
	}

	int GETransform2D::GetMapUnit()//add
	{
		return m_btMapUnit;
	}

	double GETransform2D::GetPixelPerCm()//add
	{
		return m_PixelPerCm;
	}

	double GETransform2D::GetLimitRatio()//add
	{
		return m_dbLimitRatio;
	}

	GEAffine2D* GETransform2D::GetR2VAffine()
	{
		return &m_mtR2V;
	}

	GEAffine2D* GETransform2D::GetV2RAffine()
	{
		return &m_mtV2R;
	}
}  //  namespace GISolutions

