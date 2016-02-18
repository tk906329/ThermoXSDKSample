// DlgRadiaInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgRadiaInfo.h"
#include "afxdialogex.h"
#include "sqlite3.h"

// CDlgRadiaInfo 对话框

IMPLEMENT_DYNAMIC(CDlgRadiaInfo, CDialogEx)

CDlgRadiaInfo::CDlgRadiaInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRadiaInfo::IDD, pParent)
{
	m_iTongNo = -1;
}

CDlgRadiaInfo::~CDlgRadiaInfo()
{
}

void CDlgRadiaInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDlgRadiaInfo, CDialogEx)
	ON_WM_TIMER()
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgRadiaInfo::OnItemchangedList1)
//ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDlgRadiaInfo::OnItemchangedList1)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgRadiaInfo::OnDblclkList1)
ON_BN_CLICKED(IDC_BUTTON1, &CDlgRadiaInfo::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON7, &CDlgRadiaInfo::OnBnClickedButton7)
END_MESSAGE_MAP()


// CDlgRadiaInfo 消息处理程序


BOOL CDlgRadiaInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口style      
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位     
	lStyle |= LVS_REPORT; //设置style    
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//设置style    
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）  
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）  
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件   
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list.DeleteAllItems();//清空
	m_list.InsertColumn(0, _T("序号"));//添加列
	m_list.InsertColumn(1, _T("高度"));
	m_list.InsertColumn(2, _T("半径"));
	m_list.SetColumnWidth(0, 50);//设置列宽
	m_list.SetColumnWidth(1, 80);
	m_list.SetColumnWidth(2, 70);
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgRadiaInfo::freshList()
{
	CString stemp;
	m_iNum = 0;
	m_list.DeleteAllItems();//清空
	for (std::list<struct_RadiaInfo*>::iterator pTongiterator = theApp.m_listRadiaInfo.begin(); pTongiterator != theApp.m_listRadiaInfo.end(); ++pTongiterator)
	{
		struct_RadiaInfo*pInfo = *pTongiterator;
		if (pInfo->intTongNo == theApp.m_iTongNo)
		{
			m_list.InsertItem(m_iNum, L"");
			stemp.Format(_T("%d"), pInfo->iNo);
			m_list.SetItemText(m_iNum, 0, stemp);
			stemp.Format(_T("%d"), pInfo->iHeight);
			m_list.SetItemText(m_iNum, 1, stemp);
			stemp.Format(_T("%d"), pInfo->iRadia);
			m_list.SetItemText(m_iNum, 2, stemp);
			m_iNum++;
		}
	}
}
void CDlgRadiaInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString stemp;
	if (m_iTongNo != theApp.m_iTongNo)
	{
		m_iTongNo = theApp.m_iTongNo;
		stemp.Format(_T("当前料桶编号:%d"), theApp.m_iTongNo + 1);
		SetDlgItemTextW(IDC_STATIC, stemp);
		freshList();
	}
	CDialogEx::OnTimer(nIDEvent);
}
#include "InputDlg.h"
void CDlgRadiaInfo::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int index = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (index != -1)
	{
		CString strAddres = m_list.GetItemText(index, 0);//序号

		CInputDlg dlg;
		dlg.m_strNo = m_list.GetItemText(index, 0);
		dlg.m_strHeight = m_list.GetItemText(index, 1);
		dlg.m_strRadia = m_list.GetItemText(index, 2);
		if (dlg.DoModal() == IDOK)
		{
			m_list.SetItemText(index, 1, dlg.m_strHeight);
			m_list.SetItemText(index, 2, dlg.m_strRadia);
			updateRadia(_wtoi(dlg.m_strNo), _wtoi(dlg.m_strHeight), _wtoi(dlg.m_strRadia));

			sqlite3 *m_db;
			sqlite3_stmt *m_stmt;

			char * pfilename = "hnc3.db";
			int result = sqlite3_open(pfilename, &m_db);
			if (result != 0)
				return;
			result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //提高性能
			if (result != 0)
				return;
			result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //加大缓存
			if (result != 0)
				return;
			result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //返回改变记录数
			if (result != 0)
				return;
			result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //支持中文LIKE查询

			char cSQL[200];
			sprintf(cSQL, "update RadiaInfo set [Height]=%d,[Radia]=%d where [No]=%d",
				_wtoi(dlg.m_strHeight), _wtoi(dlg.m_strRadia), _wtoi(dlg.m_strNo));
			sqlite3_exec(m_db, cSQL, 0, 0, NULL);

			sqlite3_close(m_db);
		}
	}
	*pResult = 0;
}
void CDlgRadiaInfo::updateRadia(int iNo, int iHeight, int iRadia)
{
	for (std::list<struct_RadiaInfo*>::iterator pTongiterator = theApp.m_listRadiaInfo.begin(); pTongiterator != theApp.m_listRadiaInfo.end(); ++pTongiterator)
	{
		struct_RadiaInfo*pInfo = *pTongiterator;
		if (pInfo->iNo == iNo)
		{
			pInfo->iHeight = iHeight;
			pInfo->iRadia = iRadia;
		}
	}
}

void CDlgRadiaInfo::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

}


void CDlgRadiaInfo::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
}
