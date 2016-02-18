// DlgHeatData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgHeatData.h"
#include "afxdialogex.h"


// CDlgHeatData �Ի���

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


// CDlgHeatData ��Ϣ�������


BOOL CDlgHeatData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style      
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ     
	lStyle |= LVS_REPORT; //����style    
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//����style    
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��  
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��  
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�   
	m_list.SetExtendedStyle(dwStyle); //������չ���

	m_list.DeleteAllItems();//���
	m_list.InsertColumn(0, _T("���"));//�����
	m_list.InsertColumn(1, _T("���x����"));
	m_list.InsertColumn(2, _T("���y����"));
	m_list.InsertColumn(3, _T("�¶�"));
	m_list.InsertColumn(4, _T("�����˰뾶"));
	m_list.InsertColumn(5, _T("�����˻���"));
	m_list.SetColumnWidth(0, 50);//�����п�
	m_list.SetColumnWidth(1, 70);
	m_list.SetColumnWidth(2, 70);
	m_list.SetColumnWidth(3, 50);
	m_list.SetColumnWidth(4, 75);
	m_list.SetColumnWidth(5, 75);
	refreshList();
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgHeatData::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

