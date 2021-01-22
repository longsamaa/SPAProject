#if !defined __USERACTION__H__
#define __USERACTION__H__

#include "GE/Rect2D.h"

using namespace MapEngine;
 
class CUserAction  
{
public:
	typedef enum
	{
		SHAPE_LINE,
		SHAPE_RECTANGLE,
		SHAPE_CIRCLE,
	} DrawingShape;

	//  Constructors and Destructor
public:
	CUserAction();
	virtual ~CUserAction();

	//  Attributes
protected:
	int Max(int a, int b);
	int Min(int a, int b);
	CWnd* m_pParent;

	int      m_intStyle;
	BYTE     m_btWidth;
	COLORREF m_clrColor;

	//  Methods
public:
	void GetPen(int &iStyle, BYTE &btWidth, COLORREF &clrColor);
	void SetPen(int iStyle, BYTE btWidth, COLORREF clrColor);

	//  Virtual function
public:
	virtual void SetParent(CWnd *pWnd, CSize sz);
	virtual bool DrawShape(GERect2D &rcSelected, DrawingShape shp);
	virtual bool MoveMap(GERect2D &rcSelected, CDC* pDC);
};

#endif // !defined __USERACTION__H__
