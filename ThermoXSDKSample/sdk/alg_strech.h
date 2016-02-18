#ifndef ALG_STRECH_H
#define ALG_STRECH_H

#include "camera.h"

#pragma pack(push, 1)

#define SCT_STRECH_TYPE			1
#define SCT_CONTRAST			2
#define SCT_BRIGHTNESS			4
#define SCT_GPT					8
#define SCT_TS					16

typedef enum _strech_type :u8{
	STRECH_TYPE_NORMAL = 0,
	STRECH_TYPE_TS,
}strech_type;

typedef struct _temp_seg{
	s16 temp0;
	s16 temp1;
	s16 temp2;

	u8 gray0;
	u8 gray1;
	u8 gray2;
}temp_seg;

typedef struct _strech_control{
	u32 change_type;

	//
	strech_type strech_type;
	u8 contrast;
	u8 brightness;
	bool enable_gpt;

	//
	temp_seg ts;
}strech_control;

typedef struct _strech_cfg{
	/*
	effect vars
	*/
	strech_control sc;

	/*
	mid vars
	*/
	s16 temp_min;
	s16 temp_mid;
	s16 temp_max;

	/*
	core vars
	*/
	void* buffer;

	u32* pdf;
	u8* cvt;
	u8* cvt_cb;
}strech_cfg;

#pragma pack(pop)

extern "C"{
	/*
	create-free, strech
	*/
	CAMERA_API strech_cfg* strCreate();
	CAMERA_API void strFree(strech_cfg* cfg);

	/*
	*/
	CAMERA_API void strTemp2Bgra(strech_cfg* cfg, const s16* temp, const DataFrameHeader* header, bgra* bmp, const bgra* palette);

	/*
	change
	*/
	CAMERA_API int strSetCtl(strech_cfg* cfg, const strech_control* control);
	CAMERA_API void strGetCtl(strech_cfg* cfg, strech_control* control);
}

#endif