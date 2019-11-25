
// JLUCG04View.cpp: CJLUCG04View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "JLUCG04.h"
#endif

#include "JLUCG04Doc.h"
#include "JLUCG04View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJLUCG04View

IMPLEMENT_DYNCREATE(CJLUCG04View, CView)

BEGIN_MESSAGE_MAP(CJLUCG04View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CJLUCG04View 构造/析构

CJLUCG04View::CJLUCG04View() noexcept
{
	// TODO: 在此处添加构造代码

}

CJLUCG04View::~CJLUCG04View()
{
}

BOOL CJLUCG04View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CJLUCG04View 绘图

void CJLUCG04View::OnDraw(CDC* pDC)
{
	CJLUCG04Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
}

void CJLUCG04View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (DrawType == 1) {
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		CPen pen(PS_SOLID, 1, ClipRegionColor);
		pDC->SelectObject(&pen);
		pDC->Rectangle(DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y);
		DrawEndPoint = point;
		pDC->Rectangle(DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y);
		pDC->SelectObject(&pen);
		DeleteObject(&pen);
		this->ReleaseDC(pDC);
	}
	else if (DrawType == 2){
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		DDALine(pDC, DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y, PolygonColor);
		DrawEndPoint = point;
		DDALine(pDC, DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y, PolygonColor);
		this->ReleaseDC(pDC);
	}
	CView::OnMouseMove(nFlags, point);
}

void CJLUCG04View::OnLButtonDown(UINT nFlags, CPoint point)
{
	DrawType = 1;
	DrawStartPoint = point;
	DrawEndPoint = point;
	ClipRegionStartPoint = point;
	CView::OnLButtonDown(nFlags, point);
}

void CJLUCG04View::OnLButtonUp(UINT nFlags, CPoint point)
{
	DrawEndPoint = point;
	ClipRegionEndPoint = point;
	DrawType = 0;
	CView::OnLButtonUp(nFlags, point);
}

void CJLUCG04View::OnRButtonDown(UINT nFlags, CPoint point)
{
	DrawType = 2;
	DrawStartPoint = point;
	DrawEndPoint = point;
	PolygonPoints.Add(point);
	CView::OnRButtonDown(nFlags, point);
}


void CJLUCG04View::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (DrawType == 2) {
		PolygonPoints.Add(point);
	}
	CView::OnRButtonUp(nFlags, point);
}

void CJLUCG04View::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (DrawType == 2) {
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		DrawStartPoint = point;
		DrawEndPoint = PolygonPoints[0];
		DDALine(pDC, DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y, PolygonColor);
		SutherlandHodgman(pDC);
		ReleaseDC(pDC);
	}
	DrawType = 0;
	CView::OnRButtonDblClk(nFlags, point);
}

void CJLUCG04View::SutherlandHodgman(CDC* pDC)
{
	// TODO: 在此处添加实现代码.

}
// CJLUCG04View 打印

BOOL CJLUCG04View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CJLUCG04View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CJLUCG04View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

// CJLUCG04View 诊断

#ifdef _DEBUG
void CJLUCG04View::AssertValid() const
{
	CView::AssertValid();
}

void CJLUCG04View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJLUCG04Doc* CJLUCG04View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJLUCG04Doc)));
	return (CJLUCG04Doc*)m_pDocument;
}
#endif //_DEBUG

// CJLUCG04View 消息处理程序

void CJLUCG04View::DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}


void CJLUCG04View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'c') {
		this->Invalidate(); //清屏
		this->UpdateWindow();
		PolygonPoints.RemoveAll();
		ClippedPolygonPoints.RemoveAll();
	}
	CView::OnChar(nChar, nRepCnt, nFlags);
}
