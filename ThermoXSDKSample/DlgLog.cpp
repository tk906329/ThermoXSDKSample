// DlgLog.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"

#include "DlgLog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLog dialog

CDlgLog::CDlgLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLog)
	m_strLog = _T("");
	//}}AFX_DATA_INIT

	AppendLog(_T("Log Start"), FALSE, FALSE);
}


void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLog)
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Text(pDX, IDC_EDIT_LOG, m_strLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialog)
	//{{AFX_MSG_MAP(CDlgLog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(WM_UPDATE_LOG, OnUpdateLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLog message handlers

void CDlgLog::AppendLog(CString strApp, BOOL bUpdate, BOOL bCopy)
{
	static int day = -1;
	CString strTime;

	m_CS.Lock();//本函数可能被多个线程调用，防止重入

	CTime t = CTime::GetCurrentTime();
	
	if (day==t.GetDay())
	{
		strTime = t.Format(_T("%H:%M:%S: "));
	}
	else
	{
		strTime = t.Format(_T("%Y-%m-%d %H:%M:%S: "));
		day = t.GetDay();
	}

	m_strLog +=  strTime + strApp + _T("\r\n");


	m_CS.Unlock();

	if (bUpdate)
	{
		//采用消息机制通知UI更新，以免耽误本函数的用户
		PostMessage(WM_COMMAND, WM_UPDATE_LOG);
	}
}

void CDlgLog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_editLog.GetSafeHwnd())
	{
		m_editLog.MoveWindow(&rcClient);
	}
}

void CDlgLog::OnDestroy() 
{
	CDialog::OnDestroy();

	AppendLog(_T("Log end.\r\n"), FALSE);
}

void CDlgLog::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgLog::OnCancel() 
{
	ShowWindow(SW_HIDE);
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CDlgLog::OnUpdateLog()
{
	if (IsWindow(m_editLog.GetSafeHwnd()) && m_editLog.IsWindowVisible())
	{
		#ifdef _DEBUG
			m_editLog.AssertValid();
		#endif

		//UpdateData(FALSE);
		m_editLog.SetWindowText(m_strLog);

		m_editLog.LineScroll(m_editLog.GetLineCount());
	}
}
