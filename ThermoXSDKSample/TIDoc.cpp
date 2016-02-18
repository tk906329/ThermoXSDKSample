// TIDoc.cpp : implementation of the CTIDoc class
//
#include "StdAfx.h"
#include "ThermoXSDKSample.h"
#include "TIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern struct struct_CamInfo g_CamInfo;
extern void CALLBACK NewFrame(int intCameraTemperature, DWORD dwFFCCounterdown, DWORD dwCamState);

/////////////////////////////////////////////////////////////////////////////
// CTIDoc


IMPLEMENT_DYNCREATE(CTIDoc, CDocument)

BEGIN_MESSAGE_MAP(CTIDoc, CDocument)
	//{{AFX_MSG_MAP(CTIDoc)
	ON_COMMAND(ID_FILE_SAVEDDT, OnFileSaveddt)
	ON_COMMAND(ID_FILE_LOADDDT, OnFileLoadddt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTIDoc construction/destruction

CTIDoc::CTIDoc()
{
	memset(&g_CamInfo, 0, sizeof(struct struct_CamInfo));
	g_CamInfo.intFPAWidth = 384;//预设为非零值，防止在LinkCamera()之前被误用
	g_CamInfo.intFPAHeight = 288;
	g_CamInfo.intMaxFPS = 1;
	g_CamInfo.intCurrentFPS = 1;
	g_CamInfo.intShowWidth = 750;
	g_CamInfo.intShowHeight = 500;
	g_CamInfo.intShowX = 20;
	g_CamInfo.intShowY = 20;
	
	m_bCorrectTemperature = FALSE;
	m_fDefaultEmissivity = 1.0f;

}

CTIDoc::~CTIDoc()
{
}

BOOL CTIDoc::OnNewDocument()
{
	static BOOL bFirst=TRUE;//以后不许new

	if (bFirst)
	{
		bFirst=FALSE;
	}
	else
	{
		return FALSE;
	}

	//CSize m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle(_T("武汉奋进智能机器红外视觉软件"));

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTIDoc serialization

void CTIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTIDoc diagnostics

#ifdef _DEBUG
void CTIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTIDoc commands

void CTIDoc::OnFileSaveddt() 
{
	/*if (MAG_IsProcessingImage())
	{
		if (!MAG_SaveDDT(NULL))
		{
			int e = GetLastError();
			AfxMessageBox(_T("Failed"));
		}
	}*/
}

void CTIDoc::OnFileLoadddt() 
{
	/*if (MAG_IsProcessingImage())
	{
		MAG_StopProcessImage();
	}

	if (MAG_IsLinked())
	{
		MAG_DisLinkCamera();
	}
	

	static TCHAR szDefaultExt[]= _T("ddt");
	static TCHAR szFilter[] = _T("Magnity Device Dependent Temperature Files(*.ddt)|*.ddt|All Files(*.*)|*.*||");

	CFileDialog dlgFile(TRUE, szDefaultExt, NULL, OFN_HIDEREADONLY,szFilter);
	
	if(dlgFile.DoModal() != IDOK)
	{
		return;
	}


	MAG_SetAutoEnlargePara(5, 0, 0);
	
	//MAG_SetDetailEnhancement()
	//MAG_SetColorPalette()
	//MAG_SetFixPara()
	//MAG_SetEXLevel()
	
	OutputPara paraOut;
	paraOut.dwFPAWidth = g_CamInfo.intFPAWidth;
	paraOut.dwFPAHeight = g_CamInfo.intFPAHeight;
	paraOut.dwBMPWidth = g_CamInfo.intFPAWidth;
	paraOut.dwBMPHeight = g_CamInfo.intFPAHeight;
	paraOut.dwColorBarWidth = COLORBARWIDTH;
	paraOut.dwColorBarHeight = g_CamInfo.intFPAHeight;


	//注意，此时回调函数与UI在同一线程，回调函数执行完成之后MAG_LoadDDT()才会返回
	MAG_LoadDDT(&paraOut, dlgFile.GetPathName(), NewFrame);*/
}
