// DlgTongCalib.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgTongCalib.h"
#include "afxdialogex.h"
#include "sqlite3.h"

// CDlgTongCalib �Ի���

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


// CDlgTongCalib ��Ϣ�������


BOOL CDlgTongCalib::OnInitDialog()
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
	m_list.InsertColumn(1, _T("������A4��"));
	m_list.InsertColumn(2, _T("���x����"));
	m_list.InsertColumn(3, _T("���y����"));
	m_list.SetColumnWidth(0, 50);//�����п�
	m_list.SetColumnWidth(1, 80);
	m_list.SetColumnWidth(2, 70);
	m_list.SetColumnWidth(3, 70);
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//���
void CDlgTongCalib::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(L"��ǰ���Ѵ��ڣ������ظ����");
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
	result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //�������
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //�Ӵ󻺴�
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //���ظı��¼��
	if (result != 0)
		return ;
	result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //֧������LIKE��ѯ

	char cSQL[200];
	sprintf(cSQL, "insert into TongInfo([No],[fA4],[showX],[showY],[TongNo])  values(%d,%f,%d,%d,%d)", pInfo->iNo, pInfo->fA4,
		pInfo->intShowX,pInfo->intShowY, pInfo->intTongNo);
	sqlite3_exec(m_db, cSQL, 0, 0, NULL);

	sqlite3_close(m_db);

}

//����
void CDlgTongCalib::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //�������
		if (result != 0)
			return;
		result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //�Ӵ󻺴�
		if (result != 0)
			return;
		result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //���ظı��¼��
		if (result != 0)
			return;
		result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //֧������LIKE��ѯ

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
	m_list.DeleteAllItems();//���
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString stemp;
	if (m_iTongNo!=theApp.m_iTongNo)
	{
		m_iTongNo = theApp.m_iTongNo;
		stemp.Format(_T("��ǰ��Ͱ���:%d"), theApp.m_iTongNo+1);
		SetDlgItemTextW(IDC_STATIC, stemp);
		freshList();
	}
	CDialogEx::OnTimer(nIDEvent);
}
