// DlgHeatImage.cpp : 实现文件
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "DlgHeatImage.h"
#include "afxdialogex.h"

#define PI 3.1415926
// CDlgHeatImage 对话框

IMPLEMENT_DYNAMIC(CDlgHeatImage, CDialogEx)

CDlgHeatImage::CDlgHeatImage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHeatImage::IDD, pParent)
{

}

CDlgHeatImage::~CDlgHeatImage()
{
}

void CDlgHeatImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHeatImage, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDlgHeatImage 消息处理程序


BOOL CDlgHeatImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ecl_x = 260;
	m_ecl_y = 260;
	m_radia = 240;
	m_ass = m_radia / 1000.0;
	VERIFY(m_rgn.CreateEllipticRgn(m_ecl_x - m_radia, m_ecl_y - m_radia, m_ecl_x + m_radia, m_ecl_y + m_radia));
	SetTimer(1, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgHeatImage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!IsWindowVisible())
	{
		return;
	}
	Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgHeatImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	int x, y;
	dc.Ellipse(m_ecl_x - m_radia, m_ecl_y - m_radia, m_ecl_x + m_radia, m_ecl_y + m_radia);
	dc.MoveTo(m_ecl_x - m_radia, m_ecl_y);
	dc.LineTo(m_ecl_x + m_radia, m_ecl_y);
	dc.MoveTo(m_ecl_x, m_ecl_y - m_radia);
	dc.LineTo(m_ecl_x , m_ecl_y+ m_radia);
	/*CPen pendot(PS_DOT, 1, RGB(0, 0, 0));
	dc.SelectObject(&pendot);
	dc.Ellipse(m_ecl_x - 24, m_ecl_y - 24, m_ecl_x + 24, m_ecl_y + 24);
	dc.Ellipse(m_ecl_x - 48, m_ecl_y - 48, m_ecl_x + 48, m_ecl_y + 48);
	dc.Ellipse(m_ecl_x - 72, m_ecl_y - 72, m_ecl_x + 72, m_ecl_y + 72);
	dc.Ellipse(m_ecl_x - 96, m_ecl_y - 96, m_ecl_x + 96, m_ecl_y + 96);
	dc.Ellipse(m_ecl_x - 120, m_ecl_y - 120, m_ecl_x + 120, m_ecl_y + 120);
	dc.Ellipse(m_ecl_x - 144, m_ecl_y - 144, m_ecl_x + 144, m_ecl_y + 144);
	dc.Ellipse(m_ecl_x - 168, m_ecl_y - 168, m_ecl_x + 168, m_ecl_y + 168);
	dc.Ellipse(m_ecl_x - 192, m_ecl_y - 192, m_ecl_x + 192, m_ecl_y + 192);
	dc.Ellipse(m_ecl_x - 216, m_ecl_y - 216, m_ecl_x + 216, m_ecl_y + 216);*/
	//AfxMessageBox(L"11");
	CBrush  brA, brB, brC, brD, brE;
	brA.CreateSolidBrush(RGB(255, 0, 0));
	brB.CreateSolidBrush(RGB(255, 255, 0));
	brC.CreateSolidBrush(RGB(0, 0, 0));
	brD.CreateSolidBrush(RGB(0, 128, 0));
	brE.CreateSolidBrush(RGB(163, 73, 164));
	if (m_rgn.PtInRegion(m_mousepoint))
	{
		CString str;
		float ix = (float)(m_mousepoint.x - m_ecl_x);
		float iy = (float)(m_mousepoint.y - m_ecl_y);
		float hud = atan(ix / iy);
		if (ix<0 && iy>0)
		{
			hud = PI + hud;
		}
		else if (ix>=0 && iy>0)
		{
			hud = hud - PI;
		}
		float radia = sqrt(ix*ix + iy*iy)/m_ass;
		str.Format(_T("<%.1f><%.3f>"), radia, hud);
		dc.DrawText(str, CRect(m_mousepoint.x, m_mousepoint.y - 20, m_mousepoint.x + 110, m_mousepoint.y), DT_LEFT | DT_BOTTOM);

	}
	float TEmergency = theApp.m_dvalue[8] + theApp.averT;
	if (TEmergency > 45)
		TEmergency = 45;

	EnterCriticalSection(&theApp.m_cs);
	CString stemp;
	for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
	{
		struct_Heatinfo* pHeat = *pHeatiterator;
		int x = m_ecl_x - pHeat->fRadia*m_ass*sin(pHeat->fHud);
		int y = m_ecl_y - pHeat->fRadia*m_ass*cos(pHeat->fHud);
		if (pHeat->m_dwDeleteTime != 0)
			dc.SelectObject(&brD);
		else if (pHeat->ftemprature >= TEmergency)
			dc.SelectObject(&brE);
		else
			dc.SelectObject(&brB);
		dc.Ellipse(x - 3, y - 3, x + 3, y + 3);
	}
	LeaveCriticalSection(&theApp.m_cs);
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	dc.SelectObject(&pen);
	dc.MoveTo(m_ecl_x, m_ecl_y);
	x = m_ecl_x - m_radia*sin(theApp.m_pClientSocket->SHYc(31));
	y = m_ecl_y - m_radia*cos(theApp.m_pClientSocket->SHYc(31));
	dc.LineTo(x, y);

	//下一点坐标
	dc.SelectObject(&brA);
	x = m_ecl_x - theApp.m_dvalue[0] * m_ass*sin(theApp.m_dvalue[1]);
	y = m_ecl_y - theApp.m_dvalue[0] * m_ass*cos(theApp.m_dvalue[1]);
	dc.Ellipse(x - 3, y - 3, x + 3, y + 3);

	//目标坐标
	dc.SelectObject(&brC);
	x = m_ecl_x - theApp.m_pClientSocket->SHYc(33) *m_ass*sin(theApp.m_pClientSocket->SHYc(34));
	y = m_ecl_y - theApp.m_pClientSocket->SHYc(33) * m_ass*cos(theApp.m_pClientSocket->SHYc(34));
	dc.Ellipse(x - 3, y - 3, x + 3, y + 3);

}


void CDlgHeatImage::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_mousepoint = point;

	CDialogEx::OnMouseMove(nFlags, point);
}
