
// SPAProjectView.cpp : implementation of the CSPAProjectView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SPAProject.h"
#endif

#include "SPAProjectDoc.h"
#include "SPAProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSPAProjectView

IMPLEMENT_DYNCREATE(CSPAProjectView, CView)

BEGIN_MESSAGE_MAP(CSPAProjectView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSPAProjectView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_ZOOM_ALL, ID_ZOOM_CENTER, &CSPAProjectView::OnZoom)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ZOOM_ALL, ID_ZOOM_CENTER, &CSPAProjectView::OnUpdateZoom)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CSPAProjectView construction/destruction

CSPAProjectView::CSPAProjectView() noexcept
{
	m_bRedraw = TRUE;

}

CSPAProjectView::~CSPAProjectView()
{
}

BOOL CSPAProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSPAProjectView drawing

void CSPAProjectView::OnDraw(CDC* pDC)
{
	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->RenderAvailable())
	{
		CRect rectUpdate;

		if (m_bRedraw)
		{
			pDoc->Draw();
			m_bRedraw = FALSE;
		}

		int nType = pDC->GetClipBox(rectUpdate);

		if ((nType == NULLREGION) || (nType == ERROR))
			pDC->SelectClipRgn(NULL);

		pDC->GetClipBox(rectUpdate);

		CDC* pMemDC = pDoc->m_pTransformManager->GetMainDC();

		pDC->BitBlt(rectUpdate.left,
			rectUpdate.top,
			rectUpdate.Width(),
			rectUpdate.Height(),
			pMemDC,
			rectUpdate.left,
			rectUpdate.top,
			SRCCOPY);
	}
}


// CSPAProjectView printing


void CSPAProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSPAProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSPAProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSPAProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSPAProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSPAProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_TRANSFORMATION, point.x, point.y, this, TRUE);
#endif
}


// CSPAProjectView diagnostics

#ifdef _DEBUG
void CSPAProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CSPAProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSPAProjectDoc* CSPAProjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSPAProjectDoc)));
	return (CSPAProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CSPAProjectView message handlers


void CSPAProjectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->RenderAvailable())
	{
		CRect rcClient;

		GetClientRect(&rcClient);
		
		pDoc->m_pTransformManager->CreateMainWindow(rcClient.Width(), rcClient.Height(), RGB(255, 255, 255));
	}

	m_bRedraw = TRUE;
}


BOOL CSPAProjectView::OnEraseBkgnd(CDC* pDC)
{
	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc != NULL)
	{
		if (pDoc->RenderAvailable())
			return TRUE;
	}

	return CView::OnEraseBkgnd(pDC);
}

void CSPAProjectView::OnZoom(UINT nID)
{
	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (nID == ID_ZOOM_ALL)
	{
		pDoc->m_pTransformManager->ZoomAll();

		m_bRedraw = TRUE;
		Invalidate(FALSE);
		return;
	}
	else
	{
		pDoc->m_GroupAction = TRANSFORM_MAP;
		pDoc->m_ActionId = nID;
	}
}

void CSPAProjectView::OnUpdateZoom(CCmdUI* pCmdUI)
{
	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL bEnabled = pDoc->RenderAvailable();

	if (bEnabled)
	{
		pCmdUI->SetCheck((pDoc->m_GroupAction == TRANSFORM_MAP) && (pDoc->m_ActionId == pCmdUI->m_nID));
	}

	pCmdUI->Enable(bEnabled);
}


void CSPAProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->RenderAvailable())
	{
		if (pDoc->ExcuteUserAction(point, nFlags))
		{
			m_bRedraw = TRUE;
			Invalidate(FALSE);
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


BOOL CSPAProjectView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CSPAProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->RenderAvailable())
	{
		ScreenToClient(&pt);
		CRect rcClient;
		GetClientRect(&rcClient);
		POINT p = pt;

		if (rcClient.PtInRect(p))
		{
			if (pDoc->ExecuteMouseWheel(pt, nFlags, zDelta))
			{
				m_bRedraw = TRUE;
				Invalidate(FALSE);
			}
		}
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
