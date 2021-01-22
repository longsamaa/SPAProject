//#include "stdafx.h"
#include "pch.h"
#include "UserAction.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserAction::CUserAction()
{
	m_pParent = NULL;
	SetPen(PS_DOT, 1, RGB(255,0,0));
}

CUserAction::~CUserAction()
{

}

bool CUserAction::DrawShape(GERect2D &rcSelected, DrawingShape shp)
{
	bool bRet	   = false;
	bool bMoved    = false;
	bool bCancel   = false;
	bool bExitLoop = false;

	RECT	rcClient;
	
	m_pParent->GetClientRect(&rcClient);
	m_pParent->ClientToScreen(&rcClient);

	::ClipCursor(&rcClient);
	
	m_pParent->SetCapture();
		
	
	ASSERT(CWnd::GetCapture() == m_pParent);

	CClientDC	dc(m_pParent);
	double		tmp;
	
	CPen		pen(m_intStyle, m_btWidth, m_clrColor);
	int			nOldROP2	=	dc.SetROP2(R2_NOTXORPEN);

	CPen		*pPenOld	=	dc.SelectObject(&pen);
	CBrush		*pBrushOld	=	(CBrush *)dc.SelectStockObject(NULL_BRUSH);
	
	// Create an object for storing windows message information
	MSG	msg;

	while(!bExitLoop)
	{
		// Suspend thread until a windows message has arrived
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != m_pParent)
			break;	// break for
			
		switch (msg.message)
		{
			// handle movement/accept messages
			case WM_LBUTTONUP:
				// erase old shape
				if (bMoved)
				{
					switch (shp)
					{
						case SHAPE_LINE:
							dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
							dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
							break;
						case SHAPE_RECTANGLE:
							dc.Rectangle((int)rcSelected.left,
										 (int)rcSelected.top,
										 (int)rcSelected.right,
										 (int)rcSelected.bottom);
							break;

						case SHAPE_CIRCLE:
							tmp = sqrt((rcSelected.right - rcSelected.left) * (rcSelected.right - rcSelected.left) +
									   (rcSelected.bottom - rcSelected.top) * (rcSelected.bottom - rcSelected.top));
							dc.Ellipse((int)(rcSelected.left - tmp), (int)(rcSelected.top - tmp),
									   (int)(rcSelected.left + tmp), (int)(rcSelected.top + tmp));
							dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
							dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
							break;
					}

					bExitLoop = true;
					continue;
				}
				else
				{
					bExitLoop = true;
					continue;
				}
			case WM_MOUSEMOVE:
			{
				int	nx, ny;

				nx = LOWORD(msg.lParam);
				ny = HIWORD(msg.lParam);

				//User32.ScreenToClient(m_hDC, ref p);

				if ((rcSelected.right == nx)&&(rcSelected.bottom == ny))
					break;

				/// erase old shape
				if (bMoved)
				{

					switch (shp)
					{
						case SHAPE_LINE:
							dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
							dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
							break;
						case SHAPE_RECTANGLE:
							dc.Rectangle((int)rcSelected.left,
										 (int)rcSelected.top,
										 (int)rcSelected.right,
										 (int)rcSelected.bottom);
							break;

						case SHAPE_CIRCLE:
							tmp = sqrt((rcSelected.right - rcSelected.left) * (rcSelected.right - rcSelected.left) +
									   (rcSelected.bottom - rcSelected.top) * (rcSelected.bottom - rcSelected.top));
							dc.Ellipse((int)(rcSelected.left - tmp), (int)(rcSelected.top - tmp),
									   (int)(rcSelected.left + tmp), (int)(rcSelected.top + tmp));
							dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
							dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
							break;
					}

				}
				else
					bMoved = true;
				
				rcSelected.right  = nx;
				rcSelected.bottom = ny;

				
				// draw new shape
				switch (shp)
				{
					case SHAPE_LINE:
						dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
						dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
						break;
					case SHAPE_RECTANGLE:
						dc.Rectangle((int)rcSelected.left,
									 (int)rcSelected.top,
									 (int)rcSelected.right,
									 (int)rcSelected.bottom);
						break;
					case SHAPE_CIRCLE:
						tmp = sqrt((rcSelected.right - rcSelected.left) * (rcSelected.right - rcSelected.left) +
								   (rcSelected.bottom - rcSelected.top) * (rcSelected.bottom - rcSelected.top));
						dc.Ellipse((int)(rcSelected.left - tmp), (int)(rcSelected.top - tmp),
								   (int)(rcSelected.left + tmp), (int)(rcSelected.top + tmp));
						dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
						dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
						break;
				}

				break;
			}
				// handle cancel messages
			case WM_KEYDOWN:
				if (msg.wParam == VK_ESCAPE)
				{
					if (bMoved)
					{
						switch (shp)
						{
							case SHAPE_LINE:
								dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
								dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
								break;
							case SHAPE_RECTANGLE:
								dc.Rectangle((int)rcSelected.left,
											 (int)rcSelected.top,
											 (int)rcSelected.right,
											 (int)rcSelected.bottom);
								break;

							case SHAPE_CIRCLE:
								tmp = sqrt((rcSelected.right - rcSelected.left) * (rcSelected.right - rcSelected.left)+
										   (rcSelected.bottom - rcSelected.top) * (rcSelected.bottom - rcSelected.top));
								dc.Ellipse((int)(rcSelected.left - tmp), (int)(rcSelected.top - tmp),
										   (int)(rcSelected.left + tmp), (int)(rcSelected.top + tmp));
								dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
								dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
								break;
						}
					}
					bCancel =	true;
					bExitLoop = true;
				}
				break;
			case WM_RBUTTONDOWN:
				if (bMoved)
				{
					switch (shp)
					{
						case SHAPE_LINE:
							dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
							dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
							break;
						case SHAPE_RECTANGLE:
							dc.Rectangle((int)rcSelected.left,
										 (int)rcSelected.top,
										 (int)rcSelected.right,
										 (int)rcSelected.bottom);
							break;

						case SHAPE_CIRCLE:
							tmp = sqrt((rcSelected.right - rcSelected.left) * (rcSelected.right - rcSelected.left) +
									   (rcSelected.bottom - rcSelected.top) * (rcSelected.bottom - rcSelected.top));
							dc.Ellipse((int)(rcSelected.left - tmp), (int)(rcSelected.top - tmp),
									   (int)(rcSelected.left + tmp), (int)(rcSelected.top + tmp));
							dc.MoveTo((int)rcSelected.left, (int)rcSelected.top);
							dc.LineTo((int)rcSelected.right, (int)rcSelected.bottom);
							break;
					}
				}
				bCancel =	true;
				bExitLoop = true;
				break;
			default:
				DispatchMessage(&msg);
				break;
		}
	}

	bRet = !bCancel;

	if(shp!=SHAPE_LINE)
	{
		GERect2D rc = rcSelected;

		rcSelected.left   = min(rc.left, rc.right);
		rcSelected.right  = max(rc.left, rc.right);
		rcSelected.top    = min(rc.top, rc.bottom);
		rcSelected.bottom = max(rc.top, rc.bottom);
	}
	
	::ClipCursor(NULL);
	ReleaseCapture();

	
	dc.SelectObject(pBrushOld);
	dc.SelectObject(pPenOld);
	dc.SetROP2(nOldROP2);

	return bRet;
}

void CUserAction::SetParent(CWnd *pWnd, CSize sz)
{
	m_pParent = pWnd;
}

void CUserAction::SetPen(int iStyle, BYTE btWidth, COLORREF clrColor)
{
	m_intStyle = iStyle;
	m_btWidth  = btWidth;
	m_clrColor = clrColor;
}

void CUserAction::GetPen(int &iStyle, BYTE &btWidth, COLORREF &clrColor)
{
	iStyle   = m_intStyle;
	btWidth  = m_btWidth;
	clrColor = m_clrColor;
}

bool CUserAction::MoveMap(GERect2D &rcSelected, CDC* pDC)
{
	bool bRet	   = false;
	bool bMoved    = false;
	bool bCancel   = false;
	bool bExitLoop = false;
	int h;
	int w;
	RECT	rcClient;
	
	m_pParent->GetClientRect(&rcClient);
	m_pParent->ClientToScreen(&rcClient);

	::ClipCursor(&rcClient);

	m_pParent->GetClientRect(&rcClient);

	h = rcClient.bottom - rcClient.top;
	w = rcClient.right - rcClient.left;

	m_pParent->SetCapture();
	
	ASSERT(CWnd::GetCapture() == m_pParent);

	CClientDC	dc(m_pParent);
	
	// Create an object for storing windows message information
	MSG	msg;

	while(!bExitLoop)
	{
		// Suspend thread until a windows message has arrived
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != m_pParent)
			break;	// break for
	
		switch (msg.message)
		{
			// handle movement/accept messages
			case WM_LBUTTONUP:
				// erase old shape
					bExitLoop = true;
					continue;
			case WM_MOUSEMOVE:
			{
				int	nx, ny;

				nx = LOWORD(msg.lParam);
				ny = HIWORD(msg.lParam);

				if ((rcSelected.right == nx)&&(rcSelected.bottom == ny))
					break;

				if (bMoved)
				{
					nx-= (int)rcSelected.left;
					ny-= (int)rcSelected.top;

					if((w > 0)&&(ny > 0))
						dc.PatBlt(rcClient.left, rcClient.top, w, ny, WHITENESS);

					if(((-nx) > 0)&&(h > 0))
						dc.PatBlt(rcClient.right+nx, ny, -nx, h, WHITENESS);

					if((nx > 0)&&(h > 0))
						dc.PatBlt(rcClient.left, ny, nx, h, WHITENESS);

					if((w > 0)&&((-ny) > 0))
						dc.PatBlt(rcClient.left, rcClient.bottom+ny, w, -ny, WHITENESS);
					
					VERIFY(dc.BitBlt(0, 0, rcClient.right-rcClient.left + 1, rcClient.bottom - rcClient.top + 1, pDC, -nx, -ny, SRCCOPY));
				}
				else
					bMoved = true;
				
				rcSelected.right  =	LOWORD(msg.lParam);
				rcSelected.bottom =	HIWORD(msg.lParam);
			
				break;
			}
				// handle cancel messages
			case WM_KEYDOWN:
				if (msg.wParam == VK_ESCAPE)
				{
					bMoved = false;
					bCancel = true;
					bExitLoop = true;
				}
				break;
			case WM_RBUTTONDOWN:
				bCancel =	true;
				bExitLoop = true;
				break;
			default:
				DispatchMessage(&msg);
				break;
		}
	}

	bRet = bMoved;
	
	::ClipCursor(NULL);
	ReleaseCapture();

	return bRet;
}