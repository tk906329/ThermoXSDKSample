// ThermoXSDKSample.cpp : ���x���ó�ʽ��e�О顣
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "MainFrm.h"

#include "TIDoc.h"
#include "TIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTIApp

BEGIN_MESSAGE_MAP(CTIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTIApp::OnAppAbout)
	// �����ļ�����Ę˜ʙn��
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// �˜���ӡ�O������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTIApp ����

CTIApp::CTIApp()
{
	// TODO: �ڴ˼��뽨����ʽ�a��
	// ��������Ҫ�ĳ�ʼ�O������ InitInstance ��
	m_iHeatNum = 0;
	m_iHeatCount = 0;
	m_bA4Alarm = false;
	m_robotLen = 1500;
	m_paletteindex = 9;
	m_iCount = 0;
	m_fRadia = 300;
	m_iTongNo = 0;
	m_fRobotA4 = 0;
	m_dwSend = 0;
	memset(m_fHeightValue, 0, HEIGHT_NUM * 4);
	memset(m_fHeightHud, 0, 48);
	memset(m_valueA4, 0, 40);
	m_iVisionType = 0;
	m_bVisionUpdateFlag = false;

	InitializeCriticalSection(&m_cs);
	InitializeCriticalSection(&m_cs2);
}

// �H�е�һ�� CTIApp ���

CTIApp theApp;
struct struct_CamInfo g_CamInfo;

// CTIApp ��ʼ�O��

BOOL CTIApp::InitInstance()
{
	// ���瑪�ó�ʽ�YӍ���ָ��ʹ�� ComCtl32.dll 6 (��) ����汾��
	// �톢��ҕ�X����ʽ���� Windows XP �ϣ��t��Ҫ InitCommonControls()��
	// ��t�κ�ҕ���Ľ�������ʧ����
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �O��Ҫ������������Ҫ��춑��ó�ʽ�е�
	// ͨ�ÿ����e��
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��ʽ��
	if (!AfxOleInit())
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// �׽��ֳ�ʼ��
	if (!AfxSocketInit())
	{
		MessageBox(NULL, L"Windows ͨ�Ŷ˿ڳ�ʼ��ʧ�ܡ�", L"��ʾ", MB_ICONWARNING);
		return FALSE;
	}
	// �˜ʳ�ʼ�O��
	// �������ʹ���@Щ���܁K����p��
	// ������ɵĿɈ��Йn��С��������
	// �����г�ʽ�a�Ƴ�����Ҫ�ĳ�ʼ����ʽ��
	// ׃�������O��ֵ�ĵ�䛙C�a
	// TODO: ����ԓ�m���޸Ĵ��ִ�
	// (���磬��˾���Q��M�����Q)
	SetRegistryKey(_T("���C AppWizard ���a���đ��ó�ʽ"));
	LoadStdProfileSettings(4);  // �d��˜� INI �n�x� (���� MRU)
	// ��䛑��ó�ʽ���ļ��������ļ�����ؓ؟���ļ���
	// ���ҕ���c�zҕ�g�M���B��
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTIDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ���ҕ��
		RUNTIME_CLASS(CTIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �����˜� Shell ���DDE���n���_���f�n��������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ����������ָ���ķ�����������ʹ�� 
	// /RegServer��/Register��/Unregserver �� /Unregister ���ӑ��ó�ʽ�������� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// �H��ʼ��һ��ҕ���������@ʾ�K����ԓҕ��
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// ֻ���� SDI ���ó�ʽ����������Ԫ�r���ź��� DragAcceptFiles
	// �@���l��� ProcessShellCommand ֮��
	return TRUE;
}



// �� App About ʹ�� CAboutDlg ��Ԓ���K

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��Ԓ���K�Y��
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧Ԯ

// ��ʽ�a����
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnViewData();
//	afx_msg void OnUpdateViewData(CCmdUI *pCmdUI);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_COMMAND(ID_VIEW_DATA, &CAboutDlg::OnViewData)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_DATA, &CAboutDlg::OnUpdateViewData)
END_MESSAGE_MAP()

// ���Ќ�Ԓ���K�đ��ó�ʽ����
void CTIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CTIApp ӍϢ̎��ʽ

