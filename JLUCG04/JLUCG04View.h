
// JLUCG04View.h: CJLUCG04View 类的接口
//

#pragma once


class CJLUCG04View : public CView
{
protected: // 仅从序列化创建
	CJLUCG04View() noexcept;
	DECLARE_DYNCREATE(CJLUCG04View)

// 特性
public:
	CJLUCG04Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CJLUCG04View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
	COLORREF ClipRegionColor = RGB(255, 0, 0); //裁剪区域颜色
	COLORREF PolygonColor = RGB(0, 255, 0); //多边形边框颜色
	COLORREF ResultColor = RGB(0, 0, 255); //结果颜色
	CPoint ClipRegionStartPoint; //裁剪区域起点
	CPoint ClipRegionEndPoint; //裁剪区域终点
	CArray<CPoint, CPoint> PolygonPoints; //原多边形
	CArray<CPoint, CPoint> ClippedPolygonPoints; //裁剪后的多边形
	CPoint DrawStartPoint; //画图起点
	CPoint DrawEndPoint; //画图终点
	int DrawType = 0; //0代表不画线，1代表画裁剪区域，2代表画多边形
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
private:
	void SutherlandHodgman(CDC* pDC);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // JLUCG04View.cpp 中的调试版本
inline CJLUCG04Doc* CJLUCG04View::GetDocument() const
   { return reinterpret_cast<CJLUCG04Doc*>(m_pDocument); }
#endif

