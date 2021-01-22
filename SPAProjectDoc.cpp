
// SPAProjectDoc.cpp : implementation of the CSPAProjectDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SPAProject.h"
#endif

#include "SPAProjectDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSPAProjectDoc

IMPLEMENT_DYNCREATE(CSPAProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(CSPAProjectDoc, CDocument)
END_MESSAGE_MAP()


// CSPAProjectDoc construction/destruction

CSPAProjectDoc::CSPAProjectDoc() noexcept
{
	// TODO: add one-time construction code here
	m_pExtents = NULL;
	m_GroupAction = TRANSFORM_MAP;
	m_ActionId = ID_ZOOM_CENTER;
	m_pTransformManager = NULL;

}

CSPAProjectDoc::~CSPAProjectDoc()
{
	if (m_pExtents != NULL)
		delete m_pExtents;
	m_pExtents = NULL;

	if (m_pTransformManager != NULL)
	{
		delete m_pTransformManager;
		m_pTransformManager = NULL;
	}

	ClearData(); 
}

BOOL CSPAProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CSPAProjectDoc serialization

void CSPAProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSPAProjectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSPAProjectDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSPAProjectDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSPAProjectDoc diagnostics

#ifdef _DEBUG
void CSPAProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSPAProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSPAProjectDoc commands
CView* CSPAProjectDoc::GetView(void)
{
	POSITION posView = this->GetFirstViewPosition();
	CView* pView = this->GetNextView(posView);

	return pView;
}

BOOL CSPAProjectDoc::ExecuteZoom(CPoint p, UINT nFlags)
{
	bool bRet = false;
	MapEngine::GERect2D rc;
	TransformActions action = TRANSFORM_ZOOMCENTER;

	rc.left = rc.right = p.x;
	rc.top = rc.bottom = p.y;

	switch (m_ActionId)
	{
	case ID_ZOOM_REALTIME:
		action = TRANSFORM_ZOOMREALTIME;
		break;
	case ID_ZOOM_EXTENTS:
		action = TRANSFORM_ZOOMEXTENTS;
		break;
	case ID_ZOOM_IN:
		action = TRANSFORM_ZOOMIN;
		break;
	case ID_ZOOM_OUT:
		action = TRANSFORM_ZOOMOUT;
		break;
	case ID_ZOOM_CENTER:
		action = TRANSFORM_ZOOMCENTER;
		break;
	}

	if(m_pTransformManager != NULL)
		bRet = m_pTransformManager->ExecuteAction(rc, action);

	return bRet;
}

BOOL CSPAProjectDoc::RenderAvailable(void)
{
	BOOL bAvailable = (m_pTransformManager != NULL);

	return bAvailable;
}

BOOL CSPAProjectDoc::ExcuteUserAction(CPoint p, UINT nFlags)
{
	BOOL bRet = FALSE;

	switch (m_GroupAction)
	{
	case TRANSFORM_MAP:
	{
		pClick = p; 
		isClick = true; 
		bRet = ExecuteZoom(p, nFlags);
	}
		break;
	case SELECTION_ACTIONS:
		break;
	}

	return bRet;
}

BOOL CSPAProjectDoc::ExecuteMouseWheel(CPoint p, UINT nFlags, short zDelta)
{
	BOOL bRedraw = TRUE;
	GEPoint2D pt(p.x, p.y);

	if(m_pTransformManager != NULL)
	{
		GETransform2D* pTransform = m_pTransformManager->GetMainTransform();
		if (zDelta > 0)
			pTransform->ZoomPIn(pt);
		else if (zDelta < 0)
			pTransform->ZoomPOut(pt);
		else
			bRedraw = FALSE;
	}

	return bRedraw;
}

void CSPAProjectDoc::InitMap(void)
{
	if (m_pTransformManager != NULL)
	{
		delete m_pTransformManager;
		m_pTransformManager = NULL;
	}

	if (m_pExtents != NULL)
	{
		CView* pView = GetView();

		if (pView != NULL)
		{
			CRect				rcClient;
			MapEngine::GERect2D rcView;
			MapEngine::GERect2D rcMap;

			rcMap = *m_pExtents;

			pView->GetClientRect(&rcClient);
			rcView.left = rcClient.left;
			rcView.top = rcClient.top;
			rcView.right = rcClient.right;
			rcView.bottom = rcClient.bottom;

			m_pTransformManager = new CTransformManager();
			m_pTransformManager->SetParent(pView);
			m_pTransformManager->InitMainTransform(rcMap, rcView);
			m_pTransformManager->CreateMainWindow(rcClient.Width(), rcClient.Height(), RGB(0, 0, 0));

			UpdateAllViews(NULL);
		}
	}
}

void CSPAProjectDoc::Draw()
{
	//Clear map
	if (m_pTransformManager != NULL)
	{
		GETransform2D* pTransform = m_pTransformManager->GetMainTransform();
		CDC* pDC = m_pTransformManager->GetMainDC();
		m_pTransformManager->m_MainWindow.FillBackground();
		CView* pView = GetView();
		CRect rcClient; 
		pView->GetClientRect(&rcClient); 
		MapEngine::GERect2D rcView = pTransform->GetRealViewport();

		CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen bluePen(PS_SOLID, 2, RGB(0, 0, 255));
		CPen blackPen(PS_SOLID, 2, RGB(0, 0, 0));
		CPen greenPen(PS_SOLID, 2, RGB(124, 252, 0));
		CPen whitePen(PS_SOLID, 2, RGB(255, 255, 255));

		// Draw here
		double Min[2];
		double Max[2];
		Min[0] = rcView.left;
		Max[0] = rcView.right;
		Min[1] = rcView.top;
		Max[1] = rcView.bottom;

		//draw edge
		if (m_eRTData != NULL)
		{
			Long::Edge* edge; 
			RTree<Long::Edge*, double, 2>::Iterator it_e(Min, Max); 
			m_eRTData->GetFirst(it_e);
			while (!it_e.IsNull())
			{
				edge = *it_e;
				for (int i = 0; i < edge->Nodes.size(); i++)
				{
					MapEngine::GEPoint2D v_e_1;
					MapEngine::GEPoint2D v_e_2; 
					pTransform->RP2VP(edge->Nodes[i].first->lat, edge->Nodes[i].first->lon, v_e_1); 
					pTransform->RP2VP(edge->Nodes[i].second->lat, edge->Nodes[i].second->lon, v_e_2); 
					drawEdge(v_e_1.x, v_e_1.y, v_e_2.x, v_e_2.y, pDC, pTransform, &whitePen);
				}
				m_eRTData->GetNext(it_e); 
			}
		}

		//draw node
		if (m_pRTData != NULL)
		{
			Long::Node* node;
			MapEngine::GEPoint2D vp;
			RTree<Long::Node*, double, 2>::Iterator it(Min, Max);
			m_pRTData->GetFirst(it);
			while (!it.IsNull())
			{
				node = *it; 
				pTransform->RP2VP(node->lat, node->lon, vp);
				drawNode(vp.x, vp.y, pDC, pTransform, &redPen);
				CString num;
				num.Format(L"%d", node->id);
				pDC->TextOut(vp.x + 0.5,vp.y + 0.5, num);
				m_pRTData->GetNext(it);
			}
		}
		onClick(pClick,pTransform,pDC); 
		//Draw src_Node
		MapEngine::GEPoint2D vp;
		if (src_Node.isNode())
		{
			pTransform->RP2VP(src_Node.lat, src_Node.lon, vp); 
			drawNode(vp.x, vp.y, pDC, pTransform, &greenPen); 
		}
		if (tar_Node.isNode())
		{
			pTransform->RP2VP(tar_Node.lat, tar_Node.lon, vp); 
			drawNode(vp.x, vp.y, pDC, pTransform, &greenPen);
		}

		//Find shorted_path
		//if (src_Node.isNode() && tar_Node.isNode())
		//{
		//	if (src_Node.id != old_Src_Id && src_Node.id != old_Tar_Id)
		//	{
		//		//Dijkstra
		//		Long::Dijkstra_priority_queue Dpq;
		//		Dpq.shortestPath(graph, src_Node.id, tar_Node.id);
		//		path = Dpq.getPath();
		//		printPath(path, tar_Node.id, tar_Node.id, pDC, &greenPen, pTransform);
		//		old_Src_Id = src_Node.id; 
		//		old_Tar_Id = tar_Node.id; 
		//	}
		//	else
		//	{
		//		//Draw Path
		//		printPath(path, tar_Node.id, tar_Node.id, pDC, &greenPen, pTransform); 
		//	}
		//}
		//pCatalog->Draw(pDC);
	}
}

BOOL CSPAProjectDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	/*
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
		*/
	//ClearData(); 
	init(); 
	m_strDatabaseFile = lpszPathName;

	if (m_pExtents != NULL)
	{
		delete m_pExtents;
		m_pExtents = NULL;
	}


	//Read file content
	readGraph(m_strDatabaseFile); 
	//m_pExtents = new MapEngine::GERect2D(-180.0, -90.0, 180.0, 90.0);
	m_pExtents = new MapEngine::GERect2D(graph.left,graph.top,graph.right,graph.bottom);
	InitMap();
	// TODO:  Add your specialized creation code here

	return TRUE;
}

void CSPAProjectDoc::readGraph(LPCTSTR m_strDatabaseFile)
{
	std::fstream f; 
	f.open(m_strDatabaseFile, std::ios::in); 
	if (!f)
	{
		std::cout << "Unable to open file";
		return;
	}
	m_pRTData = new RTree<Long::Node*, double, 2>;
	m_eRTData = new RTree<Long::Edge*, double, 2>;
	//read ve
	char x;
	int v;
	int e;
	int src; 
	int tar; 
	f >> x;
	f >> v;
	f >> x;
	f >> e;
	f >> src; 
	f >> tar; 
	graph.setVE(v, e);
	graph.src = src; 
	graph.target = tar; 
	//read node
	double latMin = DBL_MAX; 
	double latMax = -181; 
	double lonMin = DBL_MAX; 
	double lonMax = -181; 
	int maxID = -1; 
	double Min[2]; 
	double Max[2]; 
	Long::Node* node; 
	for (int i = 0; i < v; i++)
	{
		int id;
		double lon;
		double lat;
		f >> id;
		f >> lat;
		f >> lon;
		if (lat > latMax){latMax = lat;}
		if (lat < latMin){latMin = lat;}
		if (lon > lonMax){lonMax = lon;}
		if (lon < lonMin){lonMin = lon;}
		maxID = max(maxID, id); 
		node = new Long::Node(id, lon, lat); 
		Min[0] = Max[0] = node->lat; 
		Min[1] = Max[1] = node->lon; 
		m_pRTData->Insert(Min, Max, node);
		graph.addNode(node);
	}

	Min[0] = DBL_MAX; 
	Min[1] = DBL_MAX; 
	Max[0] = -181;
	Max[1] = -181; 

	graph.top = lonMin; 
	graph.bottom = lonMax; 
	graph.left = latMin; 
	graph.right = latMax; 
	graph.max_ID = maxID; 

	//read edges
	Long::Edge* edge; 
	for (int i = 0; i < e; i++)
	{
		int id;
		int tmp = -1;
		f >> id;
		std::string line;
		std::getline(f, line);
		std::istringstream in(line);
		in >> tmp;
		int node;
		edge = new Long::Edge(); 
		edge->id = id; 
		while (in)
		{
			in >> node;
			if (tmp != node)
			{
				Long::Node* root = graph.getNode(tmp);
				Long::Node* adjacentNode = graph.getNode(node);
				if (root != nullptr && adjacentNode != nullptr)
				{
					graph.addAdjacentNode(root, adjacentNode);
					edge->addEdge(root, adjacentNode);
					if (Min[0] > min(root->lat, adjacentNode->lat)) {Min[0] = min(root->lat, adjacentNode->lat);}
					if (Min[1] > min(root->lon, adjacentNode->lon)) {Min[1] = min(root->lon, adjacentNode->lon);}
					if (Max[0] < max(root->lat, adjacentNode->lat)) {Max[0] = max(root->lat, adjacentNode->lat);}
					if (Max[1] < max(root->lon, adjacentNode->lon)) {Max[1] = max(root->lon, adjacentNode->lon);}
					tmp = node;
				}
			}
		}
		graph.addEdge(edge); 
		m_eRTData->Insert(Min, Max, edge);
		Min[0] = DBL_MAX;
		Min[1] = DBL_MAX;
		Max[0] = -181;
		Max[1] = -181;
	} 
}

void CSPAProjectDoc::drawNode(int x, int y, CDC* pDC,GETransform2D* pTransform, CPen* Color)
{
	pDC->SelectObject(Color); 
	pDC->Ellipse((x + 0.5) - 3, (y + 0.5) - 3, (x + 0.5) + 3, (y + 0.5) + 3);
}

void CSPAProjectDoc::drawEdge(int startX, int startY, int endX, int endY, CDC* pDC, GETransform2D* pTransform, CPen* Color)
{
	pDC->SelectObject(Color); 
	pDC->MoveTo((int)(startX + 0.5), (int)(startY + 0.5));
	pDC->LineTo((int)(endX + 0.5), (int)(endY + 0.5));
}

void CSPAProjectDoc::printPath(int* parent, int j, int tmp, CDC* pDC, CPen* Color, GETransform2D* pTransform)
{
	Long::Node* node1 = graph.getNode(tmp);
	Long::Node* node2 = graph.getNode(j);

	MapEngine::GEPoint2D pStart(node1->lat, node1->lon);
	MapEngine::GEPoint2D pEnd(node2->lat, node2->lon);

	MapEngine::GEPoint2D vpStart;
	MapEngine::GEPoint2D vpEnd;

	pTransform->RP2VP(pStart, vpStart);
	pTransform->RP2VP(pEnd, vpEnd);
	if (parent[j] == -1)
	{
		drawEdge(vpStart.x, vpStart.y, vpEnd.x, vpEnd.y, pDC, pTransform, Color);
		return; 
	}
	tmp = j;
	printPath(parent,parent[j], tmp,pDC,Color,pTransform);
	drawEdge(vpStart.x, vpStart.y, vpEnd.x, vpEnd.y, pDC, pTransform, Color);
}

void CSPAProjectDoc::addNodeToEdge(Long::Node* node, Long::Node* node_1, Long::Node* node_2)
{
	graph.addNode(node); 
	graph.addAdjacentNode(node, node_1); 
	graph.addAdjacentNode(node, node_2); 
}
void CSPAProjectDoc::deleteEdge(Long::Node* src,Long::Node* tar ,epair edge_1, epair edge_2)
{
	Long::Node* node_1 = edge_1.first; 
	Long::Node* node_2 = edge_1.second;
	Long::Node* node_3 = edge_2.first;
	Long::Node* node_4 = edge_2.second;
	//delete src node
	for (int i = 0; i < graph.Nodes.size(); i++)
	{
		if (src->getId() >= graph.v)
		{
			if (node_1 != NULL)
			{
				if (graph.Nodes[i]->getId() == node_1->getId())
				{
					graph.Nodes[i]->deleteAdjacentNode(src);
				}
			}
			if (node_2 != NULL)
 			{
				if (graph.Nodes[i]->getId() == node_2->getId())
				{
					graph.Nodes[i]->deleteAdjacentNode(src);
				}
			}
		}
		if (tar->getId() >= graph.v)
		{
			if (node_3 != NULL)
			{
				if (graph.Nodes[i]->getId() == node_3->getId())
				{
					graph.Nodes[i]->deleteAdjacentNode(tar);
				}
			}
			if (node_4 != NULL)
			{
				if (graph.Nodes[i]->getId() == node_4->getId())
				{
					graph.Nodes[i]->deleteAdjacentNode(tar);
				}
			}
		}
	}
	
	if (src->getId() >= graph.v)
	{
		graph.deleteNode(src);
	}
	if (tar->getId() >= graph.v)
	{
		graph.deleteNode(tar);
	}
}
CMainFrame* CSPAProjectDoc::getMainFrame()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	return pMainWnd; 
}

void CSPAProjectDoc::ClearData()
{
	if (m_pRTData != NULL)
	{
		m_pRTData->RemoveAll();
		delete m_pRTData;
		m_pRTData = NULL;
	}
	if (m_eRTData != NULL)
	{
		m_eRTData->RemoveAll(); 
		delete m_eRTData; 
		m_eRTData = NULL; 
	}
	//delete node 
	for (auto p : graph.Nodes)
	{
		delete p; 
	}
	graph.Nodes.clear(); 
	//delete edge
	for (auto p : graph.Edges)
	{
		delete p; 
	}
	graph.Edges.clear(); 
	//delete path
	if (path != NULL)
	{
		delete[]path;
	}
}

void CSPAProjectDoc::onClick(CPoint p, GETransform2D* pTransform, CDC* pDC)
{
	if (isClick)
	{
		CPen greenPen(PS_SOLID, 2, RGB(124, 252, 0));
		CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen whitePen(PS_SOLID, 2, RGB(255, 255, 255));

		double Min_ClickE[2]; 
		double Max_ClickE[2]; 
		double Min_ClickN[2]; 
		double Max_ClickN[2]; 
		double dist = 5; 

		MapEngine::GEPoint2D min_r;
		MapEngine::GEPoint2D max_r;
		pTransform->VP2RP(p.x - dist, p.y - dist, min_r);
		pTransform->VP2RP(p.x + dist, p.y + dist, max_r); 

		Min_ClickE[0] = min_r.x;
		Max_ClickE[0] = max_r.x;
		Min_ClickE[1] = min_r.y;
		Max_ClickE[1] = max_r.y;

		Min_ClickN[0] = min(min_r.x,max_r.x); 
		Max_ClickN[0] = max(min_r.x,max_r.x); 
		Min_ClickN[1] = min(min_r.y,max_r.y); 
		Max_ClickN[1] = max(min_r.y,max_r.y); 

		//Get Point
		Long::Node n_Result = getClickedNode(pClick, pTransform, Min_ClickN, Max_ClickN);
		if (n_Result.isNode())
		{
			if (count_Click == 2)
			{
				count_Click = 0;
				src_Node.setNull();
				tar_Node.setNull();
			}
			if (count_Click == 0)
			{
				src_Node.copy(n_Result); 
			}
			if (count_Click == 1)
			{
				tar_Node.copy(n_Result); 
			}
			count_Click++; 
		}
		else
		{
			//Get Edge
			MapEngine::GEPoint2D projection_R;
			MapEngine::GEPoint2D projection_P;
			Long::Node n_Projection;
			n_Projection.setNull(); 
			Long::Edge e_Result = getClickedEdge(pClick, pTransform, Min_ClickE, Max_ClickE, projection_R,n_Projection);
			if (e_Result.isEdge())
			{
				if (is_ClickOptions)
				{
					/*pTransform->RP2VP(projection_R.x, projection_R.y, projection_P);*/
					//drawNode(projection_P.x, projection_P.y, pDC, pTransform, &greenPen);
					//Add node graph
					/*graph.addNode(&n_Projection);
					graph.v++; */
					if (count_Click == 2)
					{
						count_Click = 0;
						src_Node.setNull();
						tar_Node.setNull();
					}
					if (count_Click == 0)
					{
						src_Node.copy(n_Projection);
					}
					if (count_Click == 1)
					{
						tar_Node.copy(n_Projection);
					}
					count_Click++;
				}
				else
				{
					for (int i = 0; i < e_Result.Nodes.size(); i++)
					{
						MapEngine::GEPoint2D v_e_1;
						MapEngine::GEPoint2D v_e_2;
						pTransform->RP2VP(e_Result.Nodes[i].first->lat, e_Result.Nodes[i].first->lon, v_e_1);
						pTransform->RP2VP(e_Result.Nodes[i].second->lat, e_Result.Nodes[i].second->lon, v_e_2);
						drawEdge(v_e_1.x, v_e_1.y, v_e_2.x, v_e_2.y, pDC, pTransform, &greenPen);
					}
				}
			}
		}

		isClick = false;
	}
}
double CSPAProjectDoc::calDistFromPointToTheEdge(MapEngine::GEPoint2D p_A, MapEngine::GEPoint2D p_B, MapEngine::GEPoint2D p_C, MapEngine::GEPoint2D& projection_P)
{
	Vector v1(p_A, p_C); 
	Vector v2(p_A, p_B); 
	Vector v3; 
	v3.p1 = p_A; 
	
	v2.dx = v2.x / v2.length; 
	v2.dy = v2.y / v2.length; 

	//dot product 

	double dp = v1.x * v2.x + v1.y * v2.y;  
	double length = dp / v2.length; 
	
	v3.x = length * v2.dx; 
	v3.y = length * v2.dy; 

	v3.length = sqrt(v3.x * v3.x + v3.y * v3.y); 
	v3.p2.x = v3.p1.x + v3.x; 
	v3.p2.y = v3.p1.y + v3.y; 

	projection_P = v3.p2; 

	double d; 

	if (dp < 0)
	{
		d = -1; 
	}
	else
	{
		if (v3.length > v2.length)
		{
			d = -1; 
		}
		else
		{
			double dx = v1.x - v3.x; 
			double dy = v1.y - v3.y; 
			d = sqrt(dx * dx + dy * dy); 
		}
	}

	return d; 
}
Long::Edge CSPAProjectDoc::getClickedEdge(CPoint p, GETransform2D* pTransform, double Min_ClickE[2], double Max_ClickE[2], MapEngine::GEPoint2D& projection_P,Long::Node& n_Projection)
{
	double min_E = DBL_MAX;
	double dist_E;
	MapEngine::GEPoint2D rp_Click;
	MapEngine::GEPoint2D projection; 
	pTransform->VP2RP(p.x, p.y, rp_Click);
	Long::Edge e_Result; 
	Long::Node n_Begin; 
	Long::Node n_End; 
	e_Result.setNull(); 
	n_Begin.setNull(); 
	n_End.setNull(); 
	if (m_eRTData != NULL)
	{
		RTree<Long::Edge*, double, 2>::Iterator it_ec(Min_ClickE, Max_ClickE);
		Long::Edge* edge;
		m_eRTData->GetFirst(it_ec);
		while (!it_ec.IsNull())
		{
			edge = *it_ec;
			for (int i = 0; i < edge->Nodes.size(); i++)
			{
				MapEngine::GEPoint2D v_e_1;
				MapEngine::GEPoint2D v_e_2;
				MapEngine::GEPoint2D r_n_1(edge->Nodes[i].first->lat, edge->Nodes[i].first->lon);
				MapEngine::GEPoint2D r_n_2(edge->Nodes[i].second->lat, edge->Nodes[i].second->lon);
				dist_E = calDistFromPointToTheEdge(r_n_1, r_n_2, rp_Click, projection);
				if (dist_E != -1)
				{
					if (dist_E < min_E)
					{
						Long::Edge tmp(*edge);
						e_Result.copy(tmp);
						min_E = dist_E;
						n_Begin.copy_P(*edge->Nodes[i].first); 
						n_End.copy_P(*edge->Nodes[i].second); 
						projection_P = projection;
					}
				}
			}
			m_eRTData->GetNext(it_ec);
		}
	}
	//Create projection node
	if (n_Begin.isNode() && n_End.isNode())
	{
		n_Projection.set(graph.max_ID + 1, projection_P.x, projection_P.y);
		n_Projection.addAdjacentNode(&n_Begin);
		n_Projection.addAdjacentNode(&n_End);
	}

	return e_Result; 
}

Long::Node CSPAProjectDoc::getClickedNode(CPoint p, GETransform2D* pTransform, double Min_ClickN[2], double Max_ClickN[2])
{
	Long::Node a;
	double min_N = DBL_MAX; 
	double dist_N; 
	MapEngine::GEPoint2D rp_Click;
	pTransform->VP2RP(p.x, p.y, rp_Click);
	Long::Node n_Result;
	n_Result.setNull(); 
	if (m_pRTData != NULL)
	{
		Long::Node* node;
		RTree<Long::Node*, double, 2>::Iterator it_n(Min_ClickN, Max_ClickN);
		m_pRTData->GetFirst(it_n);
		while (!it_n.IsNull())
		{
			node = *it_n; 
			dist_N = getDistanceHaversin(rp_Click.x, rp_Click.y, node->lat, node->lon); 
			if (dist_N < min_N)
			{
				Long::Node tmp(*node); 
				n_Result.copy(tmp); 
				min_N = dist_N; 
			}
			m_pRTData->GetNext(it_n);
		}
	}
	return n_Result; 
}

void CSPAProjectDoc::init()
{
	src_Node.setNull();
	tar_Node.setNull();
	old_Src_Id = -2; 
	old_Tar_Id = -2; 
	path = NULL; 
}

double CSPAProjectDoc::getDistanceHaversin(double startLat, double startLong, double endLat, double endLong)
{

	double haversine, distance;

	double dLat, dLon;

	double DEG_RAD = 0.01745329251994;

	double R_EARTH = 6367.45;

	dLat = (endLat - startLat) * DEG_RAD;

	dLon = (endLong - startLong) * DEG_RAD;

	haversine = std::sin(dLat * 0.5) * std::sin(dLat * 0.5) +

		std::sin(dLon * 0.5) * std::sin(dLon * 0.5) *

		std::cos(startLat * DEG_RAD) *

		std::cos(endLat * DEG_RAD);

	distance = std::asin(std::sqrt(haversine)) * R_EARTH * 2.0;

	return distance;

}
