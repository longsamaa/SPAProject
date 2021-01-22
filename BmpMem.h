#if !defined __BMPMEM__H__
#define __BMPMEM__H__

#ifdef AGG_MODE
#include "agg/Agg_Lib2D.h"
#endif

class CBmpMem  
{
public:
	CBmpMem();
	virtual ~CBmpMem();

public:
	bool IsSuccess();
	bool m_bCreateSuccess;
	CSize m_sz;
	void FillBackground();
	void SetColor(COLORREF clrBK);
	COLORREF GetColor();
	CBitmap* GetBitmap();
	CDC* GetDC();
	bool OnResize(CWnd* pWnd, CSize sz);

private:
	CDC*				m_pMemDC;
	CBitmap*			m_pBmp, *m_pOldBmp;
	COLORREF			m_clrBK;
#ifdef AGG_MODE
	AGGLib2D::Image		*m_pPixelMap;
#endif
};

#endif // !defined __BMPMEM__H__
