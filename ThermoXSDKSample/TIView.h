// TIView.h : interface of the CTIView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIVIEW_H__3DC8E742_1B0F_483C_813B_A8A580AF7AA8__INCLUDED_)
#define AFX_TIVIEW_H__3DC8E742_1B0F_483C_813B_A8A580AF7AA8__INCLUDED_

#include "DlgHeatData.h"
#include "DlgHeatImage.h"
#include "DlgTongCalib.h"
#include "DlgRadiaInfo.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define POINT_NUM 2


class CTIView : public CScrollView
{
protected: // create from serialization only
	CTIView();
	DECLARE_DYNCREATE(CTIView)

// Attributes
public:
	CTIDoc* GetDocument();

	CDlgHeatData m_heatdatadlg;
	CDlgHeatImage m_heatimagedlg;
	CDlgTongCalib m_tongcalibdlg;
	CDlgRadiaInfo m_radiacalibdlg;
// Operations
public:
	BITMAPFILEHEADER ImgFileHdr;
	BITMAPINFOHEADER ImgInfoHdr;
	void CreateBmpHeader();
	void CreateBmpHeader2();
	void UpdateBMPImage();
	CDC			m_VLMemDC;						//内存DC
	CBitmap		m_VLMemBmp;						//内存bmp
	CBitmap *	m_pVLOldBmp;					//内存bmp选入DC后的备份BMP
	void savebmp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTIView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTIView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewAdvconfig();
	afx_msg void OnUpdateViewAdvconfig(CCmdUI* pCmdUI);
	afx_msg void OnViewRadiaconfig();
	afx_msg void OnUpdateViewRadiaconfig(CCmdUI* pCmdUI);
	afx_msg void OnViewMaskteperature();
	afx_msg void OnUpdateViewMaskteperature(CCmdUI* pCmdUI);
	afx_msg void OnViewCorrectpara();
	afx_msg void OnUpdateViewCorrectpara(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveddt(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPoint m_ptMouseinClient;
	
	int m_iModefy[POINT_NUM];
	CRect m_rect;
	CPoint m_mousepoint;
	void DrawCross(CDC * pDC, int x, int y, int width);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewCalib();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in TIView.cpp
inline CTIDoc* CTIView::GetDocument()
   { return (CTIDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIVIEW_H__3DC8E742_1B0F_483C_813B_A8A580AF7AA8__INCLUDED_)
