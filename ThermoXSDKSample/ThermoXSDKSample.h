// ThermoXSDKSample.h : ThermoXSDKSample ���ó�ʽ�������^�n
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �����˙n��ǰ�Ȱ��� 'stdafx.h'"
#endif

#include "resource.h"       // ��Ҫ��̖

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
// Ո��醌�����e�� ThermoXSDKSample.cpp
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

struct struct_Heatinfo//�ȵ���Ϣ
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
struct struct_TongInfo//ͰԲ����Ϣ
{
	double fA4;
	int iNo;		//	���
	int intTongNo;	//Ͱ���
	int intShowX;	//
	int intShowY;	//
};
struct struct_RadiaInfo//Ͱ�뾶��Ϣ
{
	int iNo;		//	���
	int intTongNo;	//Ͱ���
	int iHeight;	//
	int iRadia;	//
};
class CTIApp : public CWinApp
{
public:
	CTIApp();
	CRgn m_rgn;      
	DWORD m_iCount;    //����֡����ʱ���
	DWORD m_dwanalysetime;     //���ݷ���ʱ��

	std::list<struct_Heatinfo*>	m_listpHeat;	//�ȵ���Ϣ�б�
	std::list<struct_Heatinfo*>	m_listcurrentHeat;	//��ǰ�ȵ���Ϣ�б�
	std::list<struct_TongInfo*>	m_listTongInfo;	//ͰԲ����Ϣ
	std::list<struct_RadiaInfo*>	m_listRadiaInfo;	//Ͱ�뾶��Ϣ
	//CPtrList m_listHeat;

	bool m_bVisionUpdateFlag;				//ʵʱͼ����±�־
	int m_pix_min, m_distance_min;
	float m_fMinus, m_fa0_distance, m_fa1_distance;
	float m_maxT, m_minT;    //�¶ȼ�ⷶΧ
	//int value[5];
	double m_angle;      //��������A4��н�
	double m_ratio;      //����ת��ϵ��
	int m_robotx, m_roboty;    //�ο�������
	int max_x, max_y;    //����¶�λ��
	float maxT;			//�����
	float averT;		//ƽ����
	CClientSocket3 *m_pClientSocket;
	int m_pointx, m_pointy;    //�������λ��
	float m_mouseT;   //���ָ���¶�
	CPoint m_ptMouseinClient;   //���λ��

	DWORD m_dwComData;
	strech_cfg *m_cfg;		//��������
	int m_iVisionType;//������ͣ�0���޸磻1���ߵ�
	char m_cVisionIP[20];	//���IP

	int m_paletteindex;
	bgra m_palette[12][256];        //��ɫ��
	char m_spalette[12][32];   //��ɫ������
	bgra m_bmp[384 * 288];
	byte m_bmp_guide[384 * 288 * 3];
	BYTE m_pIRBmpData[384 * 288 * 4 + 54];
	BYTE m_pIRBmpData2[384 * 288 * 3 + 54];
	double m_dTemp[384 * 288];

	CString m_strFilePath ;   //����·��
	double m_fa3_1, m_fa2_1, m_fa1_1, m_fa0_1, m_fa3_2, m_fa2_2, m_fa1_2, m_fa0_2;//��Ͱ����ز�����

	int m_iDMEType;//����������ͣ�0�����⣻1��������
	float m_fRadia;          //Ͱʵʱ�뾶
	float m_fRobotA4;        //������A4������
	CPoint m_ptRadio;       //ͰԲ��ʵʱ����
	CPoint m_ptVision;       //���Բ������
	double m_dvalue[10];   //�Ϳ�����������ͨѶ
	float m_fHeightHud[12];  //�������ȵĲ��ֵ
	float m_fHeightValue[HEIGHT_NUM];   //���ֵȡ��
	float m_distance;		//���ʵʱֵ
	int m_iTongNo;			//��ǰ��Ͱ���
	CPoint m_ptVertex[2];    //�Ӵ���ͰԲ�Ļ�׼����

	DWORD m_dwSend;				//�������귢��ʱ��

	CString m_sRsPort;
	int m_iRsRate;
	// �ٽ����ṹ����
	CRITICAL_SECTION m_cs;
	CRITICAL_SECTION m_cs2;

	int m_iHeatNum, m_iHeatCount;				//�ȵ�����
	int m_iFlag1;				//��Ȧѹ����ֵ
	float m_fHeightT1;			//��Ͱ1��������
	float m_fHeightT2;			//��Ͱ2��������

	double m_valueA4[5];		//A4��仯����
	double m_speedA4;			//A4��仯б��
	float m_robotLen;			//������С�۳���

	bool m_bA4Alarm;			//A4�᳤ʱ����һ������
// ����
public:
	virtual BOOL InitInstance();

// ��ʽ�a����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTIApp theApp;
