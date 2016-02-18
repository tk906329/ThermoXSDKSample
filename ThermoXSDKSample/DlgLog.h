#if !defined(AFX_DLGLOG_H__0544B3F1_59B6_46F5_B08A_695D1BAF5E60__INCLUDED_)
#define AFX_DLGLOG_H__0544B3F1_59B6_46F5_B08A_695D1BAF5E60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLog.h : header file
//

#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgLog dialog

class CDlgLog : public CDialog
{
// Construction
public:
	void AppendLog(CString strApp, BOOL bUpdate=TRUE, BOOL bCopy=FALSE);
	CDlgLog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLog)
	enum { IDD = IDD_DIALOG_LOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnUpdateLog();

	// Generated message map functions
	//{{AFX_MSG(CDlgLog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCriticalSection m_CS;
	CEdit	m_editLog;
	CString	m_strLog;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOG_H__0544B3F1_59B6_46F5_B08A_695D1BAF5E60__INCLUDED_)
