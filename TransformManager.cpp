#include "pch.h"
//#include "stdafx.h"
#include "TransformManager.h"
#include "MainFrm.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransformManager::CTransformManager()
{
	m_pUserAction = new CUserAction;
}

CTransformManager::~CTransformManager()
{
	if (m_pUserAction != NULL)
	{
		delete m_pUserAction;
		m_pUserAction = NULL;
	}
}

bool CTransformManager::ExecuteAction(GERect2D& rc, TransformActions action)
{
	bool bRet = FALSE;
	GEPoint2D p;

	p.x = rc.left;
	p.y = rc.top;

	switch (action)
	{
	case TRANSFORM_ZOOMEXTENTS:
		bRet = ZoomExtents(rc);
		break;
	case TRANSFORM_ZOOMIN:
		bRet = ZoomIn(rc);
		break;
	case TRANSFORM_ZOOMOUT:
		bRet = ZoomOut(rc);
		break;
	case TRANSFORM_ZOOMREALTIME:
		bRet = ZoomRealtime(rc);
		break;
	case TRANSFORM_ZOOMCENTER:
		bRet = ZoomCenter(p);
		break;
	default:
		break;
	}

	return bRet;
}

bool CTransformManager::ZoomAll()
{
	bool bRet = true;

	m_MWTransform.ZoomFit();
	m_SWTransform.ZoomFit();

	return bRet;
}

bool CTransformManager::ZoomExtents(GERect2D& rc)
{
	bool bRet = m_pUserAction->MoveMap(rc, m_MainWindow.GetDC());

	if (bRet)
	{
		m_MWTransform.Translate(rc);
	}

	return true;
}

bool CTransformManager::ZoomIn(GERect2D& rc)
{
	bool bRet = false;

	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_RECTANGLE);

	if (bRet)
	{

		if ((fabs(rc.right - rc.left) < 10) || (fabs(rc.bottom - rc.top) < 10))
		{
			GEPoint2D p;

			p.x = rc.left;
			p.y = rc.top;

			m_MWTransform.ZoomPIn(p);
		}
		else
		{
			m_MWTransform.ZoomWIn(rc);
		}

	}

	return bRet;
}

bool CTransformManager::ZoomOut(GERect2D& rc)
{
	bool bRet = false;

	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_RECTANGLE);

	if (bRet)
	{
		if ((fabs(rc.right - rc.left) < 10) || (fabs(rc.bottom - rc.top) < 10))
		{
			GEPoint2D p;

			p.x = rc.left;
			p.y = rc.top;

			m_MWTransform.ZoomPOut(p);
		}
		else
		{
			m_MWTransform.ZoomWOut(rc);
		}
	}

	return bRet;
}

bool CTransformManager::ZoomRealtime(GERect2D& rc)
{
	bool bRet = false;

	GEPoint2D p;

	p.x = rc.left;
	p.y = rc.top;


	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_RECTANGLE);

	if (!bRet)
		return bRet;

	bRet = false;

	if (!((fabs(rc.right - rc.left) < 5) || (fabs(rc.bottom - rc.top) < 5)))
	{
		if (!(p.y == rc.top))
			m_MWTransform.ZoomWOut(rc);
		else
			m_MWTransform.ZoomWIn(rc);

		bRet = true;
	}

	return bRet;
}

bool CTransformManager::ZoomCenter(GEPoint2D p)
{
	bool bRet = true;

	GEPoint2D q;

	q.x = p.x;
	q.y = p.y;

	m_MWTransform.ZoomCenter(q);

	return bRet;
}

bool CTransformManager::ShowDistance(GERect2D& rc)
{
	bool bRet = true;

	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_LINE);

	return bRet;
}

bool CTransformManager::ShowLineSelector(GERect2D& rc)
{
	bool bRet = true;

	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_LINE);

	return bRet;
}

bool CTransformManager::ShowRectSelector(GERect2D& rc)
{
	bool bRet = false;

	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_RECTANGLE);

	return bRet;
}

bool CTransformManager::ShowCircularSelector(GERect2D& rc)
{
	bool bRet = false;

	bRet = m_pUserAction->DrawShape(rc, CUserAction::SHAPE_CIRCLE);

	return bRet;
}

bool CTransformManager::SetMainWindow(GERect2D rcWindow)
{
	bool bRet = true;

	m_MWTransform.SetViewMBR(rcWindow);

	return bRet;
}

bool CTransformManager::SetSubWindow(GERect2D rcWindow)
{
	bool bRet = true;

	m_SWTransform.SetViewMBR(rcWindow);

	return bRet;
}

bool CTransformManager::CreateMainWindow(int nWidth, int nHeight, COLORREF clrBK)
{
	bool bRet = false;

	CSize sz(nWidth, nHeight);
	GERect2D rc;

	rc.left = 0;
	rc.top = 0;
	rc.right = nWidth;
	rc.bottom = nHeight;

	m_MainWindow.SetColor(clrBK);

	bRet = m_MainWindow.OnResize(m_pParent, sz);
	bRet &= SetMainWindow(rc);

	m_pUserAction->SetParent(m_pParent, sz);

	return bRet;
}

bool CTransformManager::CreateSubWindow(int nWidth, int nHeight, COLORREF clrBK)
{
	bool bRet = false;

	CSize sz(nWidth, nHeight);
	GERect2D rc;

	rc.left = 0;
	rc.top = 0;
	rc.right = nWidth;
	rc.bottom = nHeight;

	m_SubWindow.SetColor(clrBK);

	bRet = m_SubWindow.OnResize(m_pParent, sz);

	bRet &= SetSubWindow(rc);

	return bRet;
}

void CTransformManager::SetParent(CWnd* pParent)
{
	m_pParent = pParent;
}

CDC* CTransformManager::GetMainDC()
{
	return m_MainWindow.GetDC();
}

CDC* CTransformManager::GetSubDC()
{
	return m_SubWindow.GetDC();
}

bool CTransformManager::InitSubTransform(GERect2D rcMBR, GERect2D rcView)
{
	m_SWTransform.SetMapMBR(rcMBR);
	m_SWTransform.SetViewMBR(rcView);
	m_SWTransform.ZoomFit();
	return true;
}

bool CTransformManager::InitMainTransform(GERect2D rcMBR, GERect2D rcView)
{
	m_MWTransform.SetMapMBR(rcMBR);
	m_MWTransform.SetViewMBR(rcView);
	m_MWTransform.ZoomFit();
	return true;
}

GETransform2D* CTransformManager::GetMainTransform()
{
	return &m_MWTransform;
}

GETransform2D* CTransformManager::GetSubTransform()
{
	return &m_SWTransform;
}

GERect2D CTransformManager::GetSubViewport()
{
	GERect2D r = m_MWTransform.GetViewMBR();
	GERect2D Ev = m_MWTransform.VR2RR(r);
	r = m_SWTransform.RR2VR(Ev);
	return r;
}

GEPoint2D CTransformManager::SP2MP(GEPoint2D p)
{
	GEPoint2D rp;
	GEPoint2D vp;

	vp.x = p.x;
	vp.y = p.y;
	rp = m_SWTransform.VP2RP(vp);
	return m_MWTransform.RP2VP(rp);
}

GERect2D CTransformManager::SR2MR(GERect2D r)
{
	GERect2D rr;
	GERect2D vr;

	vr.left = r.left;
	vr.top = r.top;
	vr.right = r.right;
	vr.bottom = r.bottom;

	rr = m_SWTransform.VR2RR(vr);
	return m_MWTransform.RR2VR(rr);
}

void CTransformManager::SetMapUnit(BYTE btMapUnit)
{
	m_MWTransform.SetMapUnit(btMapUnit);
	m_SWTransform.SetMapUnit(btMapUnit);
}

void CTransformManager::SetLimitRatio(double dbLimitRatio)
{
	m_MWTransform.SetLimitRatio(dbLimitRatio);
	m_SWTransform.SetLimitRatio(dbLimitRatio);
}

void CTransformManager::SetPixelPerCm(int nPixelPerCm)
{
	m_MWTransform.SetPixelPerCm(nPixelPerCm);
	m_SWTransform.SetPixelPerCm(nPixelPerCm);
}
