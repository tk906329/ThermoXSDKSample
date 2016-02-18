// ThermoXSDKSample.h : ThermoXSDKSample 應用程式的主標頭檔
//
#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"       // 主要符號

#include "./sdk/online.h"
#include "./sdk/alg.h"
#include "./sdk/alg_strech.h"
#include "./sdk/offline.h"
#include "./sdk/RobotInspection.h"
#include<list>
#include<algorithm>

#pragma comment(lib,"./sdk/dll/CameraClient.lib")
#pragma comment(lib,"./sdk/dll/RobotInspection.lib")
// CTIApp:
// 請參閱實作此類別的 ThermoXSDKSample.cpp
//

#define HEIGHT_NUM  50

struct struct_CamInfo
{
	int intFPAWidth;     
	int intFPAHeight;
	int intMaxFPS;
	int intCurrentFPS;
	int intShowWidth;
	int intShowHeight;
	int intShowX;
	int intShowY;
};

struct struct_Heatinfo//热点信息
{
	int intShowX;
	int intShowY;
	double fRadia;
	double fHud;
	double fA5;
	float ftemprature;
	float fspeedA4;
	DWORD m_dwDeleteTime;
};
struct struct_TongInfo//桶圆心信息
{
	double fA4;
	int iNo;		//	序号
	int intTongNo;	//桶编号
	int intShowX;	//
	int intShowY;	//
};
struct struct_RadiaInfo//桶半径信息
{
	int iNo;		//	序号
	int intTongNo;	//桶编号
	int iHeight;	//
	int iRadia;	//
};
class CTIApp : public CWinApp
{
public:
	CTIApp();
	CRgn m_rgn;      
	DWORD m_iCount;    //数据帧更新时间戳
	DWORD m_dwanalysetime;     //数据分析时间

	std::list<struct_Heatinfo*>	m_listpHeat;	//热点信息列表
	std::list<struct_Heatinfo*>	m_listcurrentHeat;	//当前热点信息列表
	std::list<struct_TongInfo*>	m_listTongInfo;	//桶圆心信息
	std::list<struct_RadiaInfo*>	m_listRadiaInfo;	//桶半径信息
	//CPtrList m_listHeat;

	bool m_bVisionUpdateFlag;				//实时图像更新标志
	int m_pix_min, m_distance_min;
	float m_fMinus, m_fa0_distance, m_fa1_distance;
	float m_maxT, m_minT;    //温度检测范围
	//int value[5];
	double m_angle;      //热像仪与A4轴夹角
	double m_ratio;      //长度转换系数
	int m_robotx, m_roboty;    //参考点坐标
	int max_x, max_y;    //最高温度位置
	float maxT;			//最高温
	float averT;		//平均温
	CClientSocket3 *m_pClientSocket;
	int m_pointx, m_pointy;    //鼠标像素位置
	float m_mouseT;   //鼠标指向温度
	CPoint m_ptMouseinClient;   //鼠标位置

	DWORD m_dwComData;
	strech_cfg *m_cfg;		//拉伸配置
	int m_iVisionType;//相机类型：0，巨哥；1，高德
	char m_cVisionIP[20];	//相机IP

	int m_paletteindex;
	bgra m_palette[12][256];        //调色板
	char m_spalette[12][32];   //调色板名字
	bgra m_bmp[384 * 288];
	byte m_bmp_guide[384 * 288 * 3];
	BYTE m_pIRBmpData[384 * 288 * 4 + 54];
	BYTE m_pIRBmpData2[384 * 288 * 3 + 54];
	double m_dTemp[384 * 288];

	CString m_strFilePath ;   //程序路径
	double m_fa3_1, m_fa2_1, m_fa1_1, m_fa0_1, m_fa3_2, m_fa2_2, m_fa1_2, m_fa0_2;//料桶的相关参数：

	int m_iDMEType;//测距仪器类型：0，激光；1，超声波
	float m_fRadia;          //桶实时半径
	float m_fRobotA4;        //机器人A4轴坐标
	CPoint m_ptRadio;       //桶圆心实时坐标
	CPoint m_ptVision;       //相机圆心坐标
	double m_dvalue[10];   //和控制器间数据通讯
	float m_fHeightHud[12];  //各个弧度的测距值
	float m_fHeightValue[HEIGHT_NUM];   //测距值取样
	float m_distance;		//测距实时值
	int m_iTongNo;			//当前料桶编号
	CPoint m_ptVertex[2];    //视窗中桶圆心基准坐标

	DWORD m_dwSend;				//上料坐标发送时间

	CString m_sRsPort;
	int m_iRsRate;
	// 临界区结构对象
	CRITICAL_SECTION m_cs;
	CRITICAL_SECTION m_cs2;

	int m_iHeatNum, m_iHeatCount;				//热点数量
	int m_iFlag1;				//外圈压气阀值
	float m_fHeightT1;			//料桶1满的条件
	float m_fHeightT2;			//料桶2满的条件

	double m_valueA4[5];		//A4轴变化趋势
	double m_speedA4;			//A4轴变化斜率
	float m_robotLen;			//机器人小臂长度

	bool m_bA4Alarm;			//A4轴长时间在一个方向
// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTIApp theApp;
