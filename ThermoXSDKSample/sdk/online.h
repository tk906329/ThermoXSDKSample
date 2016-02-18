#ifndef ONLINE_H
#define ONLINE_H

#include "camera.h"

#include <WINSOCK2.H>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

/*
*/
typedef struct _DiscoverCameraResp{
	CameraBasicInfo BasicInfo;
	u32 CameraIp;
}DiscoverCameraResp, *PDiscoverCameraResp;

typedef enum _CtlType :u16{
	CtlType_Debug = 0,
	CtlType_ChangeDataType,
	CtlType_MoveShutter,
	CtlType_StoreFactory,

	CtlType_ManualFFC = 32,
	CtlType_RestoreFactory,
	CtlType_FocusManual,
	CtlType_FocusAuto,
	CtlType_SetAlarm,
}CtlType;

typedef struct _Ctl{
	CtlType Type;
	union {
		u8 reserved[8];
		enum _DataType DataType;
		u8 ShutterState;
		s16 FocusDelta;
		focus_info FocusInfo;
		GpioAlarm Alarm;
	}Data;

}Ctl;

typedef enum _CameraFileType{
	CFT_BIN = 0,
	CFT_CALI,
}CameraFileType;

typedef struct _SerialPortData{
	u8 DataSize;
	u8 Data[31];
}SerialPortData;

typedef void(__stdcall *DataFrameRecved)(uint cameraIp);


/*

*/
extern "C"{
	/*init-free, client*/
	CAMERA_API int InitClient();
	CAMERA_API void FreeClient();

	/*discover*/
	CAMERA_API int DiscoverOnlineCameras(DiscoverCameraResp* resps, uint respsSize);

	/*data*/
	CAMERA_API int StartTransData(uint cameraIp, TransType transType, DataType dataType, DataFrameRecved dataFrameRecved);
	CAMERA_API int PauseTransData(uint cameraIp);
	CAMERA_API void StopTransData(uint cameraIp);
	CAMERA_API const DataFrame* LockDataFrame(uint cameraIp);
	CAMERA_API void UnlockDataFrame(uint cameraIp);

	//config
	CAMERA_API int GetCameraBasicInfo(uint cameraIp, CameraBasicInfo& cameraBasicInfo);
	CAMERA_API int SetCameraBasicInfo(uint cameraIp, const CameraBasicInfo& cameraBasicInfo);

	//
	CAMERA_API int GetCameraNetworkInfo(uint cameraIp, CameraNetworkInfo& cameraNetworkInfo);
	CAMERA_API int SetCameraNetworkInfo(uint cameraIp, const CameraNetworkInfo& cameraNetworkInfo);

	//
	CAMERA_API int GetFFCInfo(uint cameraIp, FFCInfo& ffcInfo);
	CAMERA_API int SetFFCInfo(uint cameraIp, const FFCInfo& ffcInfo);

	//
	CAMERA_API int GetFPAInfo(uint cameraIp, FPAInfo& fpaInfo);
	CAMERA_API int SetFPAInfo(uint cameraIp, const FPAInfo& fpaInfo);

	//
	CAMERA_API int GetFixInfo(uint cameraIp, FixInfo& fixInfo);
	CAMERA_API int SetFixInfo(uint cameraIp, const FixInfo& fixInfo);

	//
	CAMERA_API int GetMeasureInfo(uint cameraIp, MeasureInfo& measureInfo);
	CAMERA_API int SetMeasureInfo(uint cameraIp, const MeasureInfo& measureInfo);

	//
	CAMERA_API int GetTvoutInfo(uint cameraIp, TvoutInfo& tvoutInfo);
	CAMERA_API int SetTvoutInfo(uint cameraIp, const TvoutInfo& tvoutInfo);

	//
	CAMERA_API int GetSerialPortInfo(uint cameraIp, SerialPortInfo& serialPortInfo);
	CAMERA_API int SetSerialPortInfo(uint cameraIp, const SerialPortInfo& serialPortInfo);

	//
	CAMERA_API int GetGpioInfo(uint cameraIp, GpioInfo& gpioInfo);
	CAMERA_API int SetGpioInfo(uint cameraIp, const GpioInfo& gpioInfo);

	/*
	*/
	CAMERA_API int SendControl(uint cameraIp, const Ctl& ctl);
	CAMERA_API int WriteSerialPort(uint cameraIp, const SerialPortData& spd);
	CAMERA_API int UploadFile(uint cameraIp, CameraFileType cft, const char* filename);
}


#endif