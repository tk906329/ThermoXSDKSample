// DlgCalib.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgCalib.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern struct struct_CamInfo g_CamInfo;
// CDlgCalib 对话框

IMPLEMENT_DYNAMIC(CDlgCalib, CDialogEx)

CDlgCalib::CDlgCalib(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalib::IDD, pParent)
	, m_svalue1(_T(""))
	, m_svalue2(_T(""))
	, m_svalue3(_T(""))
	, m_svalue4(_T(""))
	, m_svalue5(_T(""))
	, m_svalue6(_T(""))
	, m_svalue7(_T(""))
	, m_svalue8(_T(""))
	, m_svalue9(_T(""))
	, m_svalue10(_T(""))
	, m_fa3_1(0)
	, m_fa2_1(0)
	, m_fa1_1(0)
	, m_fa0_1(0)
	, m_fa3_2(0)
	, m_fa2_2(0)
	, m_fa1_2(0)
	, m_fa0_2(0)
	, m_pix_min(0)
	, m_distance_min(0)
	, m_fMinus(0)
	, m_iRobotLen(0)
	, m_fa1_distance(0)
	, m_fa0_distance(0)
{

}

CDlgCalib::~CDlgCalib()
{
}

void CDlgCalib::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_svalue1);
	DDX_Text(pDX, IDC_EDIT2, m_svalue2);
	DDX_Text(pDX, IDC_EDIT3, m_svalue3);
	DDX_Text(pDX, IDC_EDIT4, m_svalue4);
	DDX_Text(pDX, IDC_EDIT5, m_svalue5);
	DDX_Text(pDX, IDC_EDIT6, m_svalue6);
	DDX_Text(pDX, IDC_EDIT7, m_svalue7);
	DDX_Text(pDX, IDC_EDIT8, m_svalue8);
	DDX_Text(pDX, IDC_EDIT11, m_svalue9);
	DDX_Text(pDX, IDC_EDIT15, m_svalue10);
	DDX_Text(pDX, IDC_EDIT12, m_fa3_1);
	DDX_Text(pDX, IDC_EDIT16, m_fa2_1);
	DDX_Text(pDX, IDC_EDIT22, m_fa1_1);
	DDX_Text(pDX, IDC_EDIT23, m_fa0_1);
	DDX_Text(pDX, IDC_EDIT17, m_fa3_2);
	DDX_Text(pDX, IDC_EDIT24, m_fa2_2);
	DDX_Text(pDX, IDC_EDIT25, m_fa1_2);
	DDX_Text(pDX, IDC_EDIT26, m_fa0_2);
	DDX_Text(pDX, IDC_EDIT20, m_pix_min);
	DDX_Text(pDX, IDC_EDIT21, m_distance_min);
	DDX_Text(pDX, IDC_EDIT27, m_fMinus);
	DDX_Text(pDX, IDC_EDIT28, m_iRobotLen);
	DDX_Text(pDX, IDC_EDIT29, m_fa1_distance);
	DDX_Text(pDX, IDC_EDIT30, m_fa0_distance);
}


BEGIN_MESSAGE_MAP(CDlgCalib, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgCalib::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgCalib::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgCalib::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &CDlgCalib::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgCalib::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgCalib::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDlgCalib 消息处理程序


BOOL CDlgCalib::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString stemp;
	stemp.Format(L"%d", theApp.m_robotx);
	m_svalue1 = stemp;
	stemp.Format(L"%d", theApp.m_roboty);
	m_svalue2 = stemp;
	stemp.Format(L"%f", theApp.m_ratio);
	m_svalue6 = stemp;
	stemp.Format(L"%f", theApp.m_angle);
	m_svalue7 = stemp;

	m_iRobotLen = theApp.m_robotLen;

	m_fa3_1 = theApp.m_fa3_1;
	m_fa2_1 = theApp.m_fa2_1;
	m_fa1_1 = theApp.m_fa1_1;
	m_fa0_1 = theApp.m_fa0_1;
	m_fa3_2 = theApp.m_fa3_2;
	m_fa2_2 = theApp.m_fa2_2;
	m_fa1_2 = theApp.m_fa1_2;
	m_fa0_2 = theApp.m_fa0_2;

	m_pix_min = theApp.m_pix_min;
	m_distance_min = theApp.m_distance_min;
	m_fMinus = theApp.m_fMinus;
	m_fa0_distance = theApp.m_fa0_distance;
	m_fa1_distance = theApp.m_fa1_distance;

	stemp.Format(L"%f", theApp.m_dvalue[5]);
	m_svalue10 = stemp;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

#include "OrigDlg.h"
void CDlgCalib::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	COrigDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString stemp;
		stemp.Format(L"%d", dlg.m_ix);
		m_svalue1 = stemp;
		stemp.Format(L"%d", dlg.m_iy);
		m_svalue2 = stemp;
		stemp.Format(L"%f", theApp.m_dvalue[3]);
		m_svalue10 = stemp;
		UpdateData(FALSE);
	}
}


void CDlgCalib::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stemp;
	stemp.Format(L"%d", theApp.max_x);
	m_svalue3 = stemp;
	stemp.Format(L"%d", theApp.max_y);
	m_svalue4 = stemp;
	UpdateData(FALSE);
}


void CDlgCalib::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	if (m_svalue3 == "" || m_svalue4 == "" || m_svalue8 == "" || m_svalue9 == "")
	{
		AfxMessageBox(L"请采集测量点坐标");
		return;
	}
	if (m_svalue5 == "")
	{
		AfxMessageBox(L"请输入测量点之间的距离");
		return;
	}
	double data1, data2;
	data1 = _wtof(m_svalue5);
	if (data1<100 || data1>2000)
	{
		AfxMessageBox(L"距离值异常，请重新输入，范围（100-2000）");
		return;
	}
	int x1 = _wtoi(m_svalue8);
	int y1 = _wtoi(m_svalue9);
	int x2 = _wtoi(m_svalue3);
	int y2 = _wtoi(m_svalue4);
	if (x1<=0 || x2<=0 || y1<=0 || y2<=0 || x1>384 || x2>384 || y1>288 ||y2>288)
	{
		AfxMessageBox(L"坐标值异常，请重新获取，x范围（0-384）y范围（0-288）");
		return;
	}
	double deletax = x1 - x2;
	double deletay = y1 - y2;
	data2 = data1 / sqrt(deletax*deletax + deletay*deletay);
	m_svalue6.Format(L"%lf", data2);
	UpdateData(FALSE);
}


void CDlgCalib::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	if (IDOK != MessageBox(_T("您确定要保存当前结果吗？"), _T("警告"), MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2))
	{
		return;
	}
	UpdateData();
	int i = _wtoi(m_svalue1);
	if (i<=0 || i>384)
	{
		AfxMessageBox(L"参考点x坐标异常，不在正常范围(0-384)");
		return;
	}
	theApp.m_robotx = i;
	i = _wtoi(m_svalue2);
	if (i <= 0 || i>288)
	{
		AfxMessageBox(L"参考点y坐标异常，不在正常范围(0-288)");
		return;
	}
	theApp.m_roboty = i;
	theApp.m_ptVision.x = theApp.m_robotx*g_CamInfo.intShowWidth / g_CamInfo.intFPAWidth + g_CamInfo.intShowX;
	theApp.m_ptVision.y = theApp.m_roboty*g_CamInfo.intShowHeight / g_CamInfo.intFPAHeight + g_CamInfo.intShowY;

	double f = _wtof(m_svalue6);
	if (f <= 0 || f>10)
	{
		AfxMessageBox(L"转换系数异常，请重新计算");
		return;
	}
	theApp.m_ratio = f;
	f = _wtof(m_svalue7);
	if (f < -3.14 || f>3.14)
	{
		AfxMessageBox(L"夹角异常，请重新获取");
		return;
	}
	theApp.m_angle = f;	
	theApp.m_robotLen = m_iRobotLen;

	f = _wtof(m_svalue10);
	if (f < -1000 || f>2000)
	{
		AfxMessageBox(L"基准测距值异常，请重新设置");
		return;
	}
	theApp.m_dvalue[5] = f;
	//Load Parameter
	CString szIniFile = theApp.m_strFilePath + "\\RobotConfig.ini";
	WritePrivateProfileString(L"RobotSetting", L"RobotRatio", m_svalue6, szIniFile);
	WritePrivateProfileString(L"RobotSetting", L"RobotAngle", m_svalue7, szIniFile);
	WritePrivateProfileString(L"RobotSetting", L"RobotX", m_svalue1, szIniFile);
	WritePrivateProfileString(L"RobotSetting", L"RobotY", m_svalue2, szIniFile);
	WritePrivateProfileString(L"RobotSetting", L"RobotHeight", m_svalue10, szIniFile);
	CString stemp;
	stemp.Format(L"%d", m_iRobotLen);
	WritePrivateProfileString(L"RobotSetting", L"RobotLen", stemp, szIniFile);

	theApp.m_pix_min = m_pix_min;
	theApp.m_distance_min = m_distance_min;
	theApp.m_fMinus = m_fMinus;
	stemp.Format(L"%d", m_pix_min);
	WritePrivateProfileString(L"RobotSetting", L"PIX_MIN", stemp, szIniFile);
	stemp.Format(L"%d", m_distance_min);
	WritePrivateProfileString(L"RobotSetting", L"DISTANCE_MIN", stemp, szIniFile);
	stemp.Format(L"%.2f", m_fMinus);
	WritePrivateProfileString(L"RobotSetting", L"MINUS", stemp, szIniFile);

	theApp.m_fa0_distance = m_fa0_distance;
	theApp.m_fa1_distance = m_fa1_distance;
	stemp.Format(L"%f", m_fa0_distance);
	WritePrivateProfileString(L"RobotSetting", L"Distance_A0", stemp, szIniFile);
	stemp.Format(L"%f", m_fa1_distance);
	WritePrivateProfileString(L"RobotSetting", L"Distance_A1", stemp, szIniFile);

	theApp.m_fa3_1 = m_fa3_1;
	theApp.m_fa2_1 = m_fa2_1;
	theApp.m_fa1_1 = m_fa1_1;
	theApp.m_fa0_1 = m_fa0_1;
	theApp.m_fa3_2 = m_fa3_2;
	theApp.m_fa2_2 = m_fa2_2;
	theApp.m_fa1_2 = m_fa1_2;
	theApp.m_fa0_2 = m_fa0_2;
	stemp.Format(L"%.9f", m_fa3_1);
	WritePrivateProfileString(L"CheckSetting", L"Cube1A3", stemp, szIniFile);
	stemp.Format(L"%f", m_fa2_1);
	WritePrivateProfileString(L"CheckSetting", L"Cube1A2", stemp, szIniFile);
	stemp.Format(L"%f", m_fa1_1);
	WritePrivateProfileString(L"CheckSetting", L"Cube1A1", stemp, szIniFile);
	stemp.Format(L"%.2f", m_fa0_1);
	WritePrivateProfileString(L"CheckSetting", L"Cube1A0", stemp, szIniFile);
	stemp.Format(L"%.9f", m_fa3_2);
	WritePrivateProfileString(L"CheckSetting", L"Cube2A3", stemp, szIniFile);
	stemp.Format(L"%f", m_fa2_2);
	WritePrivateProfileString(L"CheckSetting", L"Cube2A2", stemp, szIniFile);
	stemp.Format(L"%f", m_fa1_2);
	WritePrivateProfileString(L"CheckSetting", L"Cube2A1", stemp, szIniFile);
	stemp.Format(L"%.2f", m_fa0_2);
	WritePrivateProfileString(L"CheckSetting", L"Cube2A0", stemp, szIniFile);

	CDialogEx::OnOK();
}


void CDlgCalib::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	//CString stemp;
	//stemp.Format(L"%f", -1*theApp.m_pClientSocket->SHYc(5));
	//m_svalue7 = stemp;
	//UpdateData(FALSE);
	UpdateData();
	theApp.m_fa3_1 = m_fa3_1;
	theApp.m_fa2_1 = m_fa2_1;
	theApp.m_fa1_1 = m_fa1_1;
	theApp.m_fa0_1 = m_fa0_1;
	theApp.m_fa3_2 = m_fa3_2;
	theApp.m_fa2_2 = m_fa2_2;
	theApp.m_fa1_2 = m_fa1_2;
	theApp.m_fa0_2 = m_fa0_2;
}


void CDlgCalib::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码
	CString stemp;
	stemp.Format(L"%d", theApp.max_x);
	m_svalue8 = stemp;
	stemp.Format(L"%d", theApp.max_y);
	m_svalue9 = stemp;
	UpdateData(FALSE);
}

