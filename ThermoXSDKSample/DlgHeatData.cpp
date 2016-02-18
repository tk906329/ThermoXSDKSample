// DlgHeatData.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgHeatData.h"
#include "afxdialogex.h"


// CDlgHeatData 对话框

IMPLEMENT_DYNAMIC(CDlgHeatData, CDialogEx)

CDlgHeatData::CDlgHeatData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHeatData::IDD, pParent)
{

}

CDlgHeatData::~CDlgHeatData()
{
}

void CDlgHeatData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CDlgHeatData, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgHeatData 消息处理程序


BOOL CDlgHeatData::OnInitDialog()
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
	m_list.InsertColumn(1, _T("相机x坐标"));
	m_list.InsertColumn(2, _T("相机y坐标"));
	m_list.InsertColumn(3, _T("温度"));
	m_list.InsertColumn(4, _T("机器人半径"));
	m_list.InsertColumn(5, _T("机器人弧度"));
	m_list.SetColumnWidth(0, 50);//设置列宽
	m_list.SetColumnWidth(1, 70);
	m_list.SetColumnWidth(2, 70);
	m_list.SetColumnWidth(3, 50);
	m_list.SetColumnWidth(4, 75);
	m_list.SetColumnWidth(5, 75);
	refreshList();
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgHeatData::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	refreshList();
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgHeatData::refreshList()
{
	if (!IsWindowVisible())
	{
		return;
	}
	CString stemp;
	m_list.DeleteAllItems();
	int num = 0;

	EnterCriticalSection(&theApp.m_cs);
	for (std::list<struct_Heatinfo*>::iterator	pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
	{
		struct_Heatinfo* pHeat = *pHeatiterator;
		if (pHeat->m_dwDeleteTime != 0)
		{
			continue;
		}
		stemp.Format(L"%d", num+1);
		m_list.InsertItem(num, stemp);
		stemp.Format(L"%d", pHeat->intShowX);
		m_list.SetItemText(num, 1, stemp);
		stemp.Format(L"%d", pHeat->intShowY);
		m_list.SetItemText(num, 2, stemp);
		stemp.Format(L"%.1f", pHeat->ftemprature);
		m_list.SetItemText(num, 3, stemp);
		stemp.Format(L"%f", pHeat->fRadia);
		m_list.SetItemText(num, 4, stemp);
		stemp.Format(L"%f", pHeat->fHud);
		m_list.SetItemText(num, 5, stemp);
		num++;
	}
	LeaveCriticalSection(&theApp.m_cs);
}

