#if !defined __TRANSFORM2D__H__
#define __TRANSFORM2D__H__

#include "Define.h"
#include "Affine2D.h"
#include "Point2D.h"
#include "Rect2D.h"

namespace MapEngine
{
	class GETransform2D  
	{
	public:
		GETransform2D();
		virtual ~GETransform2D();

	//private:
	public:
		GEAffine2D m_mtV2R;
		GEAffine2D m_mtR2V;
		GERect2D m_rcMapMBR;
		GERect2D m_rcDisplay;

		BYTE m_btMapUnit;
		double m_dbLimitRatio;
		double m_PixelPerCm;

	public:
		GERect2D RR2VR(const GERect2D &rc);
		void RR2VR(const GERect2D &rc, GERect2D &r);
		GEPoint2D VP2RP(const GEPoint2D &p);
		void VP2RP(const GEPoint2D &p, GEPoint2D &P);
		GEPoint2D RP2VP(const GEPoint2D &p);
		void RP2VP(const GEPoint2D &p, GEPoint2D &P);
		GEPoint2D VP2RP(const double &x, const double &y);
		void VP2RP(const double &x, const double &y, GEPoint2D &P);
		GERect2D GetViewMBR();
		GERect2D GetMapMBR();
		void SetViewMBR(const GERect2D &rcMBR);
		void SetMapMBR(const GERect2D &rcMBR);
		bool AssignInvertionMatrix();
		double GetScale();
		void SetScale(double Scale);
		double GetScaleFit();
		GERect2D GetRealViewport();
		void GetRealViewport(GERect2D &rR);
		GERect2D VR2RR(const GERect2D &rc);
		void VR2RR(const GERect2D &rc, GERect2D &rcDest);
		GEPoint2D RP2VP(const double &x, const double &y);
		void RP2VP(const double &x, const double &y, GEPoint2D &P);
		void ZoomWOut(GERect2D &rc);
		void ZoomWIn(GERect2D &rc);
		void ZoomPOut(const GEPoint2D& p, int Scale);
		void ZoomPIn(const GEPoint2D& p, int Scale);
		void ZoomPIn(const GEPoint2D &p);
		void ZoomPOut(const GEPoint2D &p);
		void ZoomCenter(const GEPoint2D &p);
		void ZoomFit();
		void Translate(GERect2D &rc);
		void ZoomWindow(GERect2D &rc);
		void ZoomByScale(double dbScale);

		void SetRatio(double dbRatio);
		int GetRatio();
		int GetRatioFit();

		void SetLimitRatio(double dbLimitRatio);
		void SetMapUnit(int btMapUnit);
		void SetPixelPerCm(int nPixelPerCm);
		int GetMapUnit();
		double GetPixelPerCm();
		double GetLimitRatio();

		GEAffine2D* GetR2VAffine();
		GEAffine2D* GetV2RAffine();

	private:
		double Scale2Ratio(double dbScale);
		double Ratio2Scale(double dbRatio);
		double Unit2Cm(BYTE btMapUnit);
	};
} //  namespace GISolutions
#endif // !defined __TRANSFORM2D__H__
