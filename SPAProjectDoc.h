
// SPAProjectDoc.h : interface of the CSPAProjectDoc class
//


#pragma once
#include "TransformManager.h"
#include "Long.h"
#include "PropertiesWnd.h"
#include "MainFrm.h"
#include "GE/RTree.h"

struct edge_F
{
	Long::Node n_Begin; 
	Long::Node n_End; 
	Long::Node n_Projection; 
	int id; 
};

struct Vector
{
	Vector()
	{

	}
	Vector(MapEngine::GEPoint2D _p1, MapEngine::GEPoint2D _p2) : p1(_p1), p2(_p2)
	{
		x = (_p2.x - _p1.x); 
		y = (_p2.y - _p1.y); 
		length = sqrt(x * x + y * y); 
	}
	MapEngine::GEPoint2D p1; 
	MapEngine::GEPoint2D p2;
	double x; 
	double y; 
	double length; 
	double dx; 
	double dy; 
};

class CSPAProjectDoc : public CDocument
{
protected: // create from serialization only
	CSPAProjectDoc() noexcept;
	DECLARE_DYNCREATE(CSPAProjectDoc)

// Attributes
public:
	CTransformManager* m_pTransformManager;
	Long::Graph graph; 
	RTree<Long::Node*, double, 2>* m_pRTData; 
	RTree<Long::Edge*, double, 2>* m_eRTData;  
	Long::Node src_Node; 
	Long::Node tar_Node;
	int old_Src_Id; 
	int old_Tar_Id; 
	bool is_ClickOptions = true; 
	epair edge_1;
	epair edge_2; 
	CPoint pClick;
	bool isClick = false; 
	int count_Click = 0; 
	int* path; 
public:
	CString				 m_strDatabaseFile;
	MapEngine::GERect2D* m_pExtents;

	UINT				 m_ActionId;
	UINT				 m_GroupAction;

public:
	void InitMap(void);
	BOOL RenderAvailable(void);
	void Draw(void);
	BOOL ExcuteUserAction(CPoint p, UINT nFlags); 

private:
	CView* GetView(void);
	BOOL ExecuteZoom(CPoint p, UINT nFlags);
	BOOL ExecuteSelection(CPoint p, UINT nFlags);
	void readGraph(LPCTSTR m_strDatabaseFile);
	void drawNode(int x, int y, CDC* pDC, GETransform2D* pTransform, CPen* Color);
	void drawEdge(int startX, int startY, int endX, int endY, CDC* pDC, GETransform2D* pTransform, CPen* Color); 
	void printPath(int* parent,int j,int tmp,CDC* pDC,CPen* Color, GETransform2D* pTransform);
	void addNodeToEdge(Long::Node* node, Long::Node* node_1, Long::Node* node_2);
	void deleteEdge(Long::Node* src, Long::Node* tar, epair edge_1, epair edge_2);
	CMainFrame* getMainFrame();
	void onClick(CPoint p, GETransform2D* pTransform, CDC* pDC);
	void ClearData(); 
	double calDistFromPointToTheEdge(MapEngine::GEPoint2D p_A, MapEngine::GEPoint2D p_B, MapEngine::GEPoint2D p_C, MapEngine::GEPoint2D &projection_P); 
	Long::Edge getClickedEdge(CPoint p, GETransform2D* pTransform, double Min_ClickE[2], double Max_ClickE[2], MapEngine::GEPoint2D& projection_P, Long::Node& n_Projection);
	Long::Node getClickedNode(CPoint p, GETransform2D* pTransform, double Min_ClickN[2], double Max_ClickN[2]); 
	double getDistanceHaversin(double startLat, double startLong, double endLat, double endLong); 
	void init(); 
public:
	BOOL ExecuteMouseWheel(CPoint p, UINT nFlags, short zDelta);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSPAProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
