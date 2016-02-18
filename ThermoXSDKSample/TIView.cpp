// TIView.cpp : implementation of the CTIView class
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "TIDoc.h"
#include "TIView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern struct struct_CamInfo g_CamInfo;
float addAngle(float x, float y);
/////////////////////////////////////////////////////////////////////////////
// CTIView

IMPLEMENT_DYNCREATE(CTIView, CScrollView)

BEGIN_MESSAGE_MAP(CTIView, CScrollView)
	//{{AFX_MSG_MAP(CTIView)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_ADVCONFIG, OnViewAdvconfig)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ADVCONFIG, OnUpdateViewAdvconfig)
	ON_COMMAND(ID_VIEW_RADIACONFIG, OnViewRadiaconfig)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RADIACONFIG, OnUpdateViewRadiaconfig)
	
	ON_COMMAND(ID_VIEW_MASKTEPERATURE, OnViewMaskteperature)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MASKTEPERATURE, OnUpdateViewMaskteperature)
	ON_COMMAND(ID_VIEW_CORRECTPARA, OnViewCorrectpara)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENU_RADIAINFO, OnUpdateViewRadiaconfig)
	ON_COMMAND(ID_VIEW_MENU_RADIAINFO, OnViewRadiaconfig)
	
	ON_UPDATE_COMMAND_UI(ID_VIEW_CORRECTPARA, OnUpdateViewCorrectpara)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEDDT, OnUpdateFileSaveddt)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, savebmp)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, savebmp)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VIEW_CALIB, &CTIView::OnViewCalib)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTIView construction/destruction

CTIView::CTIView()
{
	// TODO: add construction code here
}

CTIView::~CTIView()
{
}

BOOL CTIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//cs.cx = GetSystemMetrics(SM_CXSCREEN);
	//cs.cy = GetSystemMetrics(SM_CYSCREEN);
	//cs.style &= ~WS_THICKFRAME;  //禁止调整窗口大小
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTIView drawing

void CTIView::OnDraw(CDC* pDC)
{
	
	/*CTIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC MemDC;
	CBitmap MemBitmap;

	MemDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, 750, 500);
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

	MemDC.FillSolidRect(0, 0, 750, 500, RGB(255,255,255));*/
	CRect rc;
	GetClientRect(&rc);
	//CPoint pt = GetScrollPosition();
	//rc.OffsetRect(pt);

	m_VLMemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255));
	//m_VLMemDC.FillSolidRect(0, 0, 3500, 3000, RGB(255, 255, 255));
	//UpdateBMPImage
	CString str;
	CRect rect;
	int lenEachRow = 0;

	if (theApp.m_iVisionType==0)
	{
		lenEachRow = g_CamInfo.intFPAWidth * 4;
		for (int i = 0; i < g_CamInfo.intFPAHeight; i++)
		{
			CopyMemory(theApp.m_pIRBmpData + 54 + (g_CamInfo.intFPAHeight - 1 - i) * lenEachRow,
				&theApp.m_bmp[i * g_CamInfo.intFPAWidth], lenEachRow);
		}

		BITMAPINFO* pHead = (BITMAPINFO*)(theApp.m_pIRBmpData + 14);
		UCHAR * pBmpData = theApp.m_pIRBmpData + 54;

		StretchDIBits(m_VLMemDC.GetSafeHdc(), g_CamInfo.intShowX, g_CamInfo.intShowY, g_CamInfo.intShowWidth, 
			g_CamInfo.intShowHeight,0, 0, g_CamInfo.intFPAWidth, g_CamInfo.intFPAHeight,
			pBmpData, pHead,DIB_RGB_COLORS, SRCCOPY);
	}
	else
	{
		lenEachRow = g_CamInfo.intFPAWidth * 3;
		for (int j = 0; j < g_CamInfo.intFPAHeight; j++)
		{
			CopyMemory(theApp.m_pIRBmpData2 + 54 + j * lenEachRow,
				theApp.m_bmp_guide + lenEachRow*(g_CamInfo.intFPAHeight - 1 - j), lenEachRow);
		}
		BITMAPINFO* pHead = (BITMAPINFO*)(theApp.m_pIRBmpData2 + 14);
		UCHAR * pBmpData = theApp.m_pIRBmpData2 + 54;

		StretchDIBits(m_VLMemDC.GetSafeHdc(), g_CamInfo.intShowX, g_CamInfo.intShowY, g_CamInfo.intShowWidth,
			g_CamInfo.intShowHeight,0, 0, g_CamInfo.intFPAWidth, g_CamInfo.intFPAHeight,
			pBmpData, pHead,DIB_RGB_COLORS, SRCCOPY);
	}
	CBrush brA, brB, brC;
	VERIFY(brA.CreateSolidBrush(RGB(255, 0, 0)));  // rgnA Red

	VERIFY(theApp.m_rgn.DeleteObject());
	VERIFY(theApp.m_rgn.CreateEllipticRgn(theApp.m_ptRadio.x - theApp.m_fRadia, theApp.m_ptRadio.y - theApp.m_fRadia,
		theApp.m_ptRadio.x + theApp.m_fRadia, theApp.m_ptRadio.y + theApp.m_fRadia));
	VERIFY(m_VLMemDC.FrameRgn(&theApp.m_rgn, &brA, 2, 2));
	VERIFY(brB.CreateSolidBrush(RGB(255, 255, 0)));
	VERIFY(brC.CreateSolidBrush(RGB(50, 50, 255)));
	m_VLMemDC.SelectObject(&brC);

	EnterCriticalSection(&theApp.m_cs2);
	for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listcurrentHeat.begin(); pHeatiterator != theApp.m_listcurrentHeat.end(); ++pHeatiterator)
	{
		struct_Heatinfo* pHeat = *pHeatiterator;
		int x = pHeat->intShowX* g_CamInfo.intShowWidth / g_CamInfo.intFPAWidth + g_CamInfo.intShowX;
		int y = pHeat->intShowY* g_CamInfo.intShowHeight / g_CamInfo.intFPAHeight + g_CamInfo.intShowY;
		m_VLMemDC.Ellipse(x - 3, y - 3, x + 3, y + 3);
	}
	LeaveCriticalSection(&theApp.m_cs2);
	//for (size_t i = 0; i < POINT_NUM; i++)
	//{
	//	m_VLMemDC.Ellipse(ptVertex[i].x - 4, ptVertex[i].y - 4, ptVertex[i].x + 4, ptVertex[i].y + 4);
	//}
	m_VLMemDC.SelectObject(&brB);
	m_VLMemDC.Ellipse(theApp.m_ptRadio.x - 4, theApp.m_ptRadio.y - 4, theApp.m_ptRadio.x + 4, theApp.m_ptRadio.y + 4);
	
	m_VLMemDC.MoveTo(m_rect.left, m_rect.top);
	m_VLMemDC.LineTo(m_rect.left, m_rect.bottom);
	m_VLMemDC.LineTo(m_rect.right, m_rect.bottom);
	m_VLMemDC.LineTo(m_rect.right, m_rect.top);
	m_VLMemDC.LineTo(m_rect.left, m_rect.top);

	//MAG_GetEllipseTemperatureInfo(m_rect.left - 5, (int)g_CamInfo.intFPAHeight - m_rect.bottom + 4,
	//	m_rect.right - 5, (int)g_CamInfo.intFPAHeight-m_rect.top + 4, theApp.value);

	DrawCross(&m_VLMemDC, theApp.max_x* g_CamInfo.intShowWidth / g_CamInfo.intFPAWidth + g_CamInfo.intShowX,
		theApp.max_y* g_CamInfo.intShowHeight / g_CamInfo.intFPAHeight + g_CamInfo.intShowY, 3);//*750/384    *500/288
	
	if (theApp.m_ptMouseinClient.x>=g_CamInfo.intShowX && theApp.m_ptMouseinClient.x<(int)(g_CamInfo.intShowWidth + g_CamInfo.intShowX) &&
		theApp.m_ptMouseinClient.y>=g_CamInfo.intShowY && theApp.m_ptMouseinClient.y<(int)(g_CamInfo.intShowHeight + g_CamInfo.intShowY))
	{
		int x = theApp.m_ptMouseinClient.x - g_CamInfo.intShowX;
		int y = theApp.m_ptMouseinClient.y - g_CamInfo.intShowY;
		
		x = (x*g_CamInfo.intFPAWidth) / g_CamInfo.intShowWidth;
		y = (y*g_CamInfo.intFPAHeight) / g_CamInfo.intShowHeight;

		theApp.m_pointx = x;
		theApp.m_pointy = y;

		theApp.m_mouseT = theApp.m_dTemp[y * g_CamInfo.intFPAWidth + x];
		str.Format(_T("%.1f"), theApp.m_mouseT);
		m_VLMemDC.DrawText(str, CRect(m_ptMouseinClient.x, m_ptMouseinClient.y - 15, m_ptMouseinClient.x + 40, m_ptMouseinClient.y), DT_LEFT | DT_BOTTOM);

	}
//	m_VLMemDC.TextOutW(700, 1300, L"1231");
	pDC->BitBlt(0,0, rc.Width(), rc.Height(), &m_VLMemDC, 0, 0, SRCCOPY);
	//pDC->BitBlt(0, 0, 3500, 3000, &m_VLMemDC, 0, 0, SRCCOPY);

	/*m_VLMemDC.SelectObject(m_pVLOldBmp);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();*/
}

void CTIView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	//CSize totalDocSize = GetTotalSize();
	CDC *pDC = GetDC();
	m_VLMemDC.CreateCompatibleDC(pDC);
	m_VLMemBmp.CreateCompatibleBitmap(pDC, g_CamInfo.intShowWidth+1500, g_CamInfo.intShowHeight+1000);
	m_pVLOldBmp = m_VLMemDC.SelectObject(&m_VLMemBmp);
	
	m_heatdatadlg.Create(IDD_DIALOG_HEATDATA, this);
	//m_heatdatadlg.ShowWindow(SW_SHOW);
	m_heatdatadlg.MoveWindow(CRect(850, 100, 1290, 705));
	m_heatimagedlg.Create(IDD_DIALOG_REALIMAGE, this);
	//m_heatimagedlg.ShowWindow(SW_SHOW);
	m_heatimagedlg.MoveWindow(CRect(850, 100, 1390, 640));
	m_tongcalibdlg.Create(IDD_DIALOG_TONG_CALIB, this);
	m_tongcalibdlg.MoveWindow(CRect(850, 100, 1176, 520));
	m_radiacalibdlg.Create(IDD_DIALOG_RADIAINFO, this);
	m_radiacalibdlg.MoveWindow(CRect(850, 100, 1176, 520));
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	//sizeTotal.cx = g_CamInfo.intShowWidth +20;
	//sizeTotal.cy = g_CamInfo.intShowHeight + 20;
	//sizeTotal=GetDocument()->GetDocSize();
	sizeTotal.cx = 1000;
	sizeTotal.cy = 800;
	SetScrollSizes(MM_TEXT, sizeTotal);

	if (theApp.m_iVisionType==0)
		CreateBmpHeader();
	else
		CreateBmpHeader2();

	//Load Parameter
	CString szIniFile = theApp.m_strFilePath + L"\\RobotConfig.ini";
	int index = 0;
	CString stemp;
	for (int i = 0; i < POINT_NUM; i++)
	{
		stemp.Format(L"Shield_X%d", i + 1);
		index = GetPrivateProfileInt(L"CheckSetting", stemp, 0, szIniFile);
		theApp.m_ptVertex[i].x = index;
		stemp.Format(L"Shield_Y%d", i + 1);
		index = GetPrivateProfileInt(L"CheckSetting", stemp, 0, szIniFile);
		theApp.m_ptVertex[i].y = index;
	}
	theApp.m_ptRadio = theApp.m_ptVertex[theApp.m_iTongNo];
	
	VERIFY(theApp.m_rgn.CreateEllipticRgn(theApp.m_ptRadio.x - theApp.m_fRadia, theApp.m_ptRadio.y - theApp.m_fRadia,
		theApp.m_ptRadio.x + theApp.m_fRadia, theApp.m_ptRadio.y + theApp.m_fRadia));
	SetTimer(1, 150, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CTIView printing

BOOL CTIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(1);
	
	return DoPreparePrinting(pInfo);
}

void CTIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTIView diagnostics

#ifdef _DEBUG
void CTIView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTIView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTIDoc* CTIView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTIDoc)));
	return (CTIDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTIView message handlers

void CTIView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptScroll;
	
	//加上滚动条位置
	ptScroll=GetScrollPosition();
	m_ptMouseinClient.x = ptScroll.x + point.x;
	m_ptMouseinClient.y = ptScroll.y + point.y;
	theApp.m_ptMouseinClient = m_ptMouseinClient;
	
	if (m_iModefy[0] == 1)
	{
		theApp.m_ptRadio = point;
				
		VERIFY(theApp.m_rgn.DeleteObject());
		VERIFY(theApp.m_rgn.CreateEllipticRgn(theApp.m_ptRadio.x - theApp.m_fRadia, theApp.m_ptRadio.y - theApp.m_fRadia,
			theApp.m_ptRadio.x + theApp.m_fRadia, theApp.m_ptRadio.y + theApp.m_fRadia));
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CTIView::OnViewAdvconfig() 
{
	/*if (MAG_IsLinked())
	{
		CDlgRemoteConfig dlg;

		dlg.DoModal();
	}*/
	m_heatimagedlg.ShowWindow(m_heatimagedlg.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}
void CTIView::OnUpdateViewAdvconfig(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck(MAG_IsUsingTemperatureMask());	
	pCmdUI->SetCheck(m_heatimagedlg.IsWindowVisible() ? 1 : 0);
}
void CTIView::OnViewRadiaconfig()
{
	m_radiacalibdlg.ShowWindow(m_radiacalibdlg.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}
void CTIView::OnUpdateViewRadiaconfig(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_radiacalibdlg.IsWindowVisible() ? 1 : 0);
}
void CTIView::OnViewMaskteperature() 
{
	m_heatdatadlg.ShowWindow(m_heatdatadlg.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}

void CTIView::OnUpdateViewMaskteperature(CCmdUI* pCmdUI) 
{
	//pCmdUI->SetCheck(MAG_IsUsingTemperatureMask());	
	pCmdUI->SetCheck(m_heatdatadlg.IsWindowVisible() ? 1 : 0);
}

void CTIView::OnViewCorrectpara() 
{
	m_tongcalibdlg.ShowWindow(m_tongcalibdlg.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}
void CTIView::OnUpdateViewCorrectpara(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck(MAG_IsUsingTemperatureMask());	
	pCmdUI->SetCheck(m_tongcalibdlg.IsWindowVisible() ? 1 : 0);
}

void CTIView::OnUpdateFileSaveddt(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(MAG_IsProcessingImage());
}

void CTIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_mousepoint = point;

	if (((theApp.m_ptRadio.x - point.x)*(theApp.m_ptRadio.x - point.x) + (theApp.m_ptRadio.y - point.y)*(theApp.m_ptRadio.y - point.y)) < 36)
	{
		m_iModefy[0] = 1;
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void CTIView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (m_iModefy[0] == 1)
	{
		m_iModefy[0] = 0;

		CString szIniFile = theApp.m_strFilePath + L"\\RobotConfig.ini";
		CString str, sPath;
		str.Format(L"%d", theApp.m_ptRadio.x);
		sPath.Format(L"Shield_X%d", theApp.m_iTongNo + 1);
		WritePrivateProfileString(L"CheckSetting", sPath, str, szIniFile);
		str.Format(L"%d", theApp.m_ptRadio.y);
		sPath.Format(L"Shield_Y%d", theApp.m_iTongNo + 1);
		WritePrivateProfileString(L"CheckSetting", sPath, str, szIniFile);
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CTIView::DrawCross(CDC * pDC, int x,int y, int width)					//画光标
{
	int length = 3;
	pDC->MoveTo(x, y - length - width);
	pDC->LineTo(x, y - length);
	pDC->MoveTo(x, y + length + width);
	pDC->LineTo(x, y + length);

	pDC->MoveTo(x - length - width, y);
	pDC->LineTo(x - length, y);
	pDC->MoveTo(x + length + width, y);
	pDC->LineTo(x + length, y);
}

#include "DlgCalib.h"
void CTIView::OnViewCalib()
{
	// TODO:  在此添加命令处理程序代码
	CDlgCalib dlg;

	dlg.DoModal();
}

void CTIView::CreateBmpHeader()
{
	ZeroMemory(&ImgFileHdr, sizeof(BITMAPFILEHEADER));				//此步非常重要，否则读出的图片无法显示
	ZeroMemory(&ImgInfoHdr, sizeof(BITMAPINFOHEADER));
	///FileHeader
	ImgFileHdr.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);// + 256*sizeof(RGBQUAD);
	ImgFileHdr.bfSize = ImgFileHdr.bfOffBits + 384 * 288;
	ImgFileHdr.bfType = 0x4d42;
	//Information Header
	ImgInfoHdr.biClrUsed = 0;					//
	ImgInfoHdr.biSize = sizeof(BITMAPINFOHEADER);;
	ImgInfoHdr.biHeight = 288;
	ImgInfoHdr.biWidth = 384;
	ImgInfoHdr.biPlanes = 1;
	ImgInfoHdr.biSizeImage = 384 * 288 * 4;
	ImgInfoHdr.biBitCount = 8*4;
	memcpy(theApp.m_pIRBmpData, &ImgFileHdr, sizeof(BITMAPFILEHEADER));
	memcpy(theApp.m_pIRBmpData + sizeof(BITMAPFILEHEADER), &ImgInfoHdr, sizeof(BITMAPINFOHEADER));
}

void CTIView::CreateBmpHeader2()
{
	ZeroMemory(&ImgFileHdr, sizeof(BITMAPFILEHEADER));				//此步非常重要，否则读出的图片无法显示
	ZeroMemory(&ImgInfoHdr, sizeof(BITMAPINFOHEADER));
	///FileHeader
	ImgFileHdr.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);// + 256*sizeof(RGBQUAD);
	ImgFileHdr.bfSize = ImgFileHdr.bfOffBits + 384 * 288;
	ImgFileHdr.bfType = 0x4d42;
	//Information Header
	ImgInfoHdr.biClrUsed = 0;					//
	ImgInfoHdr.biSize = 40;
	ImgInfoHdr.biHeight = 288;
	ImgInfoHdr.biWidth = 384;
	ImgInfoHdr.biPlanes = 1;
	ImgInfoHdr.biSizeImage = 384 * 288 * 3;
	ImgInfoHdr.biBitCount = 8 * 3;
	memcpy(theApp.m_pIRBmpData2, &ImgFileHdr, sizeof(BITMAPFILEHEADER));
	memcpy(theApp.m_pIRBmpData2 + sizeof(BITMAPFILEHEADER), &ImgInfoHdr, sizeof(BITMAPINFOHEADER));
}
void CTIView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//CRect rect(g_CamInfo.intShowX - 50, g_CamInfo.intShowY - 50, g_CamInfo.intShowWidth + g_CamInfo.intShowX + 50, g_CamInfo.intShowHeight + g_CamInfo.intShowY + 50);
	//InvalidateRect(&rect);
	if (theApp.m_bVisionUpdateFlag)
	{
		Invalidate();
	}
	CScrollView::OnTimer(nIDEvent);
}

BOOL CTIView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

void CTIView::savebmp()
{
	CFile file;
	CTime time = CTime::GetCurrentTime();
	CString sfile;
	sfile.Format(_T("%4d%2d%2d%2d%2d%2d.bmp"),time.GetYear(),time.GetMonth(),time.GetDay(),
		time.GetHour(),time.GetMinute(),time.GetSecond());
	file.Open(theApp.m_strFilePath+ "\\bmp\\"+sfile, CFile::modeCreate | CFile::modeWrite);
	if (theApp.m_iVisionType==0)
		file.Write(theApp.m_pIRBmpData, 384 * 288 * 4 + 54);
	else
		file.Write(theApp.m_pIRBmpData2, 384 * 288 * 3 + 54);
	file.Close();
}