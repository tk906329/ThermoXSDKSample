#pragma once
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ROBOTINSPECTION_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ROBOTINSPECTION_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef ROBOTINSPECTION_EXPORTS
#define ROBOTINSPECTION_API __declspec(dllexport)
#else
#define ROBOTINSPECTION_API __declspec(dllimport)
#endif

//����ָ��
#define returnValue_Success  1
#define returnValue_Fail 0
#define returnValue_alreadyLonOn -1
#define returnValue_logOnFail -2
#define returnValue_noServerResponse  -3
#define returnValue_commucationError -4
#define returnValue_Update_openFileFail -5
#define returnValue_Update_FileValidateFail -6
#define returnValue_OpenStreamFail -7
#define returnValue_EditCurrentUserPrivilegeError -8
#define returnValue_CurrentUserPrivilegeError -9
#define returnValue_UserNameEmptyError -10

typedef struct tagRGBANDTEMPERATUREDATA_CB
{
	BYTE* pRGBImgData;// rgb����
	int RGBImgDataLen;// rgb���ݳ���
	double* MeasureMatrix; //�¶Ⱦ���
	int matrixDataLen;
	short* CurveData;
	int CurveLen;
	int ImgWidth;
	int ImgHeight;
	int iMFocusSpeed; //�ֶ������ٶ�
	int iTouchNearSwitch; //�Ƿ�����������λ���� 0-�� 1-��
	int iTouchFarSwitch; //�Ƿ�����Զ����λ���� 0-�� 1-��
	int iAutoFocusFinish; //�Զ��Խ��Ƿ���� 0-�� 1-��
	int iAFocusForwardSpeed; //�Զ��Խ�ǰ���ٶ�
	int iAFocusBackSpeed; //�Զ��Խ������ٶ�
	int iAFocusTotalFrame; //�Զ��Խ�ͳ��֡��
	int iDimmingMode; //����ģʽ
	short AutoThrowPointRatio; //�Զ��׵����
	short Brightness; 	//����
	short Contrast; 		//�Աȶ�
	short MDimY16Min;  //�ֶ������Y16Min
	short MDimY16Max;  //�ֶ������Y16Max
	short LsothermY16Min; //������Y16Min
	short LsothermY16Max; //������Y16Max
	int iSpecialColorNum; //����ɫ�ʱ��
	int iLsothermMode; //������ģʽ
	int iPaletteNum; //��ɫ��
	int iShutterCompensationTimeSetting; //���Ŷ�ʱ����ʱ������
	int iShutterStatus; //����״̬��0-û�д���� 1-���ڴ����
	int iRecoveryStatus; //�ָ�״̬��1-�ָ��������óɹ� 2-�ָ��û����óɹ�
	int iSaveSetting; //�������� 1-����ɹ�
	int iTimingCompensationFunction; //��ʱ�������� 0-�� 1-��
	short CursorStatus; //���״̬
	short CursorXValue; //X����
	short CursorYValue; //Y����
	short CursorADValue; //��괦ADֵ
	int iTestImageModeSetting; //����ͼ��ģʽ����
	int iRemoveBarAlgorithmSwitch; //ȥ�����㷨����
	int iRemoveNoiseAlgorithmSwitch; //ȥ���㷨����
	int iEnhanceAlgorithmSwitch; //��ǿ�㷨����
	int iFreezeSwitch; //���񿪹�
	int iDigitalContextSelectFlag; //���ֿڴ�������ѡ���־
	int iNETDTest; //NETD���Կ���
	int iFrameRate; //֡Ƶ 25/9
	short DetectorTemp; //̽������ƽ���¶�	
	WORD	ProgramVersion;  //����汾
	int iGFID;
	int iVSK;
	int iGAIN;
	int iIntegrationTime;
	int iEmiss;
	int iDistance;
	int iRelHum;
	int iBackgroundTemp;
	int iBackgroundTempFlag;
	WORD MeasureRangeInfo; //���·�Χ��Ϣ
	WORD VersionInfo; //��ʽ�����԰���Ϣ
	short DetectorGear;	//̽������λ
	WORD SecondChangeGearTime; //�ڶ����е�ʱ��
	short Y16_Max;//����Y16���ֵ
	short Y16_Max_x;
	short Y16_Max_y;
	short Y16_Average;//y16�м�ֵ
	short Y16_Min;//y16��Сֵ
	short Y16_Min_x;
	short Y16_Min_y;
	short Y16_Center;//Y16�м�ֵCenterAD
	short Y16_Center_x;
	short Y16_Center_y;
	int iMeasureCursorShowMode; //���¹����ʾģʽ 0-�����ʾ���ֵ������ֵ��Ĭ�ϣ� 1-���ֵ����Сֵ 2-����ֵ����Сֵ
	int iAnalyzeMode; //����ģʽ 0-�رշ��� 1-����� 2-�������
	short Temp101; //�źŰ�101�´��¶�
	short Area_x; //�������Ͻ�X����
	short Area_y; //�������Ͻ�Y����
	short AreaWidth; //�����
	short AreaHeight; //����
	short LastShutterTemp; //��һ�ο����¶�
	int iY16Flag;
	short AtmosphereTemp;  //�����¶�
	short RealShutterTemp; //ʵʱ�����¶�
	int iT_Center;
	int iT_Max;
	int TempBeforeCorrection;
}RGBANDTEMPERATUREDATA_CB;

typedef enum CommandType
{
	FocusFar = 1,		// ��Զ��	
	FocusNear,			// ������ 
	FocusStop,			// ����ͣ
	AutoFocus,			// �Զ��۽�
	ZoomOne,			// 1���Ŵ�	
	ZoomTwo,			// 2���Ŵ�
	ZoomFour,			// 4���Ŵ�
	Bright,					// ����
	Contrast,				// �Աȶ�
	PTZUp,					// ��̨����-��
	PTZDown,				// ��̨����-��
	PTZLeft,				// ��̨����-��
	PTZRight,				// ��̨����-��
	PTZLeftUp,			// ��̨����-����
	PTZRightUp,			// ��̨����-����
	PTZLeftDown,		// ��̨����-����
	PTZRightDown,		// ��̨����-����
	PTZStop,				// ��̨����-ͣ
	PTZSetPre,			// ��̨����-����Ԥ��λ
	PTZRunPre,			// ��̨����-����Ԥ��λ
	PTZDelPre,			// ��̨����-ɾ��Ԥ��λ
	PTZSpeed,			// ��̨����-��̨�ٶ�
	Palette,					// ��ɫ��
	Shutter,				// �����Ų���
	NoShutter,			// �������Ų���
	MenuUp,				// �˵�����
	MenuDown,			// �˵�����
	MenuAdd,			// �˵���
	MenuReduce,		// �˵���
	MenuC,				// �˵�C��
	ExpertMenu,			// ר�Ҳ˵�
	Enhance,				// ͼ����ǿ
	Smooth,				// ͼ��ƽ��	
	BitRate,					// ������
	SubBitRate,			// ������
	VideoOutputEncoding,// ģ����Ƶ�����ʽ
	Reserveway,			// ͼ��תģʽ
	SDCardReset,       //SD����ʽ��
	VSK,
	FID,
	GAIN,
	MFS,
	AFS_MAX,
	AFS_MIN,
	INTSET,
	AUTOCALIBRATE,
	VSKGEAR,
	COMPENSATETIME,
	SAVEIMG,
	FrameFrequency,
	SaveParameters,
} enumCommandType;

typedef int (CALLBACK* PROCESSRECV_RGBANDTEMPERATURE_CB)(const RGBANDTEMPERATUREDATA_CB *pInfo);

extern "C" ROBOTINSPECTION_API int  _stdcall StartGetRGBAndTemperatureData(const char* strUdpAddress, PROCESSRECV_RGBANDTEMPERATURE_CB fnCallBack, int iPaletteIndex);
extern "C" ROBOTINSPECTION_API int  _stdcall StopGetRGBAndTemperatureData();
extern "C" ROBOTINSPECTION_API int  _stdcall InitialMeasureParam(short * & curveData, int totalCurveLen, int curveDataLen, int  KF[5], int  B0[5],int K1, int initialTfpa, float Dis_a0,  float Dis_a1, float Dis_b0, float Dis_b1, float Dis_c0,float Dis_c1,int Emiss, int BkgTemperature,int AlarmThreshold, int Humidity, int K2, int K3, int K4, int B2);
extern "C" ROBOTINSPECTION_API int  _stdcall ModifyMeasureParam(int  KF, int K1, int B0,float  distance, int Emiss, int bkTemp,int AlarmThreshold,  int Humidity, int K2, int K3, int K4, int B2);
extern "C" ROBOTINSPECTION_API int  _stdcall ModifyDistanceParam(float  distance, float Dis_a0,  float Dis_a1, float Dis_b0, float Dis_b1, float Dis_c0,float Dis_c1);
extern "C" ROBOTINSPECTION_API int  _stdcall sendNetWorkCmd(enumCommandType iCmdType, int iCmdData, const char* strIpAddr);
extern "C" ROBOTINSPECTION_API int  _stdcall SaveIRImage(const char* strRtspAddress,const char* ImgFileName);
extern "C" ROBOTINSPECTION_API int  _stdcall DownLoadY16InSDCard(const char* strIpAddr, const char* file, int fileLength, BYTE* &destY16Array);

//
extern "C" ROBOTINSPECTION_API LONG _stdcall openImage(const char * szFilename);
extern "C" ROBOTINSPECTION_API LONG _stdcall GetTemperature(const char * szFilename,float * temperatureAarry, int Emiss);
extern "C" ROBOTINSPECTION_API int  _stdcall IsCameraOn(const char* strIpAddr);

extern "C" ROBOTINSPECTION_API  int ScopeStart(const char * out_file_name);
extern "C" ROBOTINSPECTION_API  int ScopeEnd();

extern ROBOTINSPECTION_API int nRobotInspection;

ROBOTINSPECTION_API int fnRobotInspection(void);
