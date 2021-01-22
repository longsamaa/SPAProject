#if !defined __TRANSFORMMANAGER__H__
#define __TRANSFORMMANAGER__H__

#include "BmpMem.h"
#include "UserAction.h"
#include "GE\Transform2D.h"
#include "TransformActions.h"

class CTransformManager  
{
public:
	CTransformManager();
	virtual ~CTransformManager();

public:
	GETransform2D  m_MWTransform;//main window
	GETransform2D  m_SWTransform;//sub window
	CBmpMem		   m_MainWindow;
	CBmpMem		   m_SubWindow;

public:
	GEPoint2D SP2MP(GEPoint2D p);
	GERect2D SR2MR(GERect2D r);
	GERect2D GetSubViewport();
	GETransform2D* GetSubTransform();
	GETransform2D* GetMainTransform();
	bool InitMainTransform(GERect2D rcMBR, GERect2D rcView);
	bool InitSubTransform(GERect2D rcMBR, GERect2D rcView); 	
	
	CDC* GetSubDC();
	CDC* GetMainDC();

	void SetParent(CWnd* pParent);
	
	bool CreateSubWindow(int nWidth, int nHeight, COLORREF clrBK);
	bool CreateMainWindow(int nWidth, int nHeight, COLORREF clrBK);

	bool ShowCircularSelector(GERect2D &rc);
	bool ShowRectSelector(GERect2D &rc);
	bool ShowLineSelector(GERect2D &rc);
	bool ShowDistance(GERect2D &rc);

	bool ExecuteAction(GERect2D &rc, TransformActions action);
	bool ZoomAll();

	void SetMapUnit(BYTE btMapUnit);
	void SetLimitRatio(double dbLimitRatio);
	void SetPixelPerCm(int nPixelPerCm);

private:
	CWnd* m_pParent;
	bool SetSubWindow(GERect2D rcWindow);
	bool SetMainWindow(GERect2D rcWindow);
	bool ZoomCenter(GEPoint2D p);
	bool ZoomRealtime(GERect2D &rc);
	bool ZoomExtents(GERect2D &rc);
	bool ZoomIn(GERect2D &rc);
	bool ZoomOut(GERect2D &rc);
	
	CUserAction   *m_pUserAction;
};

#endif // !defined __TRANSFORMMANAGER__H__
