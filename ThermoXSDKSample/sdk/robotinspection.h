#pragma once
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ROBOTINSPECTION_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ROBOTINSPECTION_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef ROBOTINSPECTION_EXPORTS
#define ROBOTINSPECTION_API __declspec(dllexport)
#else
#define ROBOTINSPECTION_API __declspec(dllimport)
#endif

//返回指令
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
	BYTE* pRGBImgData;// rgb数据
	int RGBImgDataLen;// rgb数据长度
	double* MeasureMatrix; //温度矩阵
	int matrixDataLen;
	short* CurveData;
	int CurveLen;
	int ImgWidth;
	int ImgHeight;
	int iMFocusSpeed; //手动调焦速度
	int iTouchNearSwitch; //是否碰到近焦限位开关 0-否 1-是
	int iTouchFarSwitch; //是否碰到远焦限位开关 0-否 1-是
	int iAutoFocusFinish; //自动对焦是否完成 0-否 1-是
	int iAFocusForwardSpeed; //自动对焦前推速度
	int iAFocusBackSpeed; //自动对焦后推速度
	int iAFocusTotalFrame; //自动对焦统计帧数
	int iDimmingMode; //调光模式
	short AutoThrowPointRatio; //自动抛点比例
	short Brightness; 	//亮度
	short Contrast; 		//对比度
	short MDimY16Min;  //手动调光的Y16Min
	short MDimY16Max;  //手动调光的Y16Max
	short LsothermY16Min; //等温线Y16Min
	short LsothermY16Max; //等温线Y16Max
	int iSpecialColorNum; //特殊色彩编号
	int iLsothermMode; //等温线模式
	int iPaletteNum; //调色板
	int iShutterCompensationTimeSetting; //快门定时补偿时间设置
	int iShutterStatus; //快门状态：0-没有打快门 1-正在打快门
	int iRecoveryStatus; //恢复状态：1-恢复工厂设置成功 2-恢复用户设置成功
	int iSaveSetting; //保存设置 1-保存成功
	int iTimingCompensationFunction; //定时补偿功能 0-关 1-开
	short CursorStatus; //光标状态
	short CursorXValue; //X坐标
	short CursorYValue; //Y坐标
	short CursorADValue; //光标处AD值
	int iTestImageModeSetting; //测试图像模式设置
	int iRemoveBarAlgorithmSwitch; //去竖条算法开关
	int iRemoveNoiseAlgorithmSwitch; //去噪算法开关
	int iEnhanceAlgorithmSwitch; //增强算法开关
	int iFreezeSwitch; //定格开关
	int iDigitalContextSelectFlag; //数字口传输内容选择标志
	int iNETDTest; //NETD测试开关
	int iFrameRate; //帧频 25/9
	short DetectorTemp; //探测器焦平面温度	
	WORD	ProgramVersion;  //程序版本
	int iGFID;
	int iVSK;
	int iGAIN;
	int iIntegrationTime;
	int iEmiss;
	int iDistance;
	int iRelHum;
	int iBackgroundTemp;
	int iBackgroundTempFlag;
	WORD MeasureRangeInfo; //测温范围信息
	WORD VersionInfo; //正式版或测试版信息
	short DetectorGear;	//探测器档位
	WORD SecondChangeGearTime; //第二次切档时间
	short Y16_Max;//整场Y16最大值
	short Y16_Max_x;
	short Y16_Max_y;
	short Y16_Average;//y16中间值
	short Y16_Min;//y16最小值
	short Y16_Min_x;
	short Y16_Min_y;
	short Y16_Center;//Y16中间值CenterAD
	short Y16_Center_x;
	short Y16_Center_y;
	int iMeasureCursorShowMode; //测温光标显示模式 0-光标显示最大值和中心值（默认） 1-最大值和最小值 2-中心值和最小值
	int iAnalyzeMode; //分析模式 0-关闭分析 1-点分析 2-区域分析
	short Temp101; //信号板101温传温度
	short Area_x; //区域左上角X坐标
	short Area_y; //区域左上角Y坐标
	short AreaWidth; //区域宽
	short AreaHeight; //区域长
	short LastShutterTemp; //上一次快门温度
	int iY16Flag;
	short AtmosphereTemp;  //大气温度
	short RealShutterTemp; //实时快门温度
	int iT_Center;
	int iT_Max;
	int TempBeforeCorrection;
}RGBANDTEMPERATUREDATA_CB;

typedef enum CommandType
{
	FocusFar = 1,		// 调远焦	
	FocusNear,			// 调近焦 
	FocusStop,			// 调焦停
	AutoFocus,			// 自动聚焦
	ZoomOne,			// 1倍放大	
	ZoomTwo,			// 2倍放大
	ZoomFour,			// 4倍放大
	Bright,					// 亮度
	Contrast,				// 对比度
	PTZUp,					// 云台控制-上
	PTZDown,				// 云台控制-下
	PTZLeft,				// 云台控制-左
	PTZRight,				// 云台控制-右
	PTZLeftUp,			// 云台控制-左上
	PTZRightUp,			// 云台控制-右上
	PTZLeftDown,		// 云台控制-左下
	PTZRightDown,		// 云台控制-右下
	PTZStop,				// 云台控制-停
	PTZSetPre,			// 云台控制-设置预置位
	PTZRunPre,			// 云台控制-运行预置位
	PTZDelPre,			// 云台控制-删除预置位
	PTZSpeed,			// 云台控制-云台速度
	Palette,					// 调色板
	Shutter,				// 带快门补偿
	NoShutter,			// 不带快门补偿
	MenuUp,				// 菜单弹起
	MenuDown,			// 菜单按下
	MenuAdd,			// 菜单加
	MenuReduce,		// 菜单减
	MenuC,				// 菜单C键
	ExpertMenu,			// 专家菜单
	Enhance,				// 图像增强
	Smooth,				// 图像平滑	
	BitRate,					// 主码率
	SubBitRate,			// 子码率
	VideoOutputEncoding,// 模拟视频输出方式
	Reserveway,			// 图像翻转模式
	SDCardReset,       //SD卡格式化
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
