// OrigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "OrigDlg.h"
#include "afxdialogex.h"


// COrigDlg �Ի���

IMPLEMENT_DYNAMIC(COrigDlg, CDialogEx)

COrigDlg::COrigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrigDlg::IDD, pParent)
	, m_ix(0)
	, m_iy(0)
{

}

COrigDlg::~COrigDlg()
{
}

void COrigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ix);
	DDX_Text(pDX, IDC_EDIT3, m_iy);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(COrigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &COrigDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// COrigDlg ��Ϣ�������
UINT Thread_Play(LPVOID lpParam){
	COrigDlg *pdlg = (COrigDlg *)lpParam;
	DWORD dw = GetTickCount();
	int icount = 0;
	CString stemp;
	float ft[6] = {0};
	int xsum=0,ysum=0;
	for (;;)
	{
		if (theApp.m_pClientSocket->SHYx(0)!=0)
		{
			theApp.m_pClientSocket->SetSHYx(0, 0);
			Sleep(1000);
			stemp.Format(_T("%.1f"), theApp.maxT);
			pdlg->m_list.SetItemText(icount, 2, stemp);
			stemp.Format(_T("%d"), theApp.max_x);
			pdlg->m_list.SetItemText(icount, 3, stemp);
			stemp.Format(_T("%d"), theApp.max_y);
			pdlg->m_list.SetItemText(icount, 4, stemp);
			ft[icount] = theApp.maxT;
			xsum += theApp.max_x;
			ysum += theApp.max_y;
			icount++;
			dw = GetTickCount();
			pdlg->m_progress.SetPos(icount);
			theApp.m_pClientSocket->SetSHYx(1,1);
		}
		if ((icount==6) || (GetTickCount()-dw>10000))
		{
			break;
		}
		Sleep(100);
	}
	if (icount != 6)
	{
		AfxMessageBox(_T("�궨��ʱ"));
	}
	else
	{
		pdlg->m_ix = xsum / 6;
		pdlg->m_iy = ysum / 6;
		pdlg->UpdateData(FALSE);
		AfxMessageBox(_T("�궨���"));
	}
	pdlg->m_progress.SetPos(0);
	pdlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	return 0;
}

BOOL COrigDlg::OnInitDialog()
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
	m_list.InsertColumn(1, _T("����Ƕ�"));
	m_list.InsertColumn(2, _T("�¶�"));
	m_list.InsertColumn(3, _T("���x����"));
	m_list.InsertColumn(4, _T("���y����"));
	m_list.SetColumnWidth(0, 50);//�����п�
	m_list.SetColumnWidth(1, 60);
	m_list.SetColumnWidth(2, 85);
	m_list.SetColumnWidth(3, 85);
	m_list.SetColumnWidth(4, 85);
	m_list.InsertItem(0, _T("1"));
	m_list.InsertItem(1, _T("2"));
	m_list.InsertItem(2, _T("3"));
	m_list.InsertItem(3, _T("4"));
	m_list.InsertItem(4, _T("5"));
	m_list.InsertItem(5, _T("6"));
	m_list.SetItemText(0, 1, _T("60"));
	m_list.SetItemText(1, 1, _T("120"));
	m_list.SetItemText(2, 1, _T("180"));
	m_list.SetItemText(3, 1, _T("240"));
	m_list.SetItemText(4, 1, _T("300"));
	m_list.SetItemText(5, 1, _T("360"));

	m_progress.SetRange(0, 6);

	m_ix = theApp.m_robotx;
	m_iy = theApp.m_roboty;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void COrigDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	for (size_t i = 0; i < 6; i++)
	{
		m_list.SetItemText(i, 2, _T(""));
		m_list.SetItemText(i, 3, _T(""));
		m_list.SetItemText(i, 4, _T(""));
	}
	AfxBeginThread(Thread_Play, this);//�����߳�
}
