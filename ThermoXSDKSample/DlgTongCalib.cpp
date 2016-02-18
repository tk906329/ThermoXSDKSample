// DlgTongCalib.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgTongCalib.h"
#include "afxdialogex.h"
#include "sqlite3.h"

// CDlgTongCalib 对话框

IMPLEMENT_DYNAMIC(CDlgTongCalib, CDialogEx)

CDlgTongCalib::CDlgTongCalib(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTongCalib::IDD, pParent)
{
	m_iTongNo = -1;
}

CDlgTongCalib::~CDlgTongCalib()
{
}

void CDlgTongCalib::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDlgTongCalib, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTongCalib::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgTongCalib::OnBnClickedButton7)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgTongCalib 消息处理程序


BOOL CDlgTongCalib::OnInitDialog()
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
	m_list.InsertColumn(1, _T("机器人A4轴"));
	m_list.InsertColumn(2, _T("相机x坐标"));
	m_list.InsertColumn(3, _T("相机y坐标"));
	m_list.SetColumnWidth(0, 50);//设置列宽
	m_list.SetColumnWidth(1, 80);
	m_list.SetColumnWidth(2, 70);
	m_list.SetColumnWidth(3, 70);
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//添加
void CDlgTongCalib::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	bool bFind = false;
	int iNo = theApp.m_fRobotA4 * 1000;
	for (std::list<struct_TongInfo*>::iterator pTongiterator = theApp.m_listTongInfo.begin(); pTongiterator != theApp.m_listTongInfo.end(); ++pTongiterator)
	{
		struct_TongInfo*pInfo = *pTongiterator;
		if (pInfo->iNo == iNo && pInfo->intTongNo == theApp.m_iTongNo)
		{
			bFind = true;
			break;
		}
	}
	if (bFind)
	{
		AfxMessageBox(L"当前点已存在，不能重复添加");
		return;
	}
	struct_TongInfo*pInfo = new struct_TongInfo;
	pInfo->fA4 = theApp.m_fRobotA4;
	pInfo->iNo = iNo;
	pInfo->intTongNo = theApp.m_iTongNo;
	pInfo->intShowX = theApp.m_ptRadio.x;
	pInfo->intShowY = theApp.m_ptRadio.y;
	theApp.m_listTongInfo.push_back(pInfo);
	freshList();
	sqlite3 *m_db;
	sqlite3_stmt *m_stmt;
	int result = 0;

	char * pfilename = "hnc3.db";
	result = sqlite3_open(pfilename, &m_db);
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //提高性能
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //加大缓存
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //返回改变记录数
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //支持中文LIKE查询

	char cSQL[200];
	sprintf(cSQL, "insert into TongInfo([No],[fA4],[showX],[showY],[TongNo])  values(%d,%f,%d,%d,%d)", pInfo->iNo, pInfo->fA4,
		pInfo->intShowX,pInfo->intShowY, pInfo->intTongNo);
	sqlite3_exec(m_db, cSQL, 0, 0, NULL);

	sqlite3_close(m_db);

}

//覆盖
void CDlgTongCalib::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
	double sabs = 10;
	bool bFind = false;
	std::list<struct_TongInfo*>::iterator pTongiterator1 ;
	for (std::list<struct_TongInfo*>::iterator pTongiterator = theApp.m_listTongInfo.begin(); pTongiterator != theApp.m_listTongInfo.end(); ++pTongiterator)
	{
		struct_TongInfo*pInfo = *pTongiterator;
		if (pInfo->intTongNo == theApp.m_iTongNo)
		{
			if (abs(pInfo->fA4 - theApp.m_fRobotA4)<sabs)
			{
				sabs = abs(pInfo->fA4 - theApp.m_fRobotA4);
				pTongiterator1 = pTongiterator;
				bFind = true;
			}
		}
	}
	if (bFind)
	{
		struct_TongInfo*pInfo = *pTongiterator1; 
		int iNo = pInfo->iNo;
		pInfo->fA4 = theApp.m_fRobotA4;
		pInfo->iNo = theApp.m_fRobotA4 * 1000;
		pInfo->intShowX = theApp.m_ptRadio.x;
		pInfo->intShowY = theApp.m_ptRadio.y;
		freshList();
		sqlite3 *m_db;
		sqlite3_stmt *m_stmt;
		int result = 0;

		char * pfilename = "hnc3.db";
		result = sqlite3_open(pfilename, &m_db);
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
		sprintf(cSQL, "update TongInfo set [No]=%d,[fA4]=%f,[showX]=%d,[showY]=%d where [No]=%d and [TongNo]=%d", 
			pInfo->iNo, pInfo->fA4,	pInfo->intShowX, pInfo->intShowY, iNo,pInfo->intTongNo);
		sqlite3_exec(m_db, cSQL, 0, 0, NULL);

		sqlite3_close(m_db);
	}
}

void CDlgTongCalib::freshList()
{
	CString stemp;
	m_iNum = 0;
	m_list.DeleteAllItems();//清空
	for (std::list<struct_TongInfo*>::iterator pTongiterator = theApp.m_listTongInfo.begin(); pTongiterator != theApp.m_listTongInfo.end(); ++pTongiterator)
	{
		struct_TongInfo*pInfo = *pTongiterator;
		if (pInfo->intTongNo == theApp.m_iTongNo)
		{
			m_list.InsertItem(m_iNum, L"");
			stemp.Format(_T("%d"), pInfo->iNo);
			m_list.SetItemText(m_iNum, 0, stemp);
			stemp.Format(_T("%f"), pInfo->fA4);
			m_list.SetItemText(m_iNum, 1, stemp);
			stemp.Format(_T("%d"), pInfo->intShowX);
			m_list.SetItemText(m_iNum, 2, stemp);
			stemp.Format(_T("%d"), pInfo->intShowY);
			m_list.SetItemText(m_iNum, 3, stemp);
			m_iNum++;
		}
	}
}
void CDlgTongCalib::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString stemp;
	if (m_iTongNo!=theApp.m_iTongNo)
	{
		m_iTongNo = theApp.m_iTongNo;
		stemp.Format(_T("当前料桶编号:%d"), theApp.m_iTongNo+1);
		SetDlgItemTextW(IDC_STATIC, stemp);
		freshList();
	}
	CDialogEx::OnTimer(nIDEvent);
}
