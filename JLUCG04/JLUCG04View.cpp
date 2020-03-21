
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
		DeleteObject(&pen);
		this->ReleaseDC(pDC);
	}
	else if (DrawType == 2){
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		CPen pen(PS_SOLID, 1, PolygonColor);
		pDC->SelectObject(&pen);
		pDC->MoveTo(DrawStartPoint);
		pDC->LineTo(DrawEndPoint);
		//DDALine(pDC, DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y, PolygonColor);
		DrawEndPoint = point;
		pDC->MoveTo(DrawStartPoint);
		pDC->LineTo(DrawEndPoint);
		DeleteObject(&pen);
		//DDALine(pDC, DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y, PolygonColor);
		this->ReleaseDC(pDC);
	}
	CView::OnMouseMove(nFlags, point);
}

void CJLUCG04View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (HaveClipRegion) {		//把上次图形重画一遍，覆盖掉
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		CPen pen(PS_SOLID, 1, ClipRegionColor);
		pDC->SelectObject(&pen);
		pDC->Rectangle(ClipRegionStartPoint.x, ClipRegionStartPoint.y, ClipRegionEndPoint.x, ClipRegionEndPoint.y);
		DeleteObject(&pen);
		this->ReleaseDC(pDC);
		HaveClipRegion = false;
	}
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
	HaveClipRegion = true;
	CView::OnLButtonUp(nFlags, point);
}

void CJLUCG04View::OnRButtonDown(UINT nFlags, CPoint point)
{
	DrawType = 2;
	DrawStartPoint = point;
	DrawEndPoint = point;
	TempPolygonPoints.Add(point);
	CView::OnRButtonDown(nFlags, point);
}


void CJLUCG04View::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (DrawType == 2) {
		TempPolygonPoints.Add(point);
	}
	CView::OnRButtonUp(nFlags, point);
}

void CJLUCG04View::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (DrawType == 2) {
		CDC* pDC = this->GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		CPen pen(PS_SOLID, 1, PolygonColor);
		pDC->SelectObject(&pen);
		int n = PolygonPoints.GetCount();
		if (n > 0) {//把上次图形重画一遍，覆盖掉
			pDC->MoveTo(PolygonPoints[0]);
			for (int i = 0; i < n - 1; i++) {
				pDC->LineTo(PolygonPoints[i+1]);
				//DDALine(pDC, PolygonPoints[i].x, PolygonPoints[i].y, PolygonPoints[i + 1].x, PolygonPoints[i + 1].y, PolygonColor);
			}
			pDC->LineTo(PolygonPoints[0]);
			//DDALine(pDC, PolygonPoints[n - 1].x, PolygonPoints[n - 1].y, PolygonPoints[0].x, PolygonPoints[0].y, PolygonColor);
		}
		//记录本次多边形
		DrawStartPoint = point;
		DrawEndPoint = TempPolygonPoints[0];
		pDC->MoveTo(DrawStartPoint);
		pDC->LineTo(DrawEndPoint);
		//DDALine(pDC, DrawStartPoint.x, DrawStartPoint.y, DrawEndPoint.x, DrawEndPoint.y, PolygonColor);
		DeleteObject(&pen);
		TempPolygonPoints.Add(point);
		//从缓冲写入多边形顶点
		PolygonPoints.Copy(TempPolygonPoints);
		TempPolygonPoints.RemoveAll();
		SutherlandHodgman(pDC);
		ReleaseDC(pDC);
	}
	DrawType = 0;
	CView::OnRButtonDblClk(nFlags, point);
}

void CJLUCG04View::SutherlandHodgman(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, RGB(255, 255, 255));//清屏
	pDC->SetROP2(R2_NOTXORPEN);
	int m = 0; //裁剪后的多边形的顶点数
	m += Cut_Bottom();
	m += Cut_Right();
	m += Cut_Top();
	m += Cut_Left();
	int n = ClippedPolygonPoints.GetCount();
	CPen penP(PS_SOLID, 1, PolygonColor);
	CPen penR(PS_SOLID, 1, ResultColor);
	pDC->MoveTo(ClippedPolygonPoints[0]);
	for (int i = 0; i < m - 1; i++) {
		if (IsExtraPoint[i] || IsExtraPoint[i + 1]) {
			//产生新交点，将裁剪区域重画一遍
			pDC->SelectObject(&penP);
			pDC->LineTo(ClippedPolygonPoints[i+1]);
			//DDALine(pDC, ClippedPolygonPoints[i].x, ClippedPolygonPoints[i].y, ClippedPolygonPoints[i + 1].x, ClippedPolygonPoints[i + 1].y, ClipRegionColor);
		    //这条边为新产生的结果边
			pDC->SelectObject(&penR);
			pDC->MoveTo(ClippedPolygonPoints[i]);
			pDC->LineTo(ClippedPolygonPoints[i+1]);
			//DDALine(pDC, ClippedPolygonPoints[i].x, ClippedPolygonPoints[i].y, ClippedPolygonPoints[i + 1].x, ClippedPolygonPoints[i + 1].y, ResultColor);
		}
		else {//剩余情况为：这条边的两端点均位于原多边形上，不用重画
			pDC->MoveTo(ClippedPolygonPoints[i + 1]);
		}
	}
	if (IsExtraPoint[m-1] || IsExtraPoint[0]) {	//判断最后一条边
		//产生新交点，将裁剪区域重画一遍
		//pDC->SelectObject(&penP);
		//pDC->LineTo(ClippedPolygonPoints[0]);
		//DDALine(pDC, ClippedPolygonPoints[m-1].x, ClippedPolygonPoints[m-1].y, ClippedPolygonPoints[0].x, ClippedPolygonPoints[0].y, ClipRegionColor);
		//这条边为新产生的结果边
		pDC->SelectObject(&penR);
		pDC->MoveTo(ClippedPolygonPoints[m-1]);
		pDC->LineTo(ClippedPolygonPoints[0]);
		//DDALine(pDC, ClippedPolygonPoints[m-1].x, ClippedPolygonPoints[m-1].y, ClippedPolygonPoints[0].x, ClippedPolygonPoints[0].y, ResultColor);
	}
	ClippedPolygonPoints.RemoveAll();
	IsExtraPoint.RemoveAll();
	PolygonPoints.RemoveAll();
	pDC->SelectObject(&penP);
	DeleteObject(&penP);
	pDC->SelectObject(&penR);
	DeleteObject(&penR);
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
/*
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
*/

void CJLUCG04View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'c') {
		ShowWindow(FALSE);
		ShowWindow(TRUE);
		PolygonPoints.RemoveAll();
		ClippedPolygonPoints.RemoveAll();
		TempPolygonPoints.RemoveAll();
		IsExtraPoint.RemoveAll();
		HaveClipRegion = false;
	}
	CView::OnChar(nChar, nRepCnt, nFlags);
}


int CJLUCG04View::Cut_Top()
{
	CPoint F, P, S;
	int c1, c2;
	int m = 0;
	int ty = max(ClipRegionStartPoint.y, ClipRegionEndPoint.y);
	int n = PolygonPoints.GetCount();
	for (int i = 0; i < n; i++) {
		P = PolygonPoints[i];
		if (i != 0) {
			c2 = (P.y > ty ? -1 : 1);
			if (c1 + c2 == 0) {
				CPoint I;
				I.y = ty;
				I.x = (P.x - S.x) * (I.y - S.y) / (P.y - S.y) + S.x;
				m++;
				ClippedPolygonPoints.Add(I);
				IsExtraPoint.Add(true);
			}
		}
		else {
			F = P;
		}
		S = P;
		if (S.y > ty)
			c1 = -1;
		else {
			c1 = 1;
			m++;
			ClippedPolygonPoints.Add(S);
			IsExtraPoint.Add(false);
		}
	}
	c2 = (F.y > ty ? -1 : 1);
	if (c1 + c2 == 0) {
		CPoint I;
		I.y = ty;
		I.x = (F.x - S.x)*(I.y - S.y) / (F.y - S.y) + S.x;
		m++;
		ClippedPolygonPoints.Add(I);
		IsExtraPoint.Add(true);
	}
	return m;
}


int CJLUCG04View::Cut_Right()
{
	CPoint F, P, S;
	int c1, c2;
	int m = 0;
	int rx = max(ClipRegionStartPoint.x, ClipRegionEndPoint.x);
	int n = PolygonPoints.GetCount();
	for (int i = 0; i < n; i++) {
		P = PolygonPoints[i];
		if (i != 0) {
			c2 = (P.x > rx ? -1 : 1);
			if (c1 + c2 == 0) {
				CPoint I;
				I.x = rx;
				I.y = (P.y - S.y) * (I.x - S.x) / (P.x - S.x) + S.y;
				m++;
				ClippedPolygonPoints.Add(I);
				IsExtraPoint.Add(true);
			}
		}
		else {
			F = P;
		}
		S = P;
		if (S.x > rx)
			c1 = -1;
		else {
			c1 = 1;
			m++;
			ClippedPolygonPoints.Add(S);
			IsExtraPoint.Add(false);
		}
	}
	c2 = (F.x > rx ? -1 : 1);
	if (c1 + c2 == 0) {
		CPoint I;
		I.x = rx;
		I.y = (F.y - S.y) * (I.x - S.x) / (F.x - S.x) + S.y;
		m++;
		ClippedPolygonPoints.Add(I);
		IsExtraPoint.Add(true);
	}
	return m;
}


int CJLUCG04View::Cut_Bottom()
{
	CPoint F, P, S;
	int c1, c2;
	int m = 0;
	int by = min(ClipRegionStartPoint.y, ClipRegionEndPoint.y);
	int n = PolygonPoints.GetCount();
	for (int i = 0; i < n; i++) {
		P = PolygonPoints[i];
		if (i != 0) {
			c2 = (P.y < by ? -1 : 1);
			if (c1 + c2 == 0) {
				CPoint I;
				I.y = by;
				I.x = (P.x - S.x) * (I.y - S.y) / (P.y - S.y) + S.x;
				m++;
				ClippedPolygonPoints.Add(I);
				IsExtraPoint.Add(true);
			}
		}
		else {
			F = P;
		}
		S = P;
		if (S.y < by)
			c1 = -1;
		else {
			c1 = 1;
			m++;
			ClippedPolygonPoints.Add(S);
			IsExtraPoint.Add(false);
		}
	}
	c2 = (F.y < by ? -1 : 1);
	if (c1 + c2 == 0) {
		CPoint I;
		I.y = by;
		I.x = (F.x - S.x) * (I.y - S.y) / (F.y - S.y) + S.x;
		m++;
		ClippedPolygonPoints.Add(I);
		IsExtraPoint.Add(true);
	}
	return m;
}


int CJLUCG04View::Cut_Left()
{
	CPoint F, P, S;
	int c1, c2;
	int m = 0;
	int lx = min(ClipRegionStartPoint.x, ClipRegionEndPoint.x);
	int n = PolygonPoints.GetCount();
	for (int i = 0; i < n; i++) {
		P = PolygonPoints[i];
		if (i != 0) {
			c2 = (P.x < lx ? -1 : 1);
			if (c1 + c2 == 0) {
				CPoint I;
				I.x = lx;
				I.y = (P.y - S.y) * (I.x - S.x) / (P.x - S.x) + S.y;
				m++;
				ClippedPolygonPoints.Add(I);
				IsExtraPoint.Add(true);
			}
		}
		else {
			F = P;
		}
		S = P;
		if (S.x < lx)
			c1 = -1;
		else {
			c1 = 1;
			m++;
			ClippedPolygonPoints.Add(S);
			IsExtraPoint.Add(false);
		}
	}
	c2 = (F.x < lx ? -1 : 1);
	if (c1 + c2 == 0) {
		CPoint I;
		I.x = lx;
		I.y = (F.y - S.y) * (I.x - S.x) / (F.x - S.x) + S.y;
		m++;
		ClippedPolygonPoints.Add(I);
		IsExtraPoint.Add(true);
	}
	return m;
}
