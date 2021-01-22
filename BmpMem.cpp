//#include "stdafx.h"
#include "pch.h"
#include "BmpMem.h"

CBmpMem::CBmpMem()
{
	m_pMemDC = NULL;
	m_pBmp	 = NULL;
	m_clrBK  = RGB(255,255,255);//RGB(0,128,255);
	m_bCreateSuccess = FALSE;
#ifdef AGG_MODE
	m_pPixelMap = NULL;
#endif
}

CBmpMem::~CBmpMem()
{
	if(m_pMemDC!=NULL)
	{
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if(m_pBmp!=NULL)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}
#ifdef AGG_MODE
	if (m_pPixelMap != NULL)
	{
		delete m_pPixelMap;
		m_pPixelMap = NULL;
	}
#endif
}

bool CBmpMem::OnResize(CWnd *pWnd, CSize sz)
{
	m_bCreateSuccess = FALSE;

	try
	{
		if(m_pMemDC!=NULL)
		{
			delete m_pMemDC;
			m_pMemDC = NULL;
		}

		if(m_pBmp!=NULL)
		{
			delete m_pBmp;
			m_pBmp = NULL;
		}

#ifdef AGG_MODE
		if (m_pPixelMap != NULL)
		{
			delete m_pPixelMap;
			m_pPixelMap = NULL;
		}
#endif

		m_sz = sz;

		if((sz.cx!=0)&&(sz.cy!=0))
		{
			
			m_pMemDC =	new	CDC();
			m_pBmp	 =	new CBitmap();
			if (m_pMemDC->GetSafeHdc() == NULL)
			{
#ifdef AGG_MODE
				AGGLib2D::Image *pPixelMap;
				CClientDC	dc(pWnd);
							
				m_pMemDC->CreateCompatibleDC(&dc);
				m_pPixelMap = new AGGLib2D::Image(sz.cx, sz.cy);
				pPixelMap = m_pPixelMap;

				PBITMAPINFO pBmpInfo = pPixelMap->pmap.bitmap_info();
				m_pBmp->Attach(::CreateDIBSection(m_pMemDC->GetSafeHdc(), pBmpInfo, DIB_RGB_COLORS,(void**)&pPixelMap, 0, 0));
#else
				CClientDC	dc(pWnd);
							
				m_pMemDC->CreateCompatibleDC(&dc);
				m_pBmp->CreateCompatibleBitmap(&dc, sz.cx, sz.cy);
#endif
			
				m_pMemDC->SelectObject(m_pBmp);

				m_bCreateSuccess = TRUE;

				FillBackground();
			}
			else
			{
				if(m_pMemDC!=NULL)
				{
					delete m_pMemDC;
					m_pMemDC = NULL;
				}

				if(m_pBmp!=NULL)
				{
					delete m_pBmp;
					m_pBmp = NULL;
				}
			}
		}
	}
	catch(CException* e)
	{
		e->Delete();
	}

	return m_bCreateSuccess;
}

CDC* CBmpMem::GetDC()
{
	return m_pMemDC;
}

CBitmap* CBmpMem::GetBitmap()
{
	return m_pBmp;
}

COLORREF CBmpMem::GetColor()
{
	return m_clrBK;
}

void CBmpMem::SetColor(COLORREF clrBK)
{
	m_clrBK = clrBK;
}

void CBmpMem::FillBackground()
{
	if(m_bCreateSuccess)
	{
		CBrush br(m_clrBK);
		CBrush* nbr = m_pMemDC->SelectObject(&br);

		m_pMemDC->PatBlt(0,0, m_sz.cx, m_sz.cy, PATCOPY);

		m_pMemDC->SelectObject(nbr);
		br.DeleteObject();
	}
}

bool CBmpMem::IsSuccess()
{
	return m_bCreateSuccess;
}
