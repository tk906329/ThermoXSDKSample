#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"

/*
CameraBasicInfo
*/
typedef struct _CameraBasicInfo{
	char CameraId[STRING_LENGTH_32];
	char CameraName[STRING_LENGTH_32];
	char CameraType[STRING_LENGTH_16];

	char FPAId[STRING_LENGTH_32];

	char HardwareId[STRING_LENGTH_32];
	char SoftwareId[STRING_LENGTH_32];

	u16 DataWidth;
	u16 DataHeight;

	u16 DataFps;
	u16 DataRatio;

	u8 reserved[32];
}CameraBasicInfo, *PCameraBasicInfo;

typedef struct _CameraNetworkInfo{
	bool UseStaticIp;
	u8 pad;
	u16 MulticastPort;

	u32 StaticIp;
	u32 SubnetMask;
	u32 Gateway;

	u32 MulticastIp;

	u8 MacAddr[6];
	u8 reserved[26];
}CameraNetworkInfo, *PCameraNetworkInfo;

/*
FPAInfo
*/
typedef struct _fpa_vol4{
	u16 gsk;
	u16 gfid;
	u16 vsk;
	u16 vbus;
}fpa_vol4;

typedef enum _fpa_cint :u8{
	FPA_CINT_0 = 0,
	FPA_CINT_1,
	FPA_CINT_2,
	FPA_CINT_3
}fpa_cint;

typedef u16 fpa_tint;

typedef struct _FPAInfo{
	fpa_vol4 vol4;
	fpa_tint tint;
	fpa_cint  cint;
	u8 pad;
}FPAInfo, *PFPAInfo;

/*
FFCInfo
*/
typedef struct _ffc_cfg{
	u8 frames_skipped_after_close;
	u8 frames_accumlated_when_closed;
	u8 frames_skipped_after_open;
	u8 pad;
	u16 time_interval;
	u16 temp_interval;
}ffc_cfg;

typedef struct _FFCInfo{
	ffc_cfg ffc;
	u8 reserved[8];
}FFCInfo, *PFFCInfo;

/*
FixInfo
*/
typedef struct _FixInfo{
	float AtmosphericTemperature;
	float RelativeHumidity;
	float Visibility;
	float RainfallIntensity;
	float SnowfallIntensity;
	float TargetDistance;

	//
	float GlobalEmissivity;
	float InfraredWindowTrans;
	float TempOffset;

	bool EnableAtmFix;
	u8 reserved[35];
}FixInfo;

/*
MeasureInfo
*/
typedef struct _measure_point{
	u16 x;
	u16 y;
	u16 radius;
}measure_point;

typedef struct _measure_line{
	u16 x0;
	u16 y0;
	u16 x1;
	u16 y1;
	u16 radius;
}measure_line;

typedef struct _measure_rectangle{
	u16 xmin;
	u16 ymin;
	u16 xmax;
	u16 ymax;
}measure_rectangle;

typedef enum _measure_type :u8{
	MEASURE_TYPE_POINT = 0,
	MEASURE_TYPE_LINE = 1,
	MEASURE_TYPE_RECTANGLE = 2,
	MEASURE_TYPE_ELLIPSE = 3,
}measure_type;

typedef enum _alarm_type :u8{
	ALARM_TYPE_NONE = 0,
	ALARM_TYPE_MAX,
	ALARM_TYPE_MIN,
	ALARM_TYPE_MID
}alarm_type;

typedef struct _measure_object{
	char measure_id[16];
	enum _measure_type measure_type;
	enum _alarm_type alarm_type;
	u8 pad[2];
	float alarm_min;
	float alarm_max;
	float emissivity;

	union {
		measure_point point;
		measure_line line;
		measure_rectangle rectangle;
	}geometry;

	u8 reserved[14];
}measure_object;

typedef struct _MeasureInfo{
	s32					MOC;
	measure_object		MOS[8];
}MeasureInfo;

/*
TvoutInfo
*/
typedef enum _palette_type :u8{
	PT_Autumn = 0,
	PT_BlackHot,
	PT_GlowBow,
	PT_HighContrast,
	PT_HotMetal,
	PT_IronBow,
	PT_Jet,
	PT_RainBow,
	PT_RedSaturation,
	PT_WhiteHot,
	PT_Winter
}palette_type;

typedef enum _tv_std_id :u8{
	TV_STD_NTSC_M = 0,
	TV_STD_PAL_BDGHI,
	TV_STD_PAL_M,
	TV_STD_PAL_N,
	TV_STD_PAL_NC,
	TV_STD_PAL_60,
	TV_STD_NTSC_443,

	TV_STD_MAX
}tv_std_id;

typedef struct _TvoutInfo{
	bool				EnableTvout;
	palette_type		PaletteType;
	tv_std_id			TvStdId;

	bool EnableGpt;
	u8 Contrast;
	u8 Brightness;
	u8					Zoom;
	u8					reserved[33];
}TvoutInfo;

/*
SerialPortInfo
*/
typedef enum _DataBits :u8 {
	DataBits_5 = 5,
	DataBits_6,
	DataBits_7,
	DataBits_8,
} DataBits;

typedef enum _Parity :u8 {
	Parity_None = 0,
	Parity_Odd,
	Parity_Even,
} Parity;

typedef enum _StopBits :u8 {
	StopBits_1 = 0,
	StopBits_2,
} StopBits;

typedef enum _BaudRateType :u8 {
	BRT_2400 = 0,
	BRT_4800,
	BRT_9600,

	BRT_19200,
	BRT_38400,
	BRT_57600,
	BRT_115200,
} BaudRateType;

typedef enum _SerialPortUsage :u8{
	SPU_Custom = 0,
	SPU_PelcoD,
	SPU_PelcoP
}SerialPortUsage;

typedef struct _SerialPortInfo{

	enum _SerialPortUsage Usage;
	u8 PelcoAddr;
	enum _BaudRateType BaudRate;
	enum _DataBits DataBits;
	enum _StopBits StopBits;
	enum _Parity Parity;

	u8 reserved[10];
}SerialPortInfo;

/*
GpioInfo
*/
typedef enum _GpioInputType :u8{
	GIT_None = 0,
	GIT_Check,
	GIT_TakeSnapshot,
}GpioInputType;

typedef enum _GpioOutputType :u8{
	GOT_None = 0,
	GOT_Open,
}GpioOutputType;

typedef struct _GpioInfo{
	GpioInputType Input0;
	GpioInputType Input1;
	GpioOutputType Output0;
	GpioOutputType Output1;

	u8 reserved[12];
}GpioInfo;

/*
CameraInfo
*/
typedef struct _CameraInfo{
	CameraBasicInfo			BasicInfo;
	CameraNetworkInfo		NetworkInfo;
	struct _FPAInfo			FPAInfo;
	struct _FFCInfo			FFCInfo;

	struct _FixInfo			FixInfo;

	struct _MeasureInfo		MeasureInfo;
	struct _TvoutInfo		TvoutInfo;

	struct _SerialPortInfo	SerialPortInfo;

	struct _GpioInfo		GpioInfo;

	u8 reserved[240];
}CameraInfo, *PCameraInfo;

/*
DataFrame
*/
typedef enum _TransType :u8{
	TransType_Tcp = 0,
	TransType_UdpUnicast,
	TransType_UdpMulticast
}TransType;

typedef enum _DataType :u8{
	DataType_Vol = 0,
	DataType_VolDelta,
	DataType_Temp
}DataType;

#define DATA_FRAME_HEADER_SIZE					128
typedef struct _DataFrameHeader{
	u16 Width;
	u16 Height;

	u32 ComSize;

	enum _DataType DataType;
	u8 pad;

	//
	u16 Index;

	u16 Slope;
	s16 Offset;

	s32 FPATemp;
	s32 ShellTemp;

	//reserved 104,
	u8 UavInput;
	GpioInputType GpioInput0;
	GpioInputType GpioInput1;
	u8 reserved[101];
}DataFrameHeader, *PDataFrameHeader;

typedef struct _DataFrame{
	PDataFrameHeader pHeader;
	void* pCom;
	s16* pTemp;
	u8* pBmp;

	struct _DataFrame* prev;
}DataFrame, *PDataFrame;


/*
*/
typedef struct _focus_info{
	u16 xmin;
	u16 ymin;
	u16 xmax;
	u16 ymax;
}focus_info;

typedef enum _GpioAlarmType :u8{
	GAT_Manual = 0,
	GAT_Auto
}GpioAlarmType;

typedef enum _GpioAlarmValue :u8{
	GAV_Low = 0,
	GAV_High,
	GAV_Ignore,
}GpioAlarmValue;

typedef struct _GpioAlarm{
	GpioAlarmValue value0;
	GpioAlarmType type0;
	u8 time0;

	GpioAlarmValue value1;
	GpioAlarmType type1;
	u8 time1;
}GpioAlarm;



#endif