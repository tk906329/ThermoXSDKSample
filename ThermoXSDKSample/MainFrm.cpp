// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ThermoXSDKSample.h"
#include "MainFrm.h"
#include "MyConnect.h"
#include "MyConnect2.h"
#include <Dbghelp.h>
#include "sqlite3.h"

#pragma comment( lib, "sqlite3.lib" ) //指定与静态库一起连接

#define PI 3.1415926
int getHeat(double fA4, double fA5);
void addHeatList();
int refreshHeat(double startA4, double startA5, double endA4, double endA5);
double getLineInfo(int iType,double startA4, double startA5, double endA4, double endA5);
double binsolve(double *dvalue, int num);
void  analyseHeat(float dHeatinfo[384][288]);
int getTongInfo();
void getTong();
int getRadiaInfo();
float getRadia(float height);
int insertOpt(int nType);

bool b_flag1 = true;
bool b_flag2 = true;
bool b_flag3 = true;
bool b_flag4 = true;
bool b_flag5 = false;
bool b_flag6 = false;
int iCount = 0;
bool b_isDelay = false;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _CRT_SECURE_NO_WARNINGS

CMainFrame*   pFrame;
extern struct struct_CamInfo g_CamInfo;
#pragma comment( lib, "DbgHelp" )

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	SYSTEMTIME systime;		//保存时间的结构体	
	GetLocalTime(&systime);	//本地时间
	CString sfilename;
	sfilename.Format(L"DumpFile_%04d%02d%02d%02d%02d%02d.dmp", systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond);
	HANDLE lhDumpFile = CreateFile(sfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;

	loExceptionInfo.ExceptionPointers = ExceptionInfo;

	loExceptionInfo.ThreadId = GetCurrentThreadId();

	loExceptionInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);

	CloseHandle(lhDumpFile);

	return EXCEPTION_EXECUTE_HANDLER;

}
//巨哥相机接收回调
void __stdcall FrameRecved(uint cameraIp){
	static uint loop = 0;

	//锁定数据帧
	const DataFrame* frame = LockDataFrame(cameraIp);
	if (frame == NULL)return ;

	//操作数据帧
	DataFrameHeader* header = frame->pHeader;

	if (header->Slope==0)
	{
		AppLog(L"Slope参数异常");
		return;
	}
	for (size_t i = 0; i < 384*288; i++)
	{
		theApp.m_dTemp[i] = frame->pTemp[i] / header->Slope + header->Offset;
	}
	strTemp2Bgra(theApp.m_cfg, frame->pTemp, header, theApp.m_bmp, theApp.m_palette[theApp.m_paletteindex]);
	
	//解锁数据帧
	UnlockDataFrame(cameraIp);
	//::PostMessage(pFrame->GetActiveView()->m_hWnd, WM_MYMESSAGE, 0, (LPARAM)&cameraIp);	

	if ((GetTickCount() - theApp.m_iCount) > 200)
	{
		b_isDelay = true;
	}

	theApp.m_iCount = GetTickCount();
}

//高德相机接收回调
int __stdcall FrameRecved2(const tagRGBANDTEMPERATUREDATA_CB *pInfo)
{
	if ((pInfo->RGBImgDataLen != 3 * 384 * 288) || (pInfo->matrixDataLen != 384 * 288))
		return 0;
	if (pInfo->iShutterStatus==1)
	{
		return 0;
		AfxMessageBox(L"快门");
	}
	memcpy(theApp.m_dTemp, pInfo->MeasureMatrix, 384 * 288 * sizeof(double));
	
	/*for (size_t i = 0; i < 384*288; i++)
	{
		theApp.m_bmp[i].r = pInfo->pRGBImgData[i * 3];
		theApp.m_bmp[i].g = pInfo->pRGBImgData[i * 3+1];
		theApp.m_bmp[i].b = pInfo->pRGBImgData[i * 3+2];
	}*/
	memcpy(theApp.m_bmp_guide, pInfo->pRGBImgData, 384 * 288 * 3);

	if ((GetTickCount() - theApp.m_iCount) > 200)
	{
		b_isDelay = true;
	}

	theApp.m_iCount = GetTickCount();
	return 0;
}
DWORD WINAPI analyseA4(LPVOID lpParam)
{
	double dmin = 0;
	int iFlag1 = 0, iFlag2 = 0;
	for (;;)
	{
		memcpy(theApp.m_valueA4, theApp.m_valueA4 + 1, 32);
		theApp.m_valueA4[4] = theApp.m_pClientSocket->SHYc(31);
		dmin = 0;
		for (size_t i = 0; i < 5; i++)
		{
			dmin += abs(theApp.m_valueA4[i] - theApp.m_valueA4[4]);
		}
		theApp.m_speedA4 = dmin;
		if (theApp.m_pClientSocket->SHYc(31)>0)
		{
			iFlag1++;
			iFlag2 = 0;
		}
		else
		{
			iFlag2++;
			iFlag1 = 0;
		}
		if (iFlag1>100 || iFlag2>100)		
			theApp.m_bA4Alarm = true;	//A4轴长时间在一个方向
		else
			theApp.m_bA4Alarm = false;
		if (theApp.m_pClientSocket->SHYx(33) != 2 && b_flag2)				//清空热点表
		{
			b_flag2 = false;
			theApp.m_iHeatNum = 0;
			std::list<struct_Heatinfo*>::iterator	pHeatiterator;
			EnterCriticalSection(&theApp.m_cs);
			for (pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
			{
				struct_Heatinfo* pHeat = *pHeatiterator;
				delete pHeat;
			}
			theApp.m_listpHeat.clear();
			LeaveCriticalSection(&theApp.m_cs);
			EnterCriticalSection(&theApp.m_cs2);
			for (pHeatiterator = theApp.m_listcurrentHeat.begin(); pHeatiterator != theApp.m_listcurrentHeat.end(); ++pHeatiterator)
			{
				struct_Heatinfo* pHeat = *pHeatiterator;
				delete pHeat;
			}
			theApp.m_listcurrentHeat.clear();
			LeaveCriticalSection(&theApp.m_cs2);
		}
		if (theApp.m_pClientSocket->SHYx(33) == 2)
		{
			b_flag2 = true;
		}
		if (theApp.m_pClientSocket->SHYx(34) == 0 && b_flag4)
		{
			theApp.m_iHeatNum = 0;
			b_flag4 = false;
		}
		if (theApp.m_pClientSocket->SHYx(34) == 1)
		{
			b_flag4 = true;
		}
		if (theApp.m_pClientSocket->SHYx(33) != 0)
		{
			if (b_flag5)
			{
				b_flag5 = false;
				insertOpt(1);
			}
			b_flag6 = true;
		}
		else
		{
			if (b_flag6)
			{
				b_flag6 = false;
				insertOpt(0);
			}
			b_flag5 = true;
		}
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI CameraOptThread(LPVOID lpParam)
{
	CDlgData *pdlg = (CDlgData *)lpParam;
	for (;;)
	{
		if (GetTickCount() - theApp.m_iCount > 4000)
		{
			theApp.m_iCount = GetTickCount();
			pdlg->SetDlgItemTextW(IDC_STATIC_CAMERA_NETWORK, L"热像仪连接:失败");
			if (theApp.m_iVisionType == 0)
			{
				//发现在线热像仪
				DiscoverCameraResp resps[3];
				int error = DiscoverOnlineCameras(resps, ARRAY_SIZE(resps));

				if (error >= 1)
				{
					uint cameraIp = resps[0].CameraIp;
					StopTransData(cameraIp);
					error = StartTransData(cameraIp, TransType_Tcp, DataType_Temp, FrameRecved);
				}
				else
					AppLog(L"没有找到热像仪");
			}
			else
			{
				StartGetRGBAndTemperatureData(theApp.m_cVisionIP, FrameRecved2, 0);
			}
		}
		else
			pdlg->SetDlgItemTextW(IDC_STATIC_CAMERA_NETWORK, L"热像仪连接:成功");
		if (GetTickCount() - theApp.m_dwComData > 2000)
		{
			pdlg->SetDlgItemTextW(IDC_STATIC_INFO7, L"激光测距仪连接:失败");
		}
		else
			pdlg->SetDlgItemTextW(IDC_STATIC_INFO7, L"激光测距仪连接:成功");
		Sleep(1000);
	}
}
DWORD WINAPI ReadRader(LPVOID lpParam)
{
	HANDLE hCom = CreateFile(theApp.m_sRsPort, GENERIC_READ | GENERIC_WRITE, // 允许读写
		0, // 此项必须为0,dwShareMode
		NULL,	// no security attrs ,lpSecurityAttributes
		OPEN_EXISTING,	//设置产生方式dwCreationDisposition
		0,	// 同步通信dwFlagsAndAttributes
		NULL);	//hTemplateFile
	if (hCom == (HANDLE)-1)
	{
		OutInforStr = L"打开 " + theApp.m_sRsPort + L" 失败,请确认您的计算机上有该端口并且该端口未被使用!";
		MessageBox(0, OutInforStr, L"提示", MB_TOPMOST | 0x30);
		return 0;
	}
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = theApp.m_iRsRate; //波特率
	dcb.ByteSize = 8;		//每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //停止位
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;

	SetCommState(hCom, &dcb);
	COMMTIMEOUTS TimeOuts;
	GetCommTimeouts(hCom, &TimeOuts);
	//设定读超时////////在读一次输入缓冲区的内容后读操作就立即返回，而不管是否读入了要求的字符。
	TimeOuts.ReadIntervalTimeout = 30;		//接收时，两字符间最大的时延
	TimeOuts.ReadTotalTimeoutMultiplier = 30;  //读取每字节的超时
	TimeOuts.ReadTotalTimeoutConstant = 90;   //读串口数据的固定超时
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 50;
	TimeOuts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	BYTE	recvbuf[50];
	DWORD	bytesRecv = 0;
	BYTE sendbuf[2] = { 1 ,0};
	DWORD	dwBytesWritten=0;
	for (;;)
	{
		WriteFile(hCom, sendbuf, 1, &dwBytesWritten, NULL);
		Sleep(100);
		memset(recvbuf, 0, sizeof(recvbuf));
		if (ReadFile(hCom, recvbuf, 50, &bytesRecv, NULL))
		{
			if (bytesRecv <= 0)
			{
				Sleep(10);
				continue;
			}
			if (bytesRecv != 21 || recvbuf[1] != '.' || recvbuf[0]>'9' || recvbuf[0]<'0' || recvbuf[2]>'9' || recvbuf[2]<'0'
				|| recvbuf[3]>'9' || recvbuf[3]<'0' || recvbuf[4]>'9' || recvbuf[4]<'0')
				continue;
			BYTE sum = 0;
			for (size_t i = 0; i < 20; i++)
				sum += recvbuf[i];
			if (sum != recvbuf[20])
				continue;
			int ddd = (recvbuf[0] - '0') * 1000 + (recvbuf[2] - '0') * 100 + (recvbuf[3] - '0') * 10 + (recvbuf[4] - '0');
			theApp.m_distance = ddd;
			if (ddd<1000 || ddd>2000)
			{
				continue;
			}
			float ftemp = theApp.m_fa1_distance*(float)ddd + theApp.m_fa0_distance;      //1.64m
			memcpy(theApp.m_fHeightValue, theApp.m_fHeightValue + 1, (HEIGHT_NUM - 1) * 4);
			theApp.m_fHeightValue[HEIGHT_NUM - 1] = ftemp;
			if (theApp.m_pClientSocket->SHYx(33) == 2)
			{
				if (theApp.m_fHeightValue[HEIGHT_NUM - 1] > (theApp.m_fHeightValue[HEIGHT_NUM - 2] + 20))
				{
					theApp.m_fHeightValue[HEIGHT_NUM - 1] = (theApp.m_fHeightValue[HEIGHT_NUM - 2] + 20);
				}
				if (theApp.m_fHeightValue[HEIGHT_NUM - 1] < (theApp.m_fHeightValue[HEIGHT_NUM - 2] - 20))
				{
					theApp.m_fHeightValue[HEIGHT_NUM - 1] = (theApp.m_fHeightValue[HEIGHT_NUM - 2] - 20);
				}
			}
		}
		else //调用失败
		{
			AfxMessageBox(L"123");
			CloseHandle(hCom);
			return 0;
		}
	}
	return 0;
}
double converse(double x)
{
	double dret;
	if (x>2 * PI)
	{
		dret = converse(x - 2 * PI);
	}
	else if (x<0)
	{
		dret = converse(x + 2 * PI);
	}
	else
	{
		dret = x;
	}
	return dret;
}

float addAngle(float x, float y)
{
	float dret = converse(x + y);
	/*if (dret>PI)
	{
	dret -= 2 * PI;
	}*/
	return dret;
}
float getValue(float * farray)
{
	float f = 0;
	float ftemp[12];
	memcpy(ftemp, farray, 48);
	for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = i + 1; j < 12; j++)
		{
			if (ftemp[i]>ftemp[j])
			{
				f = ftemp[i];
				ftemp[i] = ftemp[j];
				ftemp[j] = f;
			}
		}
	}
	f = ftemp[3] + ftemp[4] + ftemp[5] + ftemp[6] + ftemp[7] + ftemp[8];
	return f / 6;
}
double getAngle(double x, double y)
{
	double angle = 0;
	double dtemp, dret;
	if (x == 0 && y >= 0)
	{
		dtemp = PI / 2;
	}
	else if (x == 0 && y< 0)
	{
		dtemp = 3 * PI / 2;
	}
	else if (x<0 && y <= 0)   //相机一象限
	{
		dtemp = 2 * PI - atan(y / x);
	}
	else if (x<0 && y>0)   //相机四象限
	{
		dtemp = atan(-1 * y / x);
	}
	else if (x>0 && y <= 0)   //相机二象限
	{
		dtemp = PI + atan(-1 * y / x);
	}
	else if (x>0 && y>0)   //相机三象限
	{
		dtemp = PI - atan(y / x);
	}
	return dtemp;
}
DWORD WINAPI analyseData(LPVOID lpParam)
{
	int xpos, ypos, xmax = 0, ymax = 0, num = 0;
	double Tmax = -50000, t = 0,sum=0;
	for (;;)
	{
		//图像数据未及时更新
		if (b_isDelay)
		{
			b_isDelay = false;
			Sleep(200);
			continue;
		}
		if (GetTickCount() - theApp.m_iCount > 60)
		{
			Sleep(10);
			continue;
		}
		if (GetTickCount() - theApp.m_dwanalysetime < 100)
		{
			Sleep(10);
			continue;
		}
		theApp.m_dwanalysetime = GetTickCount();
		
		if (theApp.m_iTongNo == 0)
		{
			theApp.m_dvalue[7] = theApp.m_fHeightT1;
		}
		else
		{
			theApp.m_dvalue[7] = theApp.m_fHeightT2;
		}
		//更新料筒半径
		double dtemp = theApp.m_pClientSocket->SHYc(32);//物料高度
		if (dtemp < 0)
			dtemp = 0;

		/*if (theApp.m_pClientSocket->SHYx(31) == 0)
		{
			theApp.m_fRadia = dtemp*dtemp*dtemp*theApp.m_fa3_1 + theApp.m_fa2_1*dtemp*dtemp + theApp.m_fa1_1*dtemp + theApp.m_fa0_1;
		}
		else
		{
			/*theApp.m_fRadia = dtemp*dtemp*dtemp*theApp.m_fa3_2 + theApp.m_fa2_2*dtemp*dtemp + theApp.m_fa1_2*dtemp + theApp.m_fa0_2;

		}*/
		theApp.m_fRadia = getRadia(dtemp);
		if (theApp.m_iTongNo != theApp.m_pClientSocket->SHYx(31))//当前桶编号变化，重新获取桶圆心
		{
			theApp.m_iTongNo = theApp.m_pClientSocket->SHYx(31);
			
			/*theApp.m_ptRadio = theApp.m_ptVertex[theApp.m_iTongNo];
			int ix = theApp.m_ptRadio.x - theApp.m_ptVision.x;
			int iy = theApp.m_ptRadio.y - theApp.m_ptVision.y;
			theApp.m_fRadiaRadia = sqrt((double)ix*ix + (double)iy*iy);
			theApp.m_fHudRadia = getAngle(ix, iy);
			theApp.m_fRobotA4 = 0;*/
			getTong();
		}
		if (abs(theApp.m_fRobotA4 - theApp.m_pClientSocket->SHYc(31))>0.001)//A4轴变化，桶圆心也发生改变
		{
			theApp.m_fRobotA4 = theApp.m_pClientSocket->SHYc(31);
			/*double hud = addAngle(theApp.m_fHudRadia, theApp.m_pClientSocket->SHYc(31)*-1);
			theApp.m_ptRadio.x = theApp.m_ptVision.x - theApp.m_fRadiaRadia*cos(hud);
			theApp.m_ptRadio.y = theApp.m_ptVision.y + theApp.m_fRadiaRadia*sin(hud);*/

			getTong();
		}

		num = 0;
		t = 0;

		for (size_t i = 0; i < HEIGHT_NUM; i++) //更新测距值
		{
			if (theApp.m_fHeightValue[i]>0)
			{
				t += theApp.m_fHeightValue[i];
				num++;
			}
		}
		if (num != 0)
		{
			for (size_t i = 0; i < 12; i++)
			{
				float fmin = ((float)i / 6.0 - 1)*PI;
				float fmax = ((float)(i + 1) / 6.0 - 1)*PI;
				if (theApp.m_pClientSocket->SHYc(31) >= fmin && theApp.m_pClientSocket->SHYc(31) < fmax)
					theApp.m_fHeightHud[i] = t / (double)num;
			}
			//theApp.m_dvalue[3] = t / (double)num;
			if (theApp.m_pClientSocket->SHYx(33) != 2)
				theApp.m_dvalue[3] = t / (double)num;
			else
			{
				theApp.m_dvalue[3] = getValue(theApp.m_fHeightHud);
				/*sum = 0;
				int num2 = 0;
				for (size_t i = 0; i < 12; i++)
				{
					if (theApp.m_fHeightHud[i] != 0)
					{
						sum += theApp.m_fHeightHud[i];
						num2++;
					}
				}
				if (num2!=0)
					theApp.m_dvalue[3] = sum / (double)num2;*/
			}
		}

		float aftemprature[384][288] = {0};
		Tmax = -50000;
		sum = 0;
		num = 0;

		for (size_t i = 0; i < g_CamInfo.intFPAWidth; i++)    //分析热像仪数据，取出温度信息
		{
			for (size_t j = 0; j < g_CamInfo.intFPAHeight; j++)
			{
				xpos = i* g_CamInfo.intShowWidth / g_CamInfo.intFPAWidth + g_CamInfo.intShowX;
				ypos = j*g_CamInfo.intShowHeight / g_CamInfo.intFPAHeight + g_CamInfo.intShowY;
				double distance = (xpos - theApp.m_ptRadio.x)*(xpos - theApp.m_ptRadio.x) + (ypos - theApp.m_ptRadio.y)*(ypos - theApp.m_ptRadio.y);
				//if (theApp.m_rgn.PtInRegion(xpos, ypos))
				if (distance <= theApp.m_fRadia*theApp.m_fRadia)
				{
					t = theApp.m_dTemp[j * g_CamInfo.intFPAWidth + i];
					if (t> theApp.m_maxT || t<theApp.m_minT)
						continue;
					if (t>Tmax)
					{
						Tmax = t;
						xmax = i;
						ymax = j;
					}
					sum += t;
					num++;
					if (theApp.m_pClientSocket->SHYx(33) == 2)
					{
						aftemprature[i][j] = t ;
					}
				}
			}
		}
		if (num!=0)
			theApp.averT = sum / (double)(num) ;
		theApp.maxT = Tmax ;
		theApp.max_x = xmax;
		theApp.max_y = ymax;
		if (theApp.m_pClientSocket->SHYx(33) == 2)
		{
			analyseHeat(aftemprature);
			addHeatList();
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CONTROLBAR, OnViewControlbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROLBAR, OnUpdateViewControlbar)
	ON_COMMAND(ID_VIEW_LOGBAR, OnViewLogbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LOGBAR, OnUpdateViewLogbar)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_DATA, &CMainFrame::OnViewData)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DATA, &CMainFrame::OnUpdateViewData)
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,			// status text
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_OVR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bClosed = FALSE;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX));
	//this->MoveWindow(100, 100, 1000, 800);
	/*float a4 = 5*PI/6;
	float a41 = -5 * PI / 6;
	float a5 = 600;
	float a51 = 600;
	double distance = a51*a51 + a5*a5 - 2 * a5*a51* cos(a41 - a4);
	if (abs(a41 - a4) > PI)
	{
		distance = (a5 + a51)*abs(a41 - a4)/2;
		distance = distance*distance;
	}*/

	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	m_dlgLog.Create(IDD_DIALOG_LOG, this);
	//m_dlgLog.ShowWindow(SW_SHOW);
	m_dlgLog.MoveWindow(CRect(30,700,530,850));
	
	TCHAR pszFile[_MAX_PATH];
	::GetModuleFileName(AfxGetApp()->m_hInstance, pszFile, _MAX_PATH);
	CString strPath = pszFile;
	int index = strPath.ReverseFind('\\');
	theApp.m_strFilePath = strPath.Left(index + 1);

	GetIni();

	m_dlgData.Create(IDD_DIALOG_RUNSTATUS, this);
	m_dlgData.ShowWindow(SW_SHOW);
	m_dlgData.MoveWindow(CRect(30, 460, 430, 700));

	m_dlgSetting.Create(IDD_DIALOG_SETTING, this);
	m_dlgSetting.ShowWindow(SW_SHOW);
	m_dlgSetting.CenterWindow();
	//m_dlgData.MoveWindow(CRect(750, 600, 450, 300));
	//m_dlgData.MoveWindow(CRect(550, 500, 850, 950));

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);
	
	DockControlBar(&m_wndToolBar);

	pFrame = this;
	if (theApp.m_iVisionType == 0)
	{
		//初始化通信客户端
		if (InitClient() != 0)
			AfxMessageBox(L"热像仪通信初始化失败");

		theApp.m_cfg = strCreate();
		//发现在线热像仪
		DiscoverCameraResp resps[3];
		int error = DiscoverOnlineCameras(resps, ARRAY_SIZE(resps));

		if (error >= 1)
		{
			uint cameraIp = resps[0].CameraIp;
			error = StartTransData(cameraIp, TransType_Tcp, DataType_Temp, FrameRecved);
		}
	}
	else
	{
			StartGetRGBAndTemperatureData(theApp.m_cVisionIP, FrameRecved2, 0);
	}
	//return 0;
	m_pComSocket = new CClientSocket2();	

	m_pClientSocket = new CClientSocket3();
	theApp.m_pClientSocket = m_pClientSocket;
	
	/*if (OpenRS4852(L"COM1", 19200) > 0)
	{
		;
	}*/
	if (theApp.m_iDMEType==1)
	{
		CreateThread(NULL, 0, ReadRader, NULL, 0, NULL);//分析超声测距值
	}
	else
	{
		OpenRS485(theApp.m_sRsPort, theApp.m_iRsRate);//分析激光测距值
	}
	getTongInfo();
	getRadiaInfo();
	//SocketConnect(m_sRobotIP, m_iRobotPort);
	SetTimer(1, 500, NULL);                 //网络断网重连
	SetTimer(2, 100, NULL);                 //分析检测坐标
	SetTimer(3, 20, NULL);                 //发送检测坐标
	CreateThread(NULL, 0, CameraOptThread, &m_dlgData, 0, NULL);//热像仪重连
	CreateThread(NULL, 0, analyseData, NULL, 0, NULL);//分析数据
	CreateThread(NULL, 0, analyseA4, NULL, 0, NULL);//分析A4数据
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style = WS_POPUP;
	//cs.x = 0;
	//cs.y = 0;
	cs.cx = 1000;
	cs.cy = 800;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
	
void CMainFrame::OnViewControlbar() 
{
	m_dlgSetting.ShowWindow(m_dlgSetting.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}

void CMainFrame::OnUpdateViewControlbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_dlgSetting.IsWindowVisible() ? 1 : 0);
}

void CMainFrame::OnViewLogbar() 
{
	m_dlgLog.ShowWindow( m_dlgLog.IsWindowVisible() ? SW_HIDE : SW_SHOW);
	
	if (m_dlgLog.IsWindowVisible())
	{
		m_dlgLog.PostMessage(WM_COMMAND, WM_UPDATE_LOG);
	}
}

void CMainFrame::OnUpdateViewLogbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_dlgLog.IsWindowVisible()?1:0);
}


void CMainFrame::OnViewData()
{
	m_dlgData.ShowWindow(m_dlgData.IsWindowVisible() ? SW_HIDE : SW_SHOW);
}


void CMainFrame::OnUpdateViewData(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_dlgData.IsWindowVisible() ? 1 : 0);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	CString str;
	
	if (pMsg->message==WM_COMMAND)
	{
		switch (pMsg->wParam)
		{
		case WM_DEVICE_ENUMREFRESH:
			break;
			Sleep(50);//等待所有相机已应答

			if (!m_bClosed)
			{
				MSG msg;
				while(PeekMessage(&msg, pMsg->hwnd, WM_COMMAND, WM_COMMAND, PM_NOREMOVE))
				{
					if (msg.wParam==WM_DEVICE_ENUMREFRESH)//如果队列中还有相邻的同样消息，就删除之，以免频繁刷新
					{
						PeekMessage(&msg, pMsg->hwnd, WM_COMMAND, WM_COMMAND, PM_REMOVE);
						//TRACE("PM_REMOVE WM_DEVICE_ENUMREFRESH");
					}
					else
					{
						break;
					}
				};
				
				//m_dlgControl.FillCamList();	
			}

			break;

		case WM_DEVICE_DISCONNECTED:
			break;
			/*if (MAG_IsProcessingImage())
			{
				MAG_StopProcessImage();
			}
			
			if (!m_bClosed)
			{
				m_dlgControl.m_bAutoLink = FALSE;
				m_dlgControl.FillCamList();
			}*/

			AppLog(_T("Camera diconnected."));

			break;

		case WM_DHCPSERVER_NOTIFY:
			break;
			switch(pMsg->lParam>>24)
			{
			case 1:
				str.Format(_T("offer ip: *.*.*.%d"), pMsg->lParam & 0xFF);
				break;
			case 3:
				str.Format(_T("bind ip: *.*.*.%d"), pMsg->lParam & 0xFF);
				break;
			case 4:
				str.Format(_T("decline ip: *.*.*.%d"), pMsg->lParam & 0xFF);
				break;
			case 7:
				str.Format(_T("release ip: *.*.*.%d"), pMsg->lParam & 0xFF);
				break;
			default:
				break;
			}

			AppLog(str);

			break;

		default:

			break;
		}
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnClose() 
{
	m_bClosed = TRUE;
	/*
	if (MAG_IsProcessingImage())
	{
		MAG_StopProcessImage();
	}

	if (MAG_IsDHCPServerRunning())
	{
		MAG_StopDHCPServer();
	}

	if (MAG_IsLinked())
	{
		MAG_DisLinkCamera();
	}

	if (MAG_IsInitialized())
	{
		MAG_Free();
	}*/
	
	CFrameWnd::OnClose();
}


// 创建Socket并连接
BOOL CMainFrame::SocketConnect(CAsyncSocket *pSocket, CString sIP, int iPort)
{
	if (pSocket->Create())
	{
		pSocket->AsyncSelect(FD_CONNECT | FD_READ | FD_CLOSE);
		BOOL bSucc = pSocket->Connect(sIP, iPort);

		if (bSucc)
		{
		}
		else
		{
			DWORD dwErr = GetLastError();
			if (dwErr == WSAEWOULDBLOCK)
			{
				//FD_CONNECT事件中SendRXLinkInfo

			}
			else
			{
				//delete m_pClientSocket;
				//m_pClientSocket = NULL;
				return FALSE;
			}
		}
	}
	else
	{
		//delete m_pClientSocket;
		//m_pClientSocket = NULL;
		return FALSE;
	}

	return TRUE;
}
// 断开Socket并销毁
BOOL CMainFrame::SocketDisconnect(CAsyncSocket *pSocket)
{
	/*if (m_pClientSocket)
	{
	m_pClientSocket->Close();
	delete m_pClientSocket;
	m_pClientSocket = NULL;

	}*/
	pSocket->Close();
	return TRUE;
}

void CMainFrame::GetIni()
{
	//Load Parameter
	CString szIniFile = theApp.m_strFilePath + L"RobotConfig.ini";
	CString str;
	CString stemp;
	wchar_t ccc[50];
	str = "RobotSetting";
	
	GetPrivateProfileString(str, L"RobotIP", L"192.168.110.170", ccc, 50, szIniFile);
	m_sRobotIP = ccc;
	m_iRobotPort = GetPrivateProfileInt(str, L"RobotPORT", 7807, szIniFile);
	GetPrivateProfileString(str, L"RobotRatio", L"2.3", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_ratio = _wtof(stemp);
	GetPrivateProfileString(str, L"RobotAngle", L"0.2", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_angle = _wtof(stemp);
	theApp.m_robotLen = GetPrivateProfileInt(str, L"RobotLen", 1500, szIniFile);

	theApp.m_robotx = GetPrivateProfileInt(str, L"RobotX", 233, szIniFile);
	theApp.m_roboty = GetPrivateProfileInt(str, L"RobotY", 122, szIniFile);

	GetPrivateProfileString(str, L"RobotHeight", L"179", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_dvalue[5] = _wtof(stemp);

	GetPrivateProfileString(str, L"RSPORT", L"COM5", ccc, 50, szIniFile);
	theApp.m_sRsPort = ccc;
	theApp.m_iRsRate = GetPrivateProfileInt(str, L"RSRATE", 115200, szIniFile);

	theApp.m_iDMEType = GetPrivateProfileInt(str, L"DMETYPE", 0, szIniFile);

	theApp.m_pix_min = GetPrivateProfileInt(str, L"PIX_MIN", 30, szIniFile);
	theApp.m_distance_min = GetPrivateProfileInt(str, L"DISTANCE_MIN", 140, szIniFile);
	GetPrivateProfileString(str, L"MINUS", L"0.05", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fMinus = _wtof(stemp);

	GetPrivateProfileString(str, L"Distance_A0", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa0_distance = _wtof(stemp);
	GetPrivateProfileString(str, L"Distance_A1", L"0.5", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa1_distance = _wtof(stemp);

	str = "CheckSetting";
	GetPrivateProfileString(str, L"MAX_T", L"60.2", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_maxT = _wtof(stemp);
	GetPrivateProfileString(str, L"MIN_T", L"20.2", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_minT = _wtof(stemp);
	GetPrivateProfileString(str, L"T_ROBOT", L"40.2", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_dvalue[4] = _wtof(stemp);
	GetPrivateProfileString(str, L"HEIGHT_STEP1", L"240", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_dvalue[6] = _wtof(stemp);
	GetPrivateProfileString(str, L"HEIGHT_FULL", L"1160", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fHeightT1 = _wtof(stemp);
	GetPrivateProfileString(str, L"HEIGHT_FULL2", L"1160", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fHeightT2 = _wtof(stemp);
	GetPrivateProfileString(str, L"T_EMERGENCY", L"62", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_dvalue[8] = _wtof(stemp);
	
	theApp.m_iFlag1 = GetPrivateProfileInt(str, L"INT_FLAG1", 10, szIniFile);
	GetPrivateProfileString(str, L"Cube1A3", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa3_1 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube1A2", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa2_1 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube1A1", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa1_1 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube1A0", L"249", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa0_1 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube2A3", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa3_2 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube2A2", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa2_2 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube2A1", L"0", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa1_2 = _wtof(stemp);
	GetPrivateProfileString(str, L"Cube2A0", L"249", ccc, 50, szIniFile);
	stemp = ccc;
	theApp.m_fa0_2 = _wtof(stemp);

	str = "VisionSetting";
	g_CamInfo.intShowX = GetPrivateProfileInt(str, L"VISION_X", 0, szIniFile);
	g_CamInfo.intShowY = GetPrivateProfileInt(str, L"VISION_Y", 0, szIniFile);
	g_CamInfo.intShowWidth = GetPrivateProfileInt(str, L"VISION_WIDTH", 0, szIniFile);
	g_CamInfo.intShowHeight = GetPrivateProfileInt(str, L"VISION_HEIGHT", 0, szIniFile);
	GetPrivateProfileString(str, L"VISIONIP", L"192.168.110.235", ccc, 50, szIniFile);
	size_t len = wcslen(ccc) + 1;
	size_t converted = 0;
	wcstombs_s(&converted, theApp.m_cVisionIP, len, ccc, _TRUNCATE);
	theApp.m_iVisionType = GetPrivateProfileInt(str, L"VISION_TYPE", 0, szIniFile);

	theApp.m_ptVision.x = theApp.m_robotx*g_CamInfo.intShowWidth / g_CamInfo.intFPAWidth + g_CamInfo.intShowX;
	theApp.m_ptVision.y = theApp.m_roboty*g_CamInfo.intShowHeight / g_CamInfo.intFPAHeight + g_CamInfo.intShowY;

	CString spalettefile = theApp.m_strFilePath + L"palettes";//palettes

	FILE *fp;
	long Filelen;
	char* a_pData;
	USES_CONVERSION;
	char * pFileName = T2A(spalettefile);

	if (fp = fopen(pFileName, "rb"))//(char *)spalettefile.GetBuffer(0)
	{
		fseek(fp, 0, SEEK_END);				//文件指针到文件尾
		Filelen = ftell(fp);				//读取文件长度
		//分配内存
		a_pData = (char*)malloc(Filelen);		//按文件长度申请内存空间
		//读取文件到分配的内存
		fseek(fp, 0, SEEK_SET);			//文件指针到文件头
		fread(a_pData, 1, Filelen, fp);		//读取文件到申请的内存中
		fclose(fp);
		for (size_t i = 0; i < 12; i++)
		{
			memcpy(theApp.m_spalette[i], a_pData + 1056 * i, 32);
			memcpy(theApp.m_palette[i], a_pData + 32 + 1056 * i, 1024);
		}
		free(a_pData);
	}
	else
		AppLog(L"调色板无法打开");
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	int iHeart = 0;
	int x, y;
	CString sinfo;

	if (nIDEvent == 1)
	{
		if (GetTickCount() - m_pClientSocket->m_iticket>3000)
		{
			m_pClientSocket->m_iticket = GetTickCount();
			SocketDisconnect(m_pClientSocket);
			m_pClientSocket->reset();
			m_pClientSocket->m_bConenctStates = FALSE;
			SocketConnect(m_pClientSocket,m_sRobotIP, m_iRobotPort);
			iHeart = 0;
		}
		if (GetTickCount() - m_pComSocket->m_iticket>3000)
		{
			m_pComSocket->m_iticket = GetTickCount();
			SocketDisconnect(m_pComSocket);
			m_pComSocket->m_bConenctStates = FALSE;
			SocketConnect(m_pComSocket, L"192.168.110.234", 4001);
			iHeart = 0;
		}
		iHeart++;
		if (iHeart >= 8 && m_pClientSocket->m_bConenctStates)
		{
			m_pClientSocket->SendHeartData();
			iHeart = 0;
		}
		if (m_pClientSocket->m_bConenctStates)
			m_dlgData.SetDlgItemTextW(IDC_STATIC_ROBOT_NETWORK, L"机器人网络连接:成功");
		else
			m_dlgData.SetDlgItemTextW(IDC_STATIC_ROBOT_NETWORK, L"机器人网络连接:失败");
	}
	else if (nIDEvent == 2)
	{		
		//x = theApp.value[4] % g_CamInfo.intFPAWidth;
		//y = g_CamInfo.intFPAHeight - theApp.value[4] / g_CamInfo.intFPAWidth - 1;
		//theApp.maxT = theApp.value[1] * 0.001f;sinfo.Format(L"最高温度:%0.1f", theApp.maxT);
		sinfo.Format(L"平均温度:%0.1f", theApp.averT);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO12, sinfo);
		x = theApp.max_x;
		y = theApp.max_y;
		sinfo.Format(L"最高温度:%0.1f", theApp.maxT);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO1, sinfo);
		sinfo.Format(L"最高温度位置:(%d,%d)", x,y);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO2, sinfo); 
		sinfo.Format(L"鼠标位置:(%d,%d)--(%d,%d)", theApp.m_pointx, theApp.m_pointy, theApp.m_ptMouseinClient.x, theApp.m_ptMouseinClient.y);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO5, sinfo);
		sinfo.Format(L"机器人A4轴:%f", theApp.m_fRobotA4);//m_pClientSocket->SHYc(31)
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO6, sinfo);
		//if (MAG_IsLinked())
		//	m_dlgData.SetDlgItemTextW(IDC_STATIC_CAMERA_NETWORK, L"热像仪连接:成功");
		//else
		//	m_dlgData.SetDlgItemTextW(IDC_STATIC_CAMERA_NETWORK, L"热像仪连接:失败");

		x -= theApp.m_robotx;
		y -= theApp.m_roboty;
		double daxis = theApp.m_dvalue[3] / theApp.m_dvalue[5];
		if (daxis>1 || daxis <= 0)
		{
			daxis = 1;
		}
		double radia = theApp.m_ratio*sqrt((double)x*x + (double)y*y) *daxis;
		double hud = getAngle(x, y);

		hud = hud + PI/2 + theApp.m_angle + theApp.m_pClientSocket->SHYc(31);
		hud = converse(hud);
		if (hud>PI)
		{
			hud -= 2 * PI;
		}
		sinfo.Format(L"机器人距离值:%.3f", theApp.m_distance);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO3, sinfo);
		sinfo.Format(L"机器人弧度值:%f", hud);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO4, sinfo);

		sinfo.Format(L"激光测距值:%f", theApp.m_dvalue[3]);//theApp.m_fHeightValue[theApp.m_iHeightNum - 1]
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO8, sinfo);

		sinfo.Format(L"当前料桶编号:%d", theApp.m_iTongNo + 1);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO9, sinfo);

		sinfo.Format(L"当前半径:%.1f", theApp.m_fRadia);
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO13, sinfo);

		if (m_pClientSocket->SHYx(33)==0)
			m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO11, L"当前上料模式:就绪");
		else if (m_pClientSocket->SHYx(33) == 1)
			m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO11, L"当前上料模式:打底");
		else if (m_pClientSocket->SHYx(33) == 2)
			m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO11, L"当前上料模式:探气上料");
		else
			m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO11, L"当前上料模式:结束铺料");

		sinfo.Format(L"当前物料高度:%f", theApp.m_pClientSocket->SHYc(32));
		m_dlgData.SetDlgItemTextW(IDC_STATIC_INFO10, sinfo);
		
		if (m_pClientSocket->SHYx(33)==0)  //单次测温模式
		{
			theApp.m_dvalue[0] = radia;
			theApp.m_dvalue[1] = hud;
			theApp.m_dvalue[2] = theApp.maxT;
			
		}
		m_pClientSocket->SetSHYcs(0, 8, theApp.m_dvalue);
		srand(GetTickCount());
		hud = 2 * PI*rand() / RAND_MAX-PI;
		m_pClientSocket->SetSHYc(31, hud);
		m_pClientSocket->SetSHYx(2, theApp.m_dvalue[0]);
		
	}
	else if (nIDEvent==3)
	{
		if (1 || m_pClientSocket->SHYx(32) && m_pClientSocket->SHYx(33) && b_flag1)			//请求下料坐标
		{
			int iNum = getHeat(theApp.m_dvalue[1], theApp.m_dvalue[0]);
			if (iNum > 0)
			{
				b_flag1 = false;
				theApp.m_iHeatCount += iNum;
				theApp.m_iHeatNum += iNum;
				if ((theApp.m_iHeatNum >= theApp.m_iFlag1) && (m_pClientSocket->SHYx(34) == 0))
				{
					theApp.m_iHeatNum = 0;
					m_pClientSocket->SetSHYx(34,1);
				}
			}
		}
		if (m_pClientSocket->SHYx(32) && m_pClientSocket->SHYx(33))
		{
			iCount++;
		}
		if ((m_pClientSocket->SHYx(32)==0) || (m_pClientSocket->SHYx(33)!=2) || (iCount>50))
		{
			b_flag1 = true;
			iCount = 0;
		}
		
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::ActivateFrame(int nCmdShow)
{
	// TODO:  在此添加专用代码和/或调用基类
	nCmdShow = SW_SHOWMAXIMIZED;
	CFrameWnd::ActivateFrame(nCmdShow);
}
//取出热点信息
void  analyseHeat(float dHeatinfo[384][288])
{
	float fmax = 0;
	int index_x = 0, index_y = 0;
	float Tflag = theApp.averT + theApp.m_dvalue[4];	//临界温度

	EnterCriticalSection(&theApp.m_cs2);
	for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listcurrentHeat.begin(); pHeatiterator != theApp.m_listcurrentHeat.end(); ++pHeatiterator)
	{
		struct_Heatinfo* pHeat = *pHeatiterator;
		delete pHeat;
	}
	theApp.m_listcurrentHeat.clear();//清空当前列表
	LeaveCriticalSection(&theApp.m_cs2);
	
	//分析本次数据，取出图像中的热点
	for (;;)
	{
		fmax = 1;
		for (int i = 0; i < 384; i++)
		{
			for (int j = 0; j < 288; j++)
			{
				if (dHeatinfo[i][j] > fmax)
				{
					fmax = dHeatinfo[i][j];
					index_x = i;
					index_y = j;
				}				
			}
		}
		if (fmax >= Tflag)
		{
			int ix = index_x - theApp.m_robotx;
			int iy = index_y - theApp.m_roboty;
			double daxis = theApp.m_dvalue[3] / theApp.m_dvalue[5];
			if (daxis>1 || daxis<=0)
			{
				daxis = 1;
			}
			double ftemp = theApp.m_ratio*sqrt((double)ix*ix + (double)iy*iy) *daxis;
			double hud = getAngle(ix, iy);
			hud = hud + PI / 2 + theApp.m_angle + theApp.m_fRobotA4;
			hud = converse(hud);
			if (hud>PI)
			{
				hud -= 2 * PI;
			}
			if (ftemp>1000)
			{
				continue;
			}
			struct_Heatinfo* pHeat = new struct_Heatinfo;
			pHeat->intShowX = index_x;
			pHeat->intShowY = index_y;
			pHeat->ftemprature = dHeatinfo[index_x][index_y];
			pHeat->m_dwDeleteTime = 0;
			pHeat->fspeedA4 = theApp.m_speedA4;
			pHeat->fRadia = ftemp;
			pHeat->fHud = hud;
			pHeat->fA5 = asin(ftemp / theApp.m_robotLen);
			theApp.m_listcurrentHeat.push_back(pHeat);
			int startx = index_x - theApp.m_pix_min;
			int endx = index_x + theApp.m_pix_min;
			int starty = index_y - theApp.m_pix_min;
			int endy = index_y + theApp.m_pix_min;
			if (startx < 0)
				startx = 0;
			if (endx >384)
				endx = 384;
			if (starty < 0)
				starty = 0;
			if (endy >288)
				endy = 288;
			for (size_t i = startx; i < endx; i++)
			{
				for (size_t j = starty; j < endy; j++)
				{
					dHeatinfo[i][j] = 0;
				}
			}
		}
		else
			break;
	}	
}

void addHeatList()
{
	EnterCriticalSection(&theApp.m_cs);
	//更新热点信息列表
	DWORD dwCount = GetTickCount();

	for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end();)
	{
		struct_Heatinfo* pHeat = *pHeatiterator;
		if (pHeat->m_dwDeleteTime != 0 && dwCount - pHeat->m_dwDeleteTime > 5000)
		{
			std::list<struct_Heatinfo*>::iterator pHeatiterator12 = pHeatiterator;
			pHeatiterator++;
			delete pHeat;
			theApp.m_listpHeat.erase(pHeatiterator12);
			continue;
		}
		pHeatiterator++;
	}

	for (std::list<struct_Heatinfo*>::iterator pcurrentHeatiterator = theApp.m_listcurrentHeat.begin(); pcurrentHeatiterator != theApp.m_listcurrentHeat.end(); ++pcurrentHeatiterator)
	{
		struct_Heatinfo* pcurrentHeat = *pcurrentHeatiterator;
		for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end();)
		{
			struct_Heatinfo* pHeat = *pHeatiterator;
			double dd = pcurrentHeat->fRadia*pcurrentHeat->fRadia + pHeat->fRadia*pHeat->fRadia - 2 * pcurrentHeat->fRadia
				*pHeat->fRadia* cos(pHeat->fHud - pcurrentHeat->fHud);
			if (dd<theApp.m_distance_min*theApp.m_distance_min)//170
			{
				if (pHeat->fspeedA4<pcurrentHeat->fspeedA4)	//精确率
				{
					pcurrentHeat->fspeedA4 = 30;
					break;
				}
				if (pHeat->m_dwDeleteTime != 0)
				{
					pcurrentHeat->fspeedA4 = 30;
					break;
				}
				std::list<struct_Heatinfo*>::iterator pHeatiterator12 = pHeatiterator;
				pHeatiterator++;
				delete pHeat;
				theApp.m_listpHeat.erase(pHeatiterator12);
			}
			else
				pHeatiterator++;
		}
	}

	for (std::list<struct_Heatinfo*>::iterator pcurrentHeatiterator = theApp.m_listcurrentHeat.begin(); pcurrentHeatiterator != theApp.m_listcurrentHeat.end(); ++pcurrentHeatiterator)
	{
		struct_Heatinfo* pcurrentHeat = *pcurrentHeatiterator;
		if (pcurrentHeat->fspeedA4 < 29)
		{
			struct_Heatinfo* pHeat = new struct_Heatinfo;
			pHeat->intShowX = pcurrentHeat->intShowX;
			pHeat->intShowY = pcurrentHeat->intShowY;
			pHeat->ftemprature = pcurrentHeat->ftemprature;
			pHeat->m_dwDeleteTime = 0;
			pHeat->fspeedA4 = pcurrentHeat->fspeedA4;
			pHeat->fRadia = pcurrentHeat->fRadia;
			pHeat->fHud = pcurrentHeat->fHud;
			pHeat->fA5 = pcurrentHeat->fA5;
			theApp.m_listpHeat.push_back(pHeat);
		}
	}

	LeaveCriticalSection(&theApp.m_cs);
}

//获取最近的热点
int getHeat(double fA4, double fA5)
{
	double fmin0 = 28000000000;
	double fmin1 = 28000000000;
	double fmax1 = 0;
	float TEmergency = theApp.m_dvalue[8] + theApp.averT;

	EnterCriticalSection(&theApp.m_cs);
	if (theApp.m_listpHeat.empty())
	{
		LeaveCriticalSection(&theApp.m_cs);
		return 0;
	}
	DWORD dw = GetTickCount();
	std::list<struct_Heatinfo*>::iterator  pindex0 = theApp.m_listpHeat.begin();  //指向最近的
	std::list<struct_Heatinfo*>::iterator  pindex1 = theApp.m_listpHeat.begin(); //指向最远的
	std::list<struct_Heatinfo*>::iterator  pindex2 = theApp.m_listpHeat.begin();  //指向最近的紧急温度
	bool bflag = false;
	bool bEmergency = false;
	bool bEmpty = true;

	for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
	{
		struct_Heatinfo* pHeat = *pHeatiterator;
		if (pHeat->m_dwDeleteTime!=0)
		{
			continue;
		}
		bEmpty = false;
		double distance = fA5*fA5 + pHeat->fRadia*pHeat->fRadia - 2 * fA5*pHeat->fRadia* cos(pHeat->fHud - fA4);
		if (abs(pHeat->fHud - fA4) > PI)
		{
			distance = (fA5 + pHeat->fRadia)*(fA5 + pHeat->fRadia)*abs(pHeat->fHud - fA4)*abs(pHeat->fHud - fA4);
		}

		if (pHeat->ftemprature >= TEmergency)
		{
			double ddd = getLineInfo(0, fA4, fA5, pHeat->fHud, pHeat->fRadia);
			if (ddd<fmin0)
			{
				bEmergency = true;
				fmin0 = ddd;
				pindex2 = pHeatiterator;//找出最近的紧急热点
			}
		}
		if (!bEmergency)
		{
			double ddd = getLineInfo(1,fA4, fA5, pHeat->fHud, pHeat->fRadia);

			if (ddd<fmin1)
			{
				fmin1 = ddd;
				pindex0 = pHeatiterator;
			}
		}
		if (abs(pHeat->fHud - fA4)>1.5*PI)
		{
			distance = 9999999;
		}		
		if (distance>fmax1)
		{
			fmax1 = distance;
			pindex1 = pHeatiterator;
		}
	}
	if (bEmpty)
	{
		LeaveCriticalSection(&theApp.m_cs);
		return 0;
	}
	std::list<struct_Heatinfo*>::iterator  presult ;  //指向最后的结果
	if (bEmergency)
	{
		presult = pindex2;
	}
	else
	{
		if (theApp.m_bA4Alarm && b_flag3)
		{
			presult = pindex1;
			b_flag3 = false;
		}
		else
		{
			b_flag3 = true;
			presult = pindex0;
		}
	}
	struct_Heatinfo* pHeat0 = *presult;
	theApp.m_dvalue[0] = pHeat0->fRadia;
	theApp.m_dvalue[1] = pHeat0->fHud;
	theApp.m_dvalue[2] = pHeat0->ftemprature;
	//delete pHeat0;
	//theApp.m_listpHeat.erase(pindex0);

	int iRet = refreshHeat(fA4, asin(fA5 / theApp.m_robotLen), theApp.m_dvalue[1], asin(theApp.m_dvalue[0] / theApp.m_robotLen));
	LeaveCriticalSection(&theApp.m_cs);
	theApp.m_pClientSocket->SetSHYx(32, 0);
	theApp.m_pClientSocket->SetSHYcs(0, 8, theApp.m_dvalue);
	theApp.m_dwSend = GetTickCount();
	return iRet;
}
//获取直线的热点信息
double getLineInfo(int iType, double startA4, double startRadia, double endA4, double endRadia)
{
	double startA5 = asin(startRadia / theApp.m_robotLen);
	double endA5 = asin(endRadia / theApp.m_robotLen);
	double k = (endA5 - startA5) / (endA4 - startA4);
	double a = startA5 - k*startA4;
	int num=0;
	float TEmergency = theApp.m_dvalue[8] + theApp.averT;
	if (TEmergency > 45)
		TEmergency = 45;
	if (startA4 < endA4)
	{
		for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
		{
			struct_Heatinfo* pHeat = *pHeatiterator;
			if (pHeat->m_dwDeleteTime!=0)
			{
				continue;
			}
			if (iType == 0 && pHeat->ftemprature < TEmergency)
			{
				continue;
			}
			if (pHeat->fHud >= startA4 && pHeat->fHud <= endA4)//在区间内
			{
				//double delta = (sin(k*pHeat->fHud + a) - sin(pHeat->fA5))*theApp.m_robotLen;
				double delta = k*pHeat->fHud + a - pHeat->fA5;
				if (abs(delta) < theApp.m_fMinus)
				{
					num++;
				}
			}
		}
	}
	else
	{
		for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end(); ++pHeatiterator)
		{
			struct_Heatinfo* pHeat = *pHeatiterator;
			if (pHeat->m_dwDeleteTime!=0)
			{
				continue;
			}
			if (iType == 0 && pHeat->ftemprature < TEmergency)
			{
				continue;
			}
			if (pHeat->fHud <= startA4 && pHeat->fHud >= endA4)//在区间内
			{
				double delta = k*pHeat->fHud + a - pHeat->fA5;
				if (abs(delta) < theApp.m_fMinus)
				{
					num++;
				}
			}
		}
	}
	double distance = startRadia*startRadia + endRadia*endRadia - 2 * startRadia*endRadia* cos(startA4 - endA4);
	if (abs(startA4 - endA4) > PI)
	{
		distance = (startRadia + endRadia)*abs(startA4 - endA4) / 2;
		distance = distance*distance;
	}
	if (num<=1)
	{
		return distance;
	}
	else
		return 0.6*distance/num;
}
//删除指定直线上的热点
int refreshHeat(double startA4, double startA5, double endA4, double endA5)
{
	double k = (endA5 - startA5) / (endA4 - startA4);
	double a = startA5 - k*startA4;
	int iRet = 0;
	if (startA4 < endA4)
	{
		for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end();pHeatiterator++ )
		{
			struct_Heatinfo* pHeat = *pHeatiterator;
			if (pHeat->fHud >= startA4 && pHeat->fHud <= endA4)//在区间内
			{
				double delta = k*pHeat->fHud + a - pHeat->fA5;
				if (abs(delta) < theApp.m_fMinus)
				{
					pHeat->m_dwDeleteTime = GetTickCount();
					iRet++;
				}
			}			
		}
	}
	else
	{
		for (std::list<struct_Heatinfo*>::iterator pHeatiterator = theApp.m_listpHeat.begin(); pHeatiterator != theApp.m_listpHeat.end();pHeatiterator++)
		{
			struct_Heatinfo* pHeat = *pHeatiterator;
			if (pHeat->fHud <= startA4 && pHeat->fHud >= endA4)//在区间内
			{
				double delta = k*pHeat->fHud + a - pHeat->fA5;
				if (abs(delta) < theApp.m_fMinus)
				{
					pHeat->m_dwDeleteTime = GetTickCount();
					iRet++;
				}
			}			
		}
	}
	return iRet;
}
//最小二乘法拟合直线
double binsolve(double *dvalue,int num)
{
	double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
//	double ss[5] = { 3.1, 4.1, 6.1, 7.1, 9.1 };
	for (size_t i = 0; i < num; i++)
	{
		t1 += i;
		t2 += dvalue[i];
		t3 += i * i;
		t4 += i*dvalue[i];
	}
	double k = (t4 - t1*t2 / num) / (t3 - t1*t1 / num);
	double r = t2 / num - k*t1 / num;
	double dret = k*num + r;
	//theApp.m_speedA4 = k;
	return dret;
}

int getTongInfo()
{
	sqlite3 *m_db;
	sqlite3_stmt *m_stmt;
	int result = 0;
	//CString sfilename = GetDir() + "hnc3.s3db";
	char * pfilename = "hnc3.db";
	result = sqlite3_open(pfilename, &m_db);
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //提高性能
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //加大缓存
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //返回改变记录数
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //支持中文LIKE查询

	CString stemp;
	char * pSQL = "select * from TongInfo";
	result = sqlite3_prepare_v2(m_db, pSQL, strlen(pSQL), &m_stmt, 0);
	if (result != 0)
	{
		sqlite3_close(m_db);
		return result;
	}
	while (sqlite3_step(m_stmt) == SQLITE_ROW)
	{
		struct_TongInfo*pInfo = new struct_TongInfo;
		pInfo->iNo = sqlite3_column_int(m_stmt, 0);
		pInfo->intTongNo = sqlite3_column_int(m_stmt, 4);
		pInfo->intShowX = sqlite3_column_int(m_stmt, 2);
		pInfo->intShowY = sqlite3_column_int(m_stmt, 3);
		pInfo->fA4 = sqlite3_column_double(m_stmt, 1);
		theApp.m_listTongInfo.push_back(pInfo);
	}
	sqlite3_close(m_db);
}

void getTong()
{
	double sabs = 10;
	bool bFind = false;
	std::list<struct_TongInfo*>::iterator pTongiterator1;
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
		theApp.m_ptRadio.x = pInfo->intShowX;
		theApp.m_ptRadio.y = pInfo->intShowY;
	}
}

int getRadiaInfo()
{
	sqlite3 *m_db;
	sqlite3_stmt *m_stmt;
	int result = 0;
	//CString sfilename = GetDir() + "hnc3.s3db";
	char * pfilename = "hnc3.db";
	result = sqlite3_open(pfilename, &m_db);
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //提高性能
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //加大缓存
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //返回改变记录数
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //支持中文LIKE查询

	CString stemp;
	char * pSQL = "select * from RadiaInfo";
	result = sqlite3_prepare_v2(m_db, pSQL, strlen(pSQL), &m_stmt, 0);
	if (result != 0)
	{
		sqlite3_close(m_db);
		return result;
	}
	while (sqlite3_step(m_stmt) == SQLITE_ROW)
	{
		struct_RadiaInfo*pInfo = new struct_RadiaInfo;
		pInfo->iNo = sqlite3_column_int(m_stmt, 0);
		pInfo->intTongNo = sqlite3_column_int(m_stmt, 1);
		pInfo->iHeight = sqlite3_column_int(m_stmt, 2);
		pInfo->iRadia = sqlite3_column_int(m_stmt, 3);
		theApp.m_listRadiaInfo.push_back(pInfo);
	}
	sqlite3_close(m_db);
}
float getRadia(float height)
{
	double sabs = 65535;
	float fret = 244;
	for (std::list<struct_RadiaInfo*>::iterator pTongiterator = theApp.m_listRadiaInfo.begin(); pTongiterator != theApp.m_listRadiaInfo.end(); ++pTongiterator)
	{
		struct_RadiaInfo*pInfo = *pTongiterator;
		if (pInfo->intTongNo == theApp.m_iTongNo)
		{
			double delta = height - pInfo->iHeight;
			if (delta>=0 && delta<sabs)
			{
				sabs = delta;
				fret = pInfo->iRadia;
			}
		}
	}
	
	return fret;
}
#include   <afxpriv.h>
int insertOpt(int nType)
{
	return 0;
	sqlite3 *m_db;
	sqlite3_stmt *m_stmt;
	int result = 0;
	//CString sfilename = GetDir() + "hnc3.s3db";
	char * pfilename = "hnc3.db";
	result = sqlite3_open(pfilename, &m_db);
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", 0, 0, 0);    //提高性能
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA cache_size = 8000", 0, 0, 0); //加大缓存
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA count_changes = 1", 0, 0, 0); //返回改变记录数
	if (result != 0)
		return result;
	result = sqlite3_exec(m_db, "PRAGMA case_sensitive_like = 1", 0, 0, 0); //支持中文LIKE查询
	CString stime,strsql;

	CTime t = CTime::GetCurrentTime();
	stime = t.Format("%Y-%m-%d %H:%M:%S");
	strsql.Format(L"insert into OptInfo([Time],[TongNo],[Opt]) values('%s',%d,%d)", stime, theApp.m_iTongNo + 1, nType);

	USES_CONVERSION;

	//调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	char * cSQL = T2A(strsql);

	sqlite3_exec(m_db, cSQL, 0, 0, NULL);

	sqlite3_close(m_db);
}