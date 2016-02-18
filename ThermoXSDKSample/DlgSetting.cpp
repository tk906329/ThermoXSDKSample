// DlgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgSetting.h"
#include "afxdialogex.h"


// CDlgSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSetting, CDialogEx)
extern struct struct_CamInfo g_CamInfo;

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetting::IDD, pParent)
	, m_ix(0)
	, m_iWidth(0)
	, m_iy(0)
	, m_iHeight(0)
	, m_Tmax(0)
	, m_Tmin(0)
	, m_TRobot(0)
	, m_iHeightStep1(0)
	, m_iHeightMax(0)
	, m_TEmergency(0)
	, m_iHeightMax2(0)
	, m_iFlag1(0)
{

}

CDlgSetting::~CDlgSetting()
{
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Text(pDX, IDC_EDIT1, m_ix);
	DDX_Text(pDX, IDC_EDIT2, m_iWidth);
	DDX_Text(pDX, IDC_EDIT3, m_iy);
	DDX_Text(pDX, IDC_EDIT4, m_iHeight);
	DDX_Text(pDX, IDC_EDIT9, m_Tmax);
	DDX_Text(pDX, IDC_EDIT10, m_Tmin);
	DDX_Text(pDX, IDC_EDIT13, m_TRobot);
	DDX_Text(pDX, IDC_EDIT14, m_iHeightStep1);
	DDX_Text(pDX, IDC_EDIT18, m_iHeightMax);
	DDX_Text(pDX, IDC_EDIT19, m_TEmergency);
	DDX_Text(pDX, IDC_EDIT20, m_iHeightMax2);
	DDX_Text(pDX, IDC_EDIT21, m_iFlag1);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgSetting::OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgSetting::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgSetting::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK_VISION, &CDlgSetting::OnBnClickedCheckVision)
END_MESSAGE_MAP()


// CDlgSetting 消息处理程序


BOOL CDlgSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString stemp;
	for (size_t i = 0; i < 12; i++)
	{
		stemp = "";
		for (size_t j = 0; j < 32; j++)
		{
			if (theApp.m_spalette[i][j] != 0)
				stemp += theApp.m_spalette[i][j];
		}

		//stemp = theApp.m_spalette[i];
		m_combo.AddString(stemp);
	}

	m_combo.SetCurSel(theApp.m_paletteindex);
	m_ix = g_CamInfo.intShowX;
	m_iy = g_CamInfo.intShowY;
	m_iWidth = g_CamInfo.intShowWidth;
	m_iHeight = g_CamInfo.intShowHeight;
	m_Tmax = theApp.m_maxT;
	m_Tmin = theApp.m_minT;
	m_TRobot = theApp.m_dvalue[4];
	m_iHeightStep1 = theApp.m_dvalue[6];
	m_iHeightMax = theApp.m_fHeightT1;
	m_iHeightMax2 = theApp.m_fHeightT2;
	m_TEmergency = theApp.m_dvalue[8];
	m_iFlag1 = theApp.m_iFlag1;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgSetting::OnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码

	theApp.m_paletteindex=m_combo.GetCurSel();
}


void CDlgSetting::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	g_CamInfo.intShowX = m_ix;
	g_CamInfo.intShowY = m_iy;
	g_CamInfo.intShowWidth = m_iWidth;
	g_CamInfo.intShowHeight = m_iHeight;
	theApp.m_maxT = m_Tmax;
	theApp.m_minT = m_Tmin;
	theApp.m_dvalue[4] = m_TRobot;
	theApp.m_dvalue[6] = m_iHeightStep1;
	theApp.m_fHeightT1 = m_iHeightMax;
	theApp.m_fHeightT2 = m_iHeightMax2;
	theApp.m_dvalue[8] = m_TEmergency;
	theApp.m_iFlag1 = m_iFlag1;

	CString szIniFile = theApp.m_strFilePath + L"\\RobotConfig.ini";
	CString str;
	str.Format(L"%f",m_Tmax);
	WritePrivateProfileString(L"CheckSetting", L"MAX_T", str, szIniFile);
	str.Format(L"%f", m_Tmin);
	WritePrivateProfileString(L"CheckSetting", L"MIN_T", str, szIniFile);
	str.Format(L"%d", m_ix);
	WritePrivateProfileString(L"VisionSetting", L"VISION_X", str, szIniFile);
	str.Format(L"%d", m_iy);
	WritePrivateProfileString(L"VisionSetting", L"VISION_Y", str, szIniFile);
	str.Format(L"%d", m_iWidth);
	WritePrivateProfileString(L"VisionSetting", L"VISION_WIDTH", str, szIniFile);
	str.Format(L"%d", m_iHeight);
	WritePrivateProfileString(L"VisionSetting", L"VISION_HEIGHT", str, szIniFile);
	str.Format(L"%f", m_TRobot);
	WritePrivateProfileString(L"CheckSetting", L"T_ROBOT", str, szIniFile);
	str.Format(L"%f", m_iHeightStep1);
	WritePrivateProfileString(L"CheckSetting", L"HEIGHT_STEP1", str, szIniFile);
	str.Format(L"%d", m_iHeightMax);
	WritePrivateProfileString(L"CheckSetting", L"HEIGHT_FULL", str, szIniFile);
	str.Format(L"%d", m_iHeightMax2);
	WritePrivateProfileString(L"CheckSetting", L"HEIGHT_FULL2", str, szIniFile);
	str.Format(L"%f", m_TEmergency);
	WritePrivateProfileString(L"CheckSetting", L"T_EMERGENCY", str, szIniFile);
	str.Format(L"%d", m_iFlag1);
	WritePrivateProfileString(L"CheckSetting", L"INT_FLAG1", str, szIniFile);
}


void CDlgSetting::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (MessageBox(_T("是否要清空当前的热点信息？"), _T("提示"), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
	{
		EnterCriticalSection(&theApp.m_cs);
		std::list<struct_Heatinfo*>::iterator	pHeatiterator;
		for (pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
		{
			struct_Heatinfo* pHeat = *pHeatiterator;
			delete pHeat;
		}
		theApp.m_listpHeat.clear();
		LeaveCriticalSection(&theApp.m_cs);
	}
}


void CDlgSetting::OnBnClickedCheckVision()
{
	// TODO:  在此添加控件通知处理程序代码
	theApp.m_bVisionUpdateFlag = !theApp.m_bVisionUpdateFlag;
}
