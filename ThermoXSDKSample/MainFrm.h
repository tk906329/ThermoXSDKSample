// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4DE9297D_6F6E_4693_A669_EF3F328116B2__INCLUDED_)
#define AFX_MAINFRM_H__4DE9297D_6F6E_4693_A669_EF3F328116B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThermoXSDKSample.h"

#include "DlgLog.h"
#include "DlgData.h"
#include "DlgSetting.h"

double getAngle(double x, double y);

class CMainFrame : public CFrameWnd
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CDlgLog				m_dlgLog;
	CDlgData			m_dlgData;
	CDlgSetting         m_dlgSetting;
// Operations
	CClientSocket2 *m_pComSocket;
	CClientSocket3 *m_pClientSocket;
	BOOL SocketConnect(CAsyncSocket *pSocket,CString sIP, int iPort);
	BOOL SocketDisconnect(CAsyncSocket *pSocket);
	void GetIni();
	CString m_sRobotIP;
	int m_iRobotPort;
	int Robot_Location_x[4], Robot_Location_y[4];
	double m_robot_ratio;


public:
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewControlbar();
	afx_msg void OnUpdateViewControlbar(CCmdUI* pCmdUI);
	afx_msg void OnViewLogbar();
	afx_msg void OnUpdateViewLogbar(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bClosed;
public:
	afx_msg void OnViewData();
	afx_msg void OnUpdateViewData(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void ActivateFrame(int nCmdShow = -1);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4DE9297D_6F6E_4693_A669_EF3F328116B2__INCLUDED_)
